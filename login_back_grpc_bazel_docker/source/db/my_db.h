
#include <string>

#include "source/sqlite3/sqlite3.h"
#include "source/cpp/account/user_account.h"
#include "source/cpp/account/user_session.h"

using namespace std;
using namespace my_model;

namespace db_utils{

	class Database{
	public:

		Database();
		~Database();

		static Database* database; 

		//数据库初始化
		bool init();

		//TABLE_USER_ACCOUNT 相关
		bool isUserExist(string account)
		bool addUserAccount(string account, string password);
		UserAccount queryUserAccountByAccount(string account);
	
		//TABLE_USER_SESSION 相关
		bool addUserSession(int uid, string token);
		bool updateUserSession(int uid, string token, bool isOnline);
		UserSession queryUserSessionByUid(int uid);

	private:

		//数据库表名
		string TABLE_USER_ACCOUNT = "user_account";
		string TABLE_USER_SESSION = "user_session";

		//数据库Sqlite实例指针
		sqlite3 *sql = NULL;

		//变参
		char *zErrMsg = NULL;
		int ret = 0;
		bool isHadInit = false;

		//数据库操作基础方法
		void checkAndCreateTable();
		bool insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value);
		bool update(string tabls_name, std::vector<string> v_key, std::vector<string> v_value, std::vector<string> where_key, std::vector<string> where_value);
		bool isExist(string str_sql);
		bool isTableExist(string tableName);

	};
}
