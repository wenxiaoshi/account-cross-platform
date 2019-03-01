
#include <string>

#include "source/sqlite3/sqlite3.h"

using namespace std;

namespace db_utils{

	class Database{
	public:

		Database();
		~Database();

		static Database* database; 

		bool init();

		
	
		bool isUserExist(string account);

		bool addUserAccount(string account, string password);
	
		int queryAccountId(string account);
	
	private:
		string TABLE_USER_ACCOUNT = "user_account";

		sqlite3 *sql = NULL;

		char *zErrMsg = NULL;
		int ret = 0;
		bool isHadInit = false;

		void checkAndCreateTable();

		bool insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value);
		bool update();


		bool isExist(string str_sql);
		bool isTableExist(string tableName);

	};
}
