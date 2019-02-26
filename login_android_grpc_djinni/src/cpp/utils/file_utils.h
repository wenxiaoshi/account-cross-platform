//
// Created by melon on 2019/2/24.
//

#ifndef ANDROID_FILE_UTILS_H
#define ANDROID_FILE_UTILS_H

#endif //ANDROID_FILE_UTILS_H

namespace utils{

    class File{
    public:
        static bool isFileExist(const char *dir);
        static bool createFile(const char *file_path, const char *file_name);
        static bool deleteFile(const char* file_path);
        static bool writeFile(const char *fileName,const char *content);
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