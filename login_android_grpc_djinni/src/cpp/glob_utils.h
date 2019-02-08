//
// Created by melon on 2019/2/3.
//
#include <string>
#include "NativeUtils.h"

namespace utils{



    class Device{
    public:
        static std::string getDeviceId(){
            return NativeUtils::getDeviceId();
        }
    };

    class Storage{
    public:
        static int saveData(std::string key,std::string value){
            return NativeUtils::saveData(key,value);
        }
        static std::string getData(std::string key){
            return NativeUtils::getData(key);
        }
    };

    class String{
    public:
        static void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
            std::string::size_type pos1, pos2;
            pos2 = s.find(c);
            pos1 = 0;
            while(std::string::npos != pos2)
            {
                v.push_back(s.substr(pos1, pos2-pos1));

                pos1 = pos2 + c.size();
                pos2 = s.find(c, pos1);
            }
            if(pos1 != s.length())
                v.push_back(s.substr(pos1));
        }

        static void replaceAll(std::string & s, std::string const & t, std::string const & w)
        {
            std::string::size_type pos = s.find(t), t_size = t.size(), r_size = w.size();
            while(pos != std::string::npos){ // found
                s.replace(pos, t_size, w);
                pos = s.find(t, pos + r_size );
            }
        }
    };

}