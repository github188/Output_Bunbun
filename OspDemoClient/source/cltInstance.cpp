#include "cltInstance.h"

void printE(u16 wEvent)
{
    printf("���¼���%hd\n", wEvent);
}
CClientInstance :: CClientInstance()
{
    NextState(C_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CClientInstance :: Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CClientInstance :: Idle_Ack_InsConnect;

    ReqEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance :: Req_Ack_CatOthers;
    ReqEventFunction[GetMain(EVENT_ACK_SENDFILE)][GetBran(EVENT_ACK_SENDFILE)] = &CClientInstance :: Req_Ack_SendFile;

    WorkEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance :: Work_Ack_CatOthers;
    WorkEventFunction[GetMain(EVENT_TERM_CATOTHERS)][GetBran(EVENT_TERM_CATOTHERS)] = &CClientInstance :: Work_Term_CatOthers;

    WorkEventFunction[GetMain(EVENT_ACK_SENDFILE)][GetBran(EVENT_ACK_SENDFILE)] = &CClientInstance :: Work_Ack_SendFile;
    WorkEventFunction[GetMain(EVENT_TERM_SENDFILE)][GetBran(EVENT_TERM_SENDFILE)] = &CClientInstance :: Work_Term_SendFile;

}

void CClientInstance :: DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    log( 0, (char *)"Osp: message received in default daemon instance of app %d.\nCheck your daemon implementation, please!\n", GetAppID());

}

/*
    ���¼��Ľ��� ������֧����
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
    /*�õ���ǰ��Ϣ������*/
    u16 wCurState = CurState();
    
    switch (wCurState)
    {
        case C_STATE_IDLE:
            Idle_Function(pMsg);
            break;
        case C_STATE_CONNECT:
            Connect_Function(pMsg);
            printf("��������״̬\n");
            NextState(C_STATE_REQ);
            //SetTimer(EVENT_TIMEOUT, 1000);
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
    Idle״̬�Ը��¼������ѡ��
*/
void CClientInstance :: Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}

void CClientInstance :: Idle_Req_InsConnect(CMessage *const pMsg)
{

    /*����ͬʱ����Ҫ����������һ��ʵ������ÿͻ���,�Ա����ͨѶʹ��*/
    s32 rtn = -1;
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: DAEMON), EVENT_REQ_INSCONNECT, ((CUerInfo *)pMsg->content)->username, \
        sizeof(((CUerInfo *)pMsg->content)->username), g_pConnectInfo->dstnode, MAKEIID(CLT_APP_NO, 1));
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: PENDING), EVENT_REQ_INSCONNECT, NULL, 0, g_pConnectInfo->dstnode, MAKEIID(CLT_APP_NO, 1));
    if(0 == rtn)
    {
        
        printf("����������ʵ���ɹ�!\n");
    }
    else
    {
        printf("����������ʵ��ʧ��!\n");
    }
    
    

}

void CClientInstance :: Idle_Ack_InsConnect(CMessage *const pMsg)
{
   
    if(pMsg->event == EVENT_ACK_INSCONNECT)
    {
        if(NULL == pMsg->content)
        {
            memcpy(g_pConnectInfo->pMsg, pMsg, sizeof(CMessage));
        }
        printf("��������������ʵ������������״̬\n");
        NextState(C_STATE_CONNECT);
        UserInterface();
    }
    
    
}

void CClientInstance :: Idle_Term_InsConnect(CMessage *const pMsg)
{
    printf("��δ���ӷ�����ʵ�����޷���ֹ\n");
}

void CClientInstance :: Idle_Timeout_InsConnect(CMessage *const pMsg)
{
    printf("���ӷ�����ʵ����ʱ\n");
}

/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance :: Connect_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if(wCurEvent < 10)
    {
        printf("����Ϣ���û��Զ������Ϣ%hd\n", wCurEvent);
        
        OspPost(g_pConnectInfo->pMsg->srcid, (EVENT_REQ + wCurEvent*EVENT_T), 0, 0, g_pConnectInfo->pMsg->srcnode, MAKEIID(CLT_APP_NO, 1));

    }
    else
    {
        
        u16 wCurEventMain = GetMain(wCurEvent);
        u16 wCurEventBran = GetBran(wCurEvent);
        (this->*ConnectEventFunction[wCurEventMain][wCurEventBran])(pMsg);
    }
}


/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance :: Req_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*ReqEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CClientInstance :: Req_Ack_CatOthers(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    printf("������ͬ��鿴�û�����\n");
    printf("���빤��״̬\n");
    NextState(C_STATE_WORK);
    
}

void CClientInstance :: Req_Ack_SendFile(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    printf("������ͬ����շ����ļ�\n");
    printf("���빤��״̬\n");
    NextState(C_STATE_WORK);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_ACK_SENDFILE, NULL, 0, 0, 0);

}

/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance :: Work_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*WorkEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CClientInstance :: Work_Ack_CatOthers(CMessage *const pMsg)
{
    printf("�û�:%s ״̬:����\n", pMsg->content);
}

void CClientInstance :: Work_Ack_SendFile(CMessage *const pMsg)
{
   
    sendFile(pMsg);

}

void CClientInstance :: Work_Term_CatOthers(CMessage *const pMsg)
{
    printf("��ʾ�û���Ϣ���\n");
    printf("������ֹ״̬\n");
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
}

void CClientInstance :: Work_Term_SendFile(CMessage *const pMsg)
{
    printf("100%%�Ѵ���\n");
    printf("�ļ��ϴ����\n");
    printf("������ֹ״̬\n");
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
}
/*
    ��ֹ״̬�¶Ը��¼������ѡ��
*/
void CClientInstance :: Term_Function(CMessage *const pMsg)
{
    printE(pMsg->event);
    printf("����'q'�ս�˴�ҵ��\n");
    while(getchar() != 'q')
    {
    }
    printf("�˴�ҵ���ս�\n");
    printf("�������״̬\n");
    NextState(C_STATE_IDLE);
    OspPost(MAKEIID(GetAppID(),GetInsID()), EVENT_ACK_INSCONNECT, "local", 6, 0);
}


