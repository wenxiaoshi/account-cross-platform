//
// Created by melon on 2019/2/25.
//

#include <string>
#include "../struct/hash_map.h"

#ifndef ANDROID_SHARE_PREFERENCES_H
#define ANDROID_SHARE_PREFERENCES_H

#endif //ANDROID_SHARE_PREFERENCES_H

using namespace my_struct;

namespace storage{

    class SharePreferences{
    public:
        
        /*
         主要功能
         保存键值对（缓存在内存中）
         
         入口参数
         key：           数据键
         value：         数据值
         */
        static bool save(std::string key, std::string value);
        
        /*
         主要功能
         保存键值对（直接操作写到文件）
         
         入口参数
         key：           数据键
         value：         数据值
         */
        static bool saveNow(std::string key, std::string value);
        
        /*
         主要功能
         获取键值对
         
         入口参数
         key：           数据键
         
         出口参数
         string：        数据值
         */
        static std::string get(std::string key);
        static bool execute();
    private:
        const static char* file_path;
        const static char* dir_path;
        const static char* file_name;

        static HashMap* shareMap;
        static bool readFileToSharePref();
        static bool writeFileToSharePref();
        static bool checkAndCreateSharePrefFile();

    };



}
