
#include "string"

namespace log_utils{

	class LOG
	{
	public:
		static void error(string info);
	private:
		static void init();
		static bool isHadInit;
	};

}