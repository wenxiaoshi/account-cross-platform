//
// Created by melon on 2019/2/3.
//
#include <string>
#include <vector>
//#include "network_result.h"
#include "../model/network_result.h"

using namespace demo;

namespace utils{

    class String{
    public:
        
        /*
         主要功能：
         分解字符串
         
         入口参数
         s：         分解源数据
         v：         分解后保存的数组
         c：         分解关键字符
         */
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

        /*
         主要功能：
         分解字符串
         
         入口参数
         s：         替换源数据
         t：         替换前关键词
         w：         替换后关键词
         */
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
