#include "reply_model.h"

using namespace std;
using namespace my_model;

int ReplyModel::getCode()
{
    return ReplyModel::code;
}

void ReplyModel::setCode(int co)
{
    ReplyModel::code = co;
}

std::string ReplyModel::getMsg()
{
    return ReplyModel::msg;
}

void ReplyModel::setMsg(std::string message)
{
    ReplyModel::msg = message;
}

std::string ReplyModel::getData()
{
    return ReplyModel::data;
}

void ReplyModel::setData(std::string da)
{
    ReplyModel::data = da;
}
