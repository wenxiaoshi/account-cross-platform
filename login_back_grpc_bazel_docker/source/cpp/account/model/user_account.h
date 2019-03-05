
#include <string>

using namespace std;

namespace my_model{
	
    /**
     *  用户账号Model
     **/
	class UserAccount{
	public:
		UserAccount(int uid, string account, string password);

		int getUid();
		void setUid(int uid);

		string getAccount();
		void setAccount(string account);

		string getPassword();
		void setPassword(string password);

	private:
		int uid;
		string account;
		string password;
	};

}