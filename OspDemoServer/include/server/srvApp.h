#include "srvInstance.h"

/*
template <class I, int maxins = 1, class A = CAppNoData, u8 maxAliasLen = 0>
*/
class CServerApp : public zTemplate <CServerInstance, 8, CAppNoData, 0>
{

public:
    CServerApp();

private:
    //virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    //virtual void InstanceEntry(CMessage *const pMsg);



};

CServerApp :: CServerApp()
{



}





