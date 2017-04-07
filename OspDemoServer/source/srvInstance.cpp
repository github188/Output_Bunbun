#include "srvInstance.h"

CServerInstance :: CServerInstance()
{
    
    NextState(S_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INS)][GetMain(EVENT_REQ_INS)] = &CServerInstance :: Idle_Req_Ins;

}

void CServerInstance :: DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
   
}

void CServerInstance :: InstanceEntry(CMessage *const pMsg)
{
    /*获取当前实例的状态*/
    u16 wCurState = CurState();

    switch (wCurState)
    {

        case S_STATE_IDLE:
        {
            Idle_Function(pMsg);
            break;
        }
        case S_STATE_ACK:
        {
            Ack_Function(pMsg);
            break;
        }
        case S_STATE_WORK:
        {
            Work_Function(pMsg);
            break;
        }
        case S_STATE_TERM:
        {
            Term_Function(pMsg);
            break;
        }
        
        default:
        {
            
            break;
        }
    }
    
}

u16 CServerInstance :: GetMain(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) % EVENT_T;
}

u16 CServerInstance :: GetBran(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) / EVENT_T;
}

void CServerInstance :: Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = (pMsg->event - OSP_USEREVENT_BASE) % EVENT_T;
    u16 wCurEventBran = (pMsg->event - OSP_USEREVENT_BASE) / EVENT_T;
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}

void CServerInstance :: Idle_Req_Ins(CMessage *const pMsg)
{
    printf("client %d has connected!\n", pMsg->srcnode);
    OspPost(pMsg->srcid, EVENT_Y, NULL, 0, pMsg->srcnode, MAKEIID(this->GetAppID(), this->GetInsID()), pMsg->dstnode);
    NextState(S_STATE_ACK);
}

void CServerInstance :: Idle_Ack_Ins(CMessage *const pMsg)
{

}
void CServerInstance :: Ack_Function(CMessage *const PMSG)
{

}

void CServerInstance :: Work_Function(CMessage *const pMsg)
{

}

void CServerInstance :: Term_Function(CMessage *const pMsg)
{

}