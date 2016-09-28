#include "stdex/stdcc.h"
#include "stdex/file.h"
#include "stdex/string.h"
#include "stdex/math.h"
#include "stdex/iconv.h"
#include "stdex/time.h"
#include "stdex/uuid.h"
#include "stdex/config.h"
#include "stdex/logger.h"
#include "stdex/queue.h"
#include "stdex/base64.h"
#include "stdex/json.h"
#include "stdex/misc.h"


int main()
{
	string utf8raw = "你好";
	string utf8;
	wstring utf16;
	string gbk;

	utf16 = stdex::utf8_to_utf16(utf8raw);
	utf8 = stdex::utf16_to_utf8(utf16);

	assert(utf8 == utf8raw);

	gbk = stdex::utf8_to_gbk(utf8raw);
	utf8 = stdex::gbk_to_utf8(gbk);

	assert(utf8 == utf8raw);
	return 0;
}
