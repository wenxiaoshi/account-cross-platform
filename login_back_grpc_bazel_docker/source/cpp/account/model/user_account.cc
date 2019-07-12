
#include "user_account.h"

using namespace my_model;
using namespace std;

UserAccount::UserAccount(int uid, string account, string password, string pwdSalt) : uid(uid),account(account),password(password),pwdSalt(pwdSalt) {

}

int UserAccount::getUid(){
	return uid;
}
void UserAccount::setUid(int uid){
	this->uid = uid;
}

string UserAccount::getAccount(){
	return account;
}
void UserAccount::setAccount(string account){
	this->account = account;
}

string UserAccount::getPassword(){
	return password;
}
void UserAccount::setPassword(string password){
	this->password = password;
}

string UserAccount::getPwdSalt(){
	return pwdSalt;
}
void UserAccount::setPwdSalt(string pwdSalt){
	this->pwdSalt = pwdSalt;
}
