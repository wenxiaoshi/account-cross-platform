//
// Created by melon on 2019/2/24.
//

#ifndef ANDROID_FILE_UTILS_H
#define ANDROID_FILE_UTILS_H

#endif //ANDROID_FILE_UTILS_H

namespace utils{

    class File{
    public:
        
        /*
         主要功能：
         判断文件是否存在
         
         入口参数
         dir：           文件路径
         */
        static bool isFileExist(const char *dir);
        
        /*
         主要功能：
         创建文件
         
         入口参数
         file_pathL：            文件路径
         file_name：             文件名称
         */
        static bool createFile(const char *file_path, const char *file_name);
        
        /*
         主要功能：
         删除文件
         
         入口参数
         file_path：             文件路径
         */
        static bool deleteFile(const char* file_path);
        
        /*
         主要功能：
         往文件写数据
         
         入口参数
         fileName：              文件路径
         content：               写入内容
         */
        static bool writeFile(const char *fileName,const char *content);
        
        /*
         主要功能：
         从文件读数据
         
         入口参数
         fileName：              文件路径
         */
        static char* readFile(const char *fileName);

    private:
        static bool isDirExist(const char *dir);
        static void deleteDirsFile(const char *path);
        static void getFilePath(const char *path, const char *file_name,  char *file_path);
        static bool isDir(const char *path);
        static bool isFile(const char *path);
        static bool isSpecialDir(const char *path);
        static bool createDirs(const char *dir);
    };


}
