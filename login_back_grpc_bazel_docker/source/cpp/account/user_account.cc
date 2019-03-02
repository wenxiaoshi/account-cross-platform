
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

int UserAccount::getAccount(){
	return account;
}
string UserAccount::setAccount(int account){
	this->account = account;
}

int UserAccount::getPassword(){

}
string UserAccount::setPassword(int password){
	return password;
}