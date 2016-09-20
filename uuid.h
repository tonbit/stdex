#ifndef STDEX_UUID_H_
#define STDEX_UUID_H_

#include "stdex/stdcc.h"

#ifdef _MSC_VER
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

namespace stdex {

#ifdef _MSC_VER

inline string get_uuid()
{
	GUID id;
	CoCreateGuid(&id);

	char buf[64];
	snprintf(buf, sizeof(buf), "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
	    id.Data1, id.Data2, id.Data3,
	    id.Data4[0], id.Data4[1],
	    id.Data4[2], id.Data4[3], id.Data4[4],
	    id.Data4[5], id.Data4[6], id.Data4[7]);

	return buf;
}

#else

inline string get_uuid()
{
	uuid_t id;
	uuid_generate(id);

	char buf[64];
	snprintf(buf, sizeof(buf), "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
	    id[0], id[1], id[2], id[3],
	    id[4], id[5], id[6], id[7], id[8], id[9],
	    id[10], id[11], id[12],
	    id[13], id[14], id[15]);

	return buf;
}

#endif

}
#endif //STDEX_UUID_H_
