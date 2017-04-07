#include "cltInstance.h"


CClientInstance :: CClientInstance()
{



}

void CClientInstance :: DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    log( 0, (char *)"Osp: message received in default daemon instance of app %d.\nCheck your daemon implementation, please!\n", GetAppID() );

}

void CClientInstance :: InstanceEntry(CMessage *const pMsg)
{
    /*得到当前消息的类型*/
    u16 wCurEvent = pMsg->event;
    
    switch (wCurEvent)
    {
        case EVENT_Y:
            UserInterface();
            break;
        default:
            Def_Fuction(pMsg);
            break;
    }
    
}

/*
*消息反馈功能
*/
void CClientInstance :: ReqFunction(CMessage *const pMsg)
{
    u32 dwCurState = CurState();

    switch (dwCurState)
    {
        case 1:
            break;
        default:
            break;
    }

}

void CClientInstance :: AckFuction(CMessage *const pMsg)
{


}

void CClientInstance :: Term_Fuction(CMessage *const pMsg)
{

}

void CClientInstance :: Timeout_Function()
{

}

void CClientInstance :: Def_Fuction(CMessage *const pMsg)
{


}

void CClientInstance :: StartWork(CMessage *const pMsg)
{
   /* g_pMsg = (CMessage const*)malloc(sizeof(pMsg));
    memcpy((void*)g_pMsg, pMsg, sizeof(pMsg));
    printf("You have connected server......\n");
    */
}

void CClientInstance :: Printf_Function(CMessage *const pMsg)
{
    printf("client's info is %s\n", pMsg->content);
    

}
