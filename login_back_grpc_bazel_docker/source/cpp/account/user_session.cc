
#include "user_session.h"

using namespace my_model;
using namespace std;

UserSession::UserSession(int uid, string token, bool isOnline) : uid(uid),token(token),isOnline(isOnline) {

}

int UserSession::getUid(){
	return uid;
}
void UserSession::setUid(int uid){
	this->uid = uid;
}

string UserSession::getToken(){
	return token;
}
void UserSession::setToken(string token){
	this->token = token;
}

bool UserSession::isOnline(){
	return isOnline;
}
void UserSession::setOnline(bool isOnline){
	this.isOnline = isOnline;
}