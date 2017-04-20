#include "srvInstance.h"



CServerInstance::CServerInstance()
{
  
    NextState(S_STATE_IDLE);
    m_dwCurInsNum = 0;//��ʼ״̬��û���û�����
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CServerInstance::Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CServerInstance::Idle_Ack_InsConnect;

    AckEventFunction[GetMain(EVENT_REQ_CATOTHERS)][GetBran(EVENT_REQ_CATOTHERS)] = &CServerInstance::Ack_Req_CatOthers;
    AckEventFunction[GetMain(EVENT_REQ_TRANSINFO)][GetBran(EVENT_REQ_TRANSINFO)] = &CServerInstance::Ack_Req_TransInfo;
    AckEventFunction[GetMain(EVENT_REQ_SENDFILE)][GetBran(EVENT_REQ_SENDFILE)] = &CServerInstance::Ack_Req_SendFile;
    AckEventFunction[GetMain(EVENT_REQ_SENDCHAR)][GetBran(EVENT_REQ_SENDCHAR)] = &CServerInstance::Ack_Req_SendChar;
    AckEventFunction[GetMain(EVENT_REQ_DISCONNECT)][GetBran(EVENT_REQ_DISCONNECT)] = &CServerInstance::Ack_Req_DisConnect;
    AckEventFunction[GetMain(EVENT_ACK_DISCONNECT)][GetBran(EVENT_ACK_DISCONNECT)] = &CServerInstance::Ack_Ack_DisConnect;

    WorkEventFunction[GetMain(EVENT_TERM_CATOTHERS)][GetBran(EVENT_TERM_CATOTHERS)] = &CServerInstance::Work_Term_CatOthers;

    WorkEventFunction[GetMain(EVENT_TERM_TRANSINFO)][GetBran(EVENT_TERM_TRANSINFO)] = &CServerInstance::Work_Term_TransInfo;

    WorkEventFunction[GetMain(EVENT_ACK_SENDFILE)][GetBran(EVENT_ACK_SENDFILE)] = &CServerInstance::Work_Ack_SendFile;
    WorkEventFunction[GetMain(EVENT_TERM_SENDFILE)][GetBran(EVENT_TERM_SENDFILE)] = &CServerInstance::Work_Term_SendFile;

    WorkEventFunction[GetMain(EVENT_ACK_SENDCHAR)][GetBran(EVENT_ACK_SENDCHAR)] = &CServerInstance::Work_Ack_SendChar;
    WorkEventFunction[GetMain(EVENT_TERM_SENDCHAR)][GetBran(EVENT_TERM_SENDCHAR)] = &CServerInstance::Work_Term_SendChar;

    TermEventFunction[GetMain(EVENT_REQ_DISCONNECT)][GetBran(EVENT_REQ_DISCONNECT)] = &CServerInstance::Term_Req_DisConnect;


}


void CServerInstance::DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    CSUserInfo *cSUer = new CSUserInfo;
    cSUer->pMsg = new CMessage;
    u16 wCurEvent = pMsg->event;
    u16 wFlag = 0;              //���û��Ƿ��������û�
    if(EVENT_REQ_INSCONNECT == pMsg->event)
    {
        for(s32 i = 0; i < MAXINS; i++)
        {
            if(!strcmp(m_pUserInfo[i].pByAlias, (s8*)pMsg->content))
            {
                m_pUserInfo[i].dwState = 1;
                wFlag = 1;
                cSUer->wFlag = 1;
                OspPost(MAKEIID(GetAppID(), i + 1), EVENT_REQ_INSCONNECT, pMsg, sizeof(CMessage), 0);
                break;
            }
        
        }
        if(wFlag)
        {
            //OspPost(MAKEIID(GetAppID(), m_dwCurInsNum + 1), EVENT_REQ_INSCONNECT, pMsg, sizeof(CMessage), 0);
        }
        else
        {
            if(m_dwCurInsNum <= MAXINS) //��¼���ӵ��û���Ϣ
            {
                m_pUserInfo[m_dwCurInsNum].pMsg = (CMessage *)malloc(sizeof(CMessage));
                memcpy(m_pUserInfo[m_dwCurInsNum].pMsg, pMsg, sizeof(CMessage));
                m_pUserInfo[m_dwCurInsNum].dwNumber = m_dwCurInsNum + 1;
                m_pUserInfo[m_dwCurInsNum].dwState = 1;
                strcpy(m_pUserInfo[m_dwCurInsNum].pByAlias, (s8*)pMsg->content);
                OspPost(MAKEIID(GetAppID(), m_dwCurInsNum + 1), EVENT_REQ_INSCONNECT, pMsg, sizeof(CMessage), 0);
                m_dwCurInsNum++;
           
            }
            else
            {
                printf("��������æ���û�����������������ƣ����Ժ�����(δʵ��)\n");
        
            }
        }
    }
    else if(EVENT_REQ_CATOTHERS == wCurEvent)
    {
        catOthers((CMessage*)pMsg->content);
        OspPost(pMsg->srcid, EVENT_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
    }
    else if(EVENT_REQ_DISCONNECT == wCurEvent)
    {
        OspPost(pMsg->srcid, EVENT_ACK_DISCONNECT, pMsg->content, sizeof(CMessage), 0);
        
    }
    else if(EVENT_REQ_TRANSINFO == wCurEvent)
    {
        transInfo((CMessage*)pMsg->content);
        
    }
    else if(EVENT_ACK_TRANSINFO == wCurEvent)
    {
        
        //ת�����ͻ���Ϣ
        OspPost(m_pUserInfo[((CTransInfoBuffer *)pMsg->content)->dwUserNum - 1].pMsg->srcid,
            EVENT_REQ_TRANSINFO,
            ((CTransInfoBuffer *)pMsg->content),
            sizeof(CTransInfoBuffer),
            m_pUserInfo[((CTransInfoBuffer *)pMsg->content)->dwUserNum - 1].pMsg->srcnode,
            MAKEIID(GetAppID(), GetInsID()));
       //��֪ʵ����һ��ҵ�����
        OspPost(MAKEIID(GetAppID() , ((CTransInfoBuffer *)pMsg->content)->dwCurIns), EVENT_TERM_TRANSINFO, NULL, 0, 0, 
            MAKEIID(GetAppID(), GetInsID()));
    }
    else if(OSP_DISCONNECT == wCurEvent)
    {
        //printf("�û�%s�쳣�Ͽ�\n", pMsg->);
    }
}

void CServerInstance::InstanceEntry(CMessage *const pMsg)
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
u16 CServerInstance::GetMain(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) % EVENT_T;
}

u16 CServerInstance::GetBran(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) / EVENT_T;
}

/*
    Idle״̬�Ը��¼������ѡ��
*/
void CServerInstance::Idle_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}


/*
    Idle״̬��EVENT_INSCONNECT�¼��صĴ���
*/
void CServerInstance::Idle_Req_InsConnect(CMessage *const pMsg)
{
    /*��¼���ʵ�����ӵĿͻ�����Ϣ*/
    if(m_pCurUser.dwState == 1)     //���û�ԭ���Ѵ���
    {
        m_pCurUser.dwState = ONLINE;
        strcpy(m_pCurUser.pByAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_pCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_pCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /*���ö������*/
        OspSetHBParam(m_pCurUser.pMsg->srcnode, 2, 2);

        /*����ʱ֪ͨDaemon*/
        OspNodeDiscCBReg(m_pCurUser.pMsg->srcnode, GetAppID(), GetInsID());
        
        printf("�û� %s ������!\n", m_pCurUser.pByAlias);

        OspPost(((CMessage *)pMsg->content)->srcid, EVENT_ACK_INSCONNECT, &m_Rcd, sizeof(m_Rcd), ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));
    }
    else
    {
        m_pCurUser.dwState = ONLINE;
        strcpy(m_pCurUser.pByAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_pCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_pCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /*���ö������*/
        OspSetHBParam(m_pCurUser.pMsg->srcnode, 2, 2);

        /*����ʱ֪ͨDaemon*/
        OspNodeDiscCBReg(m_pCurUser.pMsg->srcnode, GetAppID(), GetInsID());

        /*��¼�û���*/
        strcpy(m_Rcd.pUsername, m_pCurUser.pByAlias);

        printf("�û� %s ������!\n", m_pCurUser.pByAlias);
        OspPost(((CMessage *)pMsg->content)->srcid, EVENT_ACK_INSCONNECT, NULL, 0, ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));
        printf("������ʵ��%d�ѽ���ACK״̬\n", GetInsID());
        NextState(S_STATE_ACK);
    }
}

void CServerInstance::Idle_Ack_InsConnect(CMessage *const pMsg)
{

}

/*
    Ack״̬�Ը��¼������ѡ��
*/
void CServerInstance::Ack_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*AckEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CServerInstance::Ack_Req_CatOthers(CMessage *const pMsg)
{
    OspPost(pMsg->srcid, EVENT_ACK_CATOTHERS, 0, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(SRV_APP_NO, CServerInstance::DAEMON), EVENT_REQ_CATOTHERS, pMsg, sizeof(CMessage), 0, MAKEIID(SRV_APP_NO, GetInsID()));
    printf("ͬ��鿴�û����󣬷�����ʵ��%d���빤��״̬\n", GetInsID());
    NextState(S_STATE_WORK);
     
}

void CServerInstance::Ack_Req_TransInfo(CMessage *const pMsg)
{
    OspPost(pMsg->srcid, EVENT_ACK_TRANSINFO, 0, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(SRV_APP_NO, CServerInstance::DAEMON), EVENT_REQ_TRANSINFO, pMsg, sizeof(CMessage), 0, MAKEIID(SRV_APP_NO, GetInsID()));
    printf("Daemon���ڸ��û���ʾ��Ϣ\n");
    printf("������ʵ��%d���빤��״̬\n", GetInsID());
    NextState(S_STATE_WORK);

}

void CServerInstance::Ack_Req_SendFile(CMessage *const pMsg)
{
    OspPost(pMsg->srcid, EVENT_ACK_SENDFILE, NULL, 0, pMsg->srcnode, pMsg->dstid);
    printf("ͬ������û�������ļ���������ʵ��%d���빤��״̬\n", GetInsID());
    NextState(S_STATE_WORK);

}

void CServerInstance::Ack_Req_SendChar(CMessage *const pMsg)
{
    OspPost(pMsg->srcid, EVENT_ACK_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
    printf("ͬ������û�������Ϣ��������ʵ��%d���빤��״̬\n", GetInsID());
    NextState(S_STATE_WORK);
}

void CServerInstance::Ack_Req_DisConnect(CMessage *const pMsg)
{
    OspPost(MAKEIID(GetAppID(), CServerInstance::DAEMON), EVENT_REQ_DISCONNECT, pMsg, sizeof(CMessage), 0, MAKEIID(GetAppID(), GetInsID()));//�������Daemon�������
}

void CServerInstance::Ack_Ack_DisConnect(CMessage *const pMsg)
{
    
    OspPost(((CMessage *)pMsg->content)->srcid, EVENT_ACK_DISCONNECT, NULL, 0, ((CMessage *)pMsg->content)->srcnode);
    printf("�û�%s���˳�����\n", m_pCurUser.pByAlias);
    printf("������ʵ��%d�������״̬\n", GetInsID());
    NextState(S_STATE_IDLE);
}
/*
    Work״̬�Ը��¼������ѡ��
*/
void CServerInstance::Work_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if(wCurEvent < OSP_USEREVENT_BASE)
    {
        switch (wCurEvent)
        {
            case OSP_DISCONNECT:
            {
                Work_OspDisConnect(pMsg);
                break;
            }
            default:
                break;
        }
    }
    else
    {
        u16 wCurEventMain = GetMain(wCurEvent);
        u16 wCurEventBran = GetBran(wCurEvent);
        (this->*WorkEventFunction[wCurEventMain][wCurEventBran])(pMsg);
    }
}

void CServerInstance::Work_Term_CatOthers(CMessage *const pMsg)
{
    printf("������ʵ��%d������ֹ״̬\n", GetInsID());
    NextState(S_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);

}

void CServerInstance::Work_Term_TransInfo(CMessage *const pMsg)
{
    printf("������ʵ��%d������ֹ״̬\n", GetInsID());
    NextState(S_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);

}
void CServerInstance::Work_Ack_SendFile(CMessage *const pMsg)
{
    rcvFile(pMsg);
}

void CServerInstance::Work_Ack_SendChar(CMessage *const pMsg)
{
    rcvChar(pMsg);

}
void CServerInstance::Work_Term_SendFile(CMessage *const pMsg)
{
    printf("������ʵ��%d������ֹ״̬\n", GetInsID());
    NextState(S_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
}

void CServerInstance::Work_OspDisConnect(CMessage *const pMsg)
{
    printf("�ͻ����쳣�Ͽ���,���������Ϣ\n");
    saveDisConnectInfo();
    printf("������ʵ��%d�������״̬\n", GetInsID());
    NextState(S_STATE_IDLE);
    //OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);

}


void CServerInstance::Work_Term_SendChar(CMessage *const pMsg)
{
    printf("������ʵ��%d������ֹ״̬\n", GetInsID());
    NextState(S_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
}
/*
    Term״̬�Ը��¼������ѡ��
*/
void CServerInstance::Term_Function(CMessage *const pMsg)
{
    printf("������ʵ��%d����Ack״̬\n", GetInsID());
    NextState(S_STATE_ACK);
}

void CServerInstance::Term_Req_DisConnect(CMessage *const pMsg)
{
    OspPost(MAKEIID(GetAppID(), CServerInstance::DAEMON), EVENT_REQ_DISCONNECT, NULL, 0, 0);
}