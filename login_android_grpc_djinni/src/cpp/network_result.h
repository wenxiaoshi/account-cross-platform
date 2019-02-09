//
// Created by melon on 2019/2/3.
//

#include <string>

namespace demo{
    class ReqResult{
        public:
            int getCode();
            void setCode(std::string code);
            std::string getMsg();
            void setMsg(std::string msg);
            std::string getData();
            void setData(std::string data);
        private:
            int code = 0;
            std::string msg = "";
            std::string data = "";
        };
}
