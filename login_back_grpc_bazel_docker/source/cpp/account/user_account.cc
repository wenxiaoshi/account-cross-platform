
#include "user_account.h"

using namespace my_model;
using namespace std;

UserAccount::UserAccount(int uid, string account, string password) : uid(uid),account(account),password(password) {

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
void UserAccount::setAccount(int account){
	this->account = account;
}

string UserAccount::getPassword(){

}
void UserAccount::setPassword(int password){
	return password;
}