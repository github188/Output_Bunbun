#include "cltInstance.h"


CClientInstance :: CClientInstance()
{
    NextState(C_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CClientInstance :: Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CClientInstance :: Idle_Ack_InsConnect;

}

void CClientInstance :: DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    log( 0, (char *)"Osp: message received in default daemon instance of app %d.\nCheck your daemon implementation, please!\n", GetAppID());

}

/*
    对事件的解析 主、分支函数
*/
u16 CClientInstance :: GetMain(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) % EVENT_T;
}

u16 CClientInstance :: GetBran(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) / EVENT_T;
}


void CClientInstance :: InstanceEntry(CMessage *const pMsg)
{
    /*得到当前消息的类型*/
    u16 wCurState = CurState();
    
    switch (wCurState)
    {
        case C_STATE_IDLE:
            Idle_Function(pMsg);
            break;
        case C_STATE_CONNECT:
            Connect_Function(pMsg);
            break;
        case C_STATE_REQ:
            Req_Function(pMsg);
            break;
        case C_STATE_WORK:
            Work_Function(pMsg);
            break;
        case C_STATE_TERM:
            Term_Function(pMsg);
            break;
        default:
            
            break;
    }
    
}

/*
    Idle状态对各事件处理的选择
*/
void CClientInstance :: Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = (wCurEvent - OSP_USEREVENT_BASE) % EVENT_T;
    u16 wCurEventBran = (wCurEvent - OSP_USEREVENT_BASE) / EVENT_T;
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}

void CClientInstance :: Idle_Req_InsConnect(CMessage *const pMsg)
{
    printf("turn into ready work status\n");
    NextState(C_STATE_CONNECT);

}

void CClientInstance :: Idle_Ack_InsConnect(CMessage *const pMsg)
{


}

void CClientInstance :: Connect_Function(CMessage *const pMsg)
{

}

void CClientInstance :: Req_Function(CMessage *const pMsg)
{

}

void CClientInstance :: Work_Function(CMessage *const pMsg)
{

}

void CClientInstance :: Term_Function(CMessage *const pMsg)
{

}