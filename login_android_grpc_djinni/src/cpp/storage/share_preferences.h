//
// Created by melon on 2019/2/25.
//

#include <string>
#include <struct/hash_map.h>

#ifndef ANDROID_SHARE_PREFERENCES_H
#define ANDROID_SHARE_PREFERENCES_H

#endif //ANDROID_SHARE_PREFERENCES_H

using namespace my_struct;

namespace storage{

    class SharePreferences{
    public:
        static bool save(std::string key, std::string value);
        static bool saveNow(std::string key, std::string value);
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