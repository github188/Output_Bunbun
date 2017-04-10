#ifndef _CLTAPP_H_
#define _CLTAPP_H_


#include "cltInstance.h"

/*
template <class I, int maxins = 1, class A = CAppNoData, u8 maxAliasLen = 0>
*/
class CClientApp : public zTemplate <CClientInstance, 1, CAppNoData, 0>
{




};

#endif //cltApp.h