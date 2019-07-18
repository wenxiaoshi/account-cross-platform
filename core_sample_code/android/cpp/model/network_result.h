//
// Created by melon on 2019/2/3.
//

#ifndef ANDROID_REQ_RESULT
#define ANDROID_REQ_RESULT

#include <string>

namespace demo{
    class ReqResult{
        public:
            int getCode();
            void setCode(std::string code);
            void setCode(int code);
            std::string getMsg();
            void setMsg(std::string msg);
            std::string getData();
            void setData(std::string data);
        private:
            int code = -1;
            std::string msg = "";
            std::string data = "";
        };
}

#endif //ANDROID_REQ_RESULT