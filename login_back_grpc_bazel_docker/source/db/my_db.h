
#include <string>

#include "source/sqlite3/sqlite3.h"

namespace db_utils{

	class Database{
	public:

		Database();
		~Database();

		const static Database* database; 

	private:

		sqlite3 *sql;

		char *zErrMsg;
		int ret;

		bool isHadInit;

		int callback(void *NotUsed, int argc, char **argv, char **azColName);

		bool init();

		bool create();
		bool insert();
		bool update();
		bool query();
	}
}