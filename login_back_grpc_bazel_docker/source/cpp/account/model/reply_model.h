
#include <string>

using namespace std;

namespace my_model{

class ReplyModel
{

public:
  int getCode();
  void setCode(int co);
  std::string getMsg();
  void setMsg(std::string message);
  std::string getData();
  void setData(std::string da);

  private:
  int code = 0;
  std::string msg = "";
  std::string data = "";

};

}