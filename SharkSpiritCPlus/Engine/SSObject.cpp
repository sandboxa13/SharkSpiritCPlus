#include "SSObject.h"
#pragma comment(lib, "Rpcrt4.lib") 

using namespace std;

SSObject::SSObject()
{
	UuidCreate(&m_id);
}
