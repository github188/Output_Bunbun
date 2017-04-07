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
    /*��ȡ��ǰʵ����״̬*/
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
    ���¼��Ľ��� ������֧����
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
    Idle״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = (wCurEvent - OSP_USEREVENT_BASE) % EVENT_T;
    u16 wCurEventBran = (wCurEvent - OSP_USEREVENT_BASE) / EVENT_T;
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}


/*
    Idle״̬��EVENT_INSCONNECT�¼��صĴ���
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
    Ack״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Ack_Function(CMessage *const PMSG)
{

}

/*
    Work״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Work_Function(CMessage *const pMsg)
{

}

/*
    Term״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Term_Function(CMessage *const pMsg)
{

}