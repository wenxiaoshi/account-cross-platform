//
// Created by melon on 2019/2/24.
//


#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <string>

#include "file_utils.h"
#include "../log/my_log.h"

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGW(msg)  utils::LogUtil::LOGW(msg);

//判断是否为目录
bool utils::File::isDir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return false;
}

//判断是否为常规文件
bool utils::File::isFile(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//判断文件是否为常规文件
    return false;
}

//判断是否是特殊目录
bool utils::File::isSpecialDir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void utils::File::getFilePath(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}
//删除文件目录下的所有文件
void utils::File::deleteDirsFile(const char *path)
{
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    if(isFile(path))
    {
        remove(path);
        return;
    }
    if(isDir(path))
    {
        if((dir = opendir(path)) == NULL)
            return;
        while((dir_info = readdir(dir)) != NULL)
        {
            getFilePath(path, dir_info->d_name, file_path);
            if(isSpecialDir(dir_info->d_name))
                continue;
            deleteDirsFile(file_path);
            rmdir(file_path);
        }
    }
}

//判断文件是否存在
bool utils::File::isFileExist(const char *dir) {
    if (dir == NULL)
        return false;

    return (access(dir, F_OK)) != -1;
}
//判断文件目录是否存在
bool utils::File::isDirExist(const char *dir) {
    if (dir == NULL)
        return false;
    if (opendir(dir) == NULL)
    {
        LOGD("file dir is not exist");
        return false;
    }
    LOGD("file dir had exist");
    return true;
}


//创建多级目录

bool utils::File::createDirs(const char *dir) {
    if (dir == NULL)
        return false;
    char DirName[256];
    strcpy(DirName, dir);
    int i, len = strlen(DirName);
    if (DirName[len - 1] != '/')
        strcat(DirName, "/");
    len = strlen(DirName);
    for (i = 1; i < len; i++) {
        if (DirName[i] == '/') {
            DirName[i] = 0;
            //判断路径是否存在，如果不存在则创建路径
            if (!isDirExist(DirName) ) {
                if (mkdir(DirName,0777) == -1) {
                    perror("mkdir   error");
                    return false;
                }
            }
            DirName[i] = '/';
        }
    }
    return true;
}

//创建文件可读写的文件
bool utils::File::createFile(const char *file_path, const char *file_name) {
    if(isDirExist(file_path)){
        //文件已经存在
        if (isFileExist(file_name)) {
            return  true;
        } else{
            if(open(file_name,O_RDWR | O_CREAT, S_IRWXU)!=-1){
                return  true;
            }
        }
    }
    else{
        //文件目录不存在，创建目录
        if (createDirs(file_path)) {
            // 创建目录成功，这里去创建文件
            if(open(file_name,O_RDWR | O_CREAT, S_IRWXU)!=-1)
                return  true;
        }

    }
    return  false;

}
//删除文件
bool utils::File::deleteFile(const char* file_path) {
    if (file_path == NULL) {
        return false;
    }
    return remove(file_path) != -1;

}
//以追加的方式写入文件
bool utils::File::writeFile(const char *fileName,const char *content) {
    FILE *fp;
    fp = fopen(fileName, "w");
    if (NULL == fp) {
        return false;
    }
    if( fwrite(content, strlen(content), 1, fp)!=1)
        return false;
    fclose (fp);
    return true;
}

//读文件内容
char* utils::File::readFile(const char *fileName) {
    FILE *fp;
    fp = fopen(fileName, "r");
    if (NULL == fp) {
        return NULL;
    }

    //求得文件的大小
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    //申请一块能装下整个文件的空间
    char* ar = (char*)malloc(sizeof(char)*size);

    //读文件
    fread(ar,size,1,fp);//每次读一个，共读size次

    fclose (fp);
//    free(ar);

    return ar;
}
