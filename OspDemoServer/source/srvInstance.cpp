#include "srvInstance.h"



CServerInstance :: CServerInstance()
{
  
    NextState(S_STATE_IDLE);
    m_dwCurInsNum = 0;//��ʼ״̬��û���û�����
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CServerInstance :: Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CServerInstance :: Idle_Ack_InsConnect;

    AckEventFunction[GetMain(EVENT_REQ_CATOTHERS)][GetBran(EVENT_REQ_CATOTHERS)] = &CServerInstance :: Ack_Req_CatOthers;

    WorkEventFunction[GetMain(EVENT_TERM_CATOTHERS)][GetBran(EVENT_TERM_CATOTHERS)] = &CServerInstance :: Work_Term_CatOthers;
}


void CServerInstance :: DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    u16 wCurEvent = pMsg->event;
    if(EVENT_REQ_INSCONNECT == pMsg->event)
    {
        if(m_dwCurInsNum < MAXINS) //��¼���ӵ��û���Ϣ
        {
 
            m_pUserInfo[m_dwCurInsNum].pMsg = (CMessage *)malloc(sizeof(CMessage));
            memcpy(m_pUserInfo[m_dwCurInsNum].pMsg, pMsg, sizeof(CMessage));
            m_pUserInfo[m_dwCurInsNum].dwNumber = m_dwCurInsNum + 1;
            m_pUserInfo[m_dwCurInsNum].dwState = 1;
            strcpy(m_pUserInfo[m_dwCurInsNum].pByAlias, (s8*)pMsg->content);
            m_dwCurInsNum++;
        }
        else
        {
            printf("��������æ���Ժ�����(δʵ��)\n");
        
        }
    }
    else if(EVENT_REQ_CATOTHERS == wCurEvent)
    {
        CatOthers((CMessage*)pMsg->content);
        OspPost(pMsg->srcid, EVENT_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
    }

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
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}


/*
    Idle״̬��EVENT_INSCONNECT�¼��صĴ���
*/
void CServerInstance :: Idle_Req_InsConnect(CMessage *const pMsg)
{
    printf("client %d has connected!\n", pMsg->srcnode);
    OspPost(pMsg->srcid, EVENT_ACK_INSCONNECT, NULL, 0, pMsg->srcnode, MAKEIID(this->GetAppID(), this->GetInsID()), pMsg->dstnode);
    printf("����������ACK״̬  insNum = %d\n", GetInsID());
    NextState(S_STATE_ACK);
}

void CServerInstance :: Idle_Ack_InsConnect(CMessage *const pMsg)
{

}

/*
    Ack״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Ack_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*AckEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CServerInstance :: Ack_Req_CatOthers(CMessage *const pMsg)
{
    OspPost(pMsg->srcid, EVENT_ACK_CATOTHERS, 0, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(SRV_APP_NO, CServerInstance :: DAEMON), EVENT_REQ_CATOTHERS, pMsg, sizeof(CMessage), 0, MAKEIID(SRV_APP_NO, GetInsID()));
    printf("ͬ��鿴�û����󣬷��������빤��״̬\n");
    NextState(S_STATE_WORK);
   
    
}

/*
    Work״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Work_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*WorkEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CServerInstance :: Work_Term_CatOthers(CMessage *const pMsg)
{
    printf("������������ֹ״̬\n");
    NextState(S_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);

}
/*
    Term״̬�Ը��¼������ѡ��
*/
void CServerInstance :: Term_Function(CMessage *const pMsg)
{
    printf("����������Ack״̬\n");
    NextState(S_STATE_ACK);
}