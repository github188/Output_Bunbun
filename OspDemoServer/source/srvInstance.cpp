#include "srvInstance.h"

CServerInstance :: CServerInstance()
{
    
    NextState(S_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CServerInstance :: Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CServerInstance :: Idle_Ack_InsConnect;

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

/*
    对事件的解析 主、分支函数
*/
u16 CServerInstance :: GetMain(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) % EVENT_T;
}

u16 CServerInstance :: GetBran(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) / EVENT_T;
}

/*
    Idle状态对各事件处理的选择
*/
void CServerInstance :: Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = (wCurEvent - OSP_USEREVENT_BASE) % EVENT_T;
    u16 wCurEventBran = (wCurEvent - OSP_USEREVENT_BASE) / EVENT_T;
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}


/*
    Idle状态对EVENT_INSCONNECT事件簇的处理
*/
void CServerInstance :: Idle_Req_InsConnect(CMessage *const pMsg)
{
    printf("client %d has connected!\n", pMsg->srcnode);
    OspPost(pMsg->srcid, EVENT_Y, NULL, 0, pMsg->srcnode, MAKEIID(this->GetAppID(), this->GetInsID()), pMsg->dstnode);
    NextState(S_STATE_ACK);
}

void CServerInstance :: Idle_Ack_InsConnect(CMessage *const pMsg)
{

}

/*
    Ack状态对各事件处理的选择
*/
void CServerInstance :: Ack_Function(CMessage *const PMSG)
{

}

/*
    Work状态对各事件处理的选择
*/
void CServerInstance :: Work_Function(CMessage *const pMsg)
{

}

/*
    Term状态对各事件处理的选择
*/
void CServerInstance :: Term_Function(CMessage *const pMsg)
{

}