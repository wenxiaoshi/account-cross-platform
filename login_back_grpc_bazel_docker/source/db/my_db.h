
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

		bool create();
		bool insertAccount(string account, string password);
		bool update();
                bool query();
		bool queryAccountId(string account);
	private:

		sqlite3 *sql = NULL;

		char *zErrMsg = NULL;
		int ret = 0;

		bool isHadInit = false;

		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		bool insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value);
		//bool create();
		//bool insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value);
		//bool update();
		//bool query();
	};
}
