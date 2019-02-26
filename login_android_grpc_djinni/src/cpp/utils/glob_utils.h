//
// Created by melon on 2019/2/3.
//
#include <string>
#include "NativeUtils.h"
#include <vector>
#include "network_result.h"

using namespace demo;

namespace utils{



    class Device{
    public:
        static std::string getDeviceId(){
            return NativeUtils::getDeviceId();
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

    class Network{
    public:
        static void getCodeByResult(ReqResult & result,std::string response){

            const std::string KEY_CODE = "code";
            const std::string KEY_MSG = "msg";
            const std::string KEY_DATA = "data";

            utils::String::replaceAll(response,"{","");
            utils::String::replaceAll(response,"}","");
            utils::String::replaceAll(response,"\"","");
            std::vector<std::string> v;

            utils::String::splitString(response, v,",");

            for(std::string map : v){
                std::vector<std::string> kv;
                utils::String::splitString(map, kv,":");

                if(kv[0] == KEY_CODE){
                    if(kv.size()>1){
                        result.setCode(kv[1]);
                    }
                }else if(kv[0] == KEY_MSG){
                    if(kv.size()>1){
                        result.setMsg(kv[1]);
                    }
                }else if(kv[0] == KEY_DATA){
                    if(kv.size()>1){
                        result.setData(kv[1]);
                    }
                }

            }
        }
    };

}