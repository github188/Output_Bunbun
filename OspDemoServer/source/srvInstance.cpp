#include "srvInstance.h"

CServerInstance::CServerInstance()
{
    NextState(S_STATE_IDLE);
    m_nCurInsNum = 0;/* ��ʼ״̬��û���û����� */
}

void CServerInstance::DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    PTCSUserInfo ptSUer = new CSUserInfo;
    ptSUer->m_pMsg = new CMessage;
    u16 wCurEvent = pMsg->event;
    u16 wFlag = 0;              /* ���û��Ƿ��������û� */
    if (EV_REQ_INSCONNECT == pMsg->event)
    {
        for (s32 i = 0; i < MAXINS; i++)
        {
            if (!strcmp(m_ptUserInfo[i].m_achAlias, (s8*)pMsg->content))
            {
                m_ptUserInfo[i].m_nState = 1;
                wFlag = 1;
                ptSUer->m_wFlag = 1;
                OspPost(MAKEIID(GetAppID(), i + 1), EV_REQ_INSCONNECT, pMsg,
                    sizeof(CMessage), 0);
                break;
            }
        }
        if (wFlag)
        {
            //OspPost(MAKEIID(GetAppID(), m_nCurInsNum + 1), EV_REQ_INSCONNECT, pMsg, sizeof(CMessage), 0);
        }
        else
        {
            if (m_nCurInsNum <= MAXINS) /* ��¼���ӵ��û���Ϣ */
            {
                m_ptUserInfo[m_nCurInsNum].pMsg = new CMessage();
                memcpy(m_ptUserInfo[m_nCurInsNum].pMsg, pMsg, sizeof(CMessage));
                m_ptUserInfo[m_nCurInsNum].m_nNumber = m_nCurInsNum + 1;
                m_ptUserInfo[m_nCurInsNum].m_nState = 1;
                strcpy(m_ptUserInfo[m_nCurInsNum].m_achAlias, (s8*)pMsg->content);
                OspPost(MAKEIID(GetAppID(), m_nCurInsNum + 1),
                    EV_REQ_INSCONNECT, pMsg, sizeof(CMessage), 0);
                m_nCurInsNum++;           
            }
            else
            {
                printf("��������æ���û�����������������ƣ����Ժ�����\n"); 
            }
        }
    }
    else if (EV_REQ_CATOTHERS == wCurEvent)
    {
        CatOthers((CMessage*)pMsg->content);
        OspPost(pMsg->srcid, EV_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
    }
    else if (EV_REQ_DISCONNECT == wCurEvent)
    {
        OspPost(pMsg->srcid, EV_ACK_DISCONNECT, pMsg->content,
            sizeof(CMessage), 0);       
    }
    else if (EV_REQ_TRANSINFO == wCurEvent)
    {
        TransInfo((CMessage*)pMsg->content);    
    }
    else if (EV_ACK_TRANSINFO == wCurEvent)
    {
        
        /* ת�����ͻ���Ϣ */
        OspPost(m_ptUserInfo[((PTCTransInfoBuffer)pMsg->content)->m_nUserNum - 1].pMsg->srcid,
            EV_REQ_TRANSINFO,
            ((PTCTransInfoBuffer)pMsg->content),
            sizeof(CTransInfoBuffer),
            m_ptUserInfo[((PTCTransInfoBuffer)pMsg->content)->m_nUserNum - 1].pMsg->srcnode,
            MAKEIID(GetAppID(), GetInsID()));
       /* ��֪ʵ����һ��ҵ����� */
        OspPost(MAKEIID(GetAppID() , ((PTCTransInfoBuffer)pMsg->content)->m_nCurIns),
            EV_TERM_TRANSINFO, NULL, 0, 0, MAKEIID(GetAppID(), GetInsID()));
    }
    else if (OSP_DISCONNECT == wCurEvent)
    {
        printf("�û��쳣�Ͽ�\n");
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
            IdleFunction(pMsg);
            break;
        }
        case S_STATE_ACK:
        {
            AckFunction(pMsg);
            break;
        }
        case S_STATE_WORK:
        {
            WorkFunction(pMsg);
            break;
        }
        case S_STATE_TERM:
        {
            TermFunction(pMsg);
            break;
        }   
        default:
        {    
            break;
        }
    }    
}

/*
    Idle״̬�Ը��¼������ѡ��
*/
void CServerInstance::IdleFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_REQ_INSCONNECT:
            IdleReqInsConnect(pMsg);
            break;
        default:
            break;
    }
}

/*
    Idle״̬��EV_INSCONNECT�¼��صĴ���
*/
void CServerInstance::IdleReqInsConnect(CMessage *const pMsg)
{
    /* ��¼���ʵ�����ӵĿͻ�����Ϣ */
    if (1 == m_ptCurUser.m_nState)     /* ���û�ԭ���Ѵ��� */
    {
        m_ptCurUser.m_nState = ONLINE;
        strcpy(m_ptCurUser.m_achAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_ptCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_ptCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /* ���ö������ */
        OspSetHBParam(m_ptCurUser.pMsg->srcnode, 2, 2);

        /* ����ʱ֪ͨDaemon */
        OspNodeDiscCBReg(m_ptCurUser.pMsg->srcnode, GetAppID(), GetInsID());
        
        printf("�û� %s ������!\n", m_ptCurUser.m_achAlias);

        OspPost(((CMessage *)pMsg->content)->srcid, EV_ACK_INSCONNECT,
            &m_tRcd, sizeof(m_tRcd), ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));

        printf("���빤��״̬\n");
        NextState(S_STATE_WORK);
    }
    else
    {
        m_ptCurUser.m_nState = ONLINE;
        strcpy(m_ptCurUser.m_achAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_ptCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_ptCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /* ���ö������ */
        OspSetHBParam(m_ptCurUser.pMsg->srcnode, 2, 2);

        /* ����ʱ֪ͨDaemon */
        OspNodeDiscCBReg(m_ptCurUser.pMsg->srcnode, GetAppID(), GetInsID());

        /* ��¼�û��� */
        strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);

        printf("�û� %s ������!\n", m_ptCurUser.m_achAlias);
        OspPost(((CMessage *)pMsg->content)->srcid, EV_ACK_INSCONNECT,
            NULL, 0, ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));
        printf("������ʵ��%d�ѽ���ACK״̬\n", GetInsID());
        NextState(S_STATE_ACK);
    }
}


/*
    Ack״̬�Ը��¼������ѡ��
*/
void CServerInstance::AckFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_REQ_CATOTHERS:
        {
            OspPost(pMsg->srcid, EV_ACK_CATOTHERS, 0, 0, pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(SRV_APP_NO, CServerInstance::DAEMON),
                EV_REQ_CATOTHERS, pMsg, sizeof(CMessage), 0,
                MAKEIID(SRV_APP_NO, GetInsID()));
            printf("ͬ��鿴�û����󣬷�����ʵ��%d���빤��״̬\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_TRANSINFO:
        {
            OspPost(pMsg->srcid, EV_ACK_TRANSINFO, 0, 0, pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(SRV_APP_NO, CServerInstance::DAEMON),
                EV_REQ_TRANSINFO, pMsg, sizeof(CMessage), 0,
                MAKEIID(SRV_APP_NO, GetInsID()));
            printf("Daemon���ڸ��û���ʾ��Ϣ\n");
            printf("������ʵ��%d���빤��״̬\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_SENDFILE:
        {
            OspPost(pMsg->srcid, EV_ACK_SENDFILE, NULL, 0, pMsg->srcnode, pMsg->dstid);
            printf("ͬ������û�������ļ���������ʵ��%d���빤��״̬\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_SENDCHAR:
        {
            OspPost(pMsg->srcid, EV_ACK_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
            printf("ͬ������û�������Ϣ��������ʵ��%d���빤��״̬\n", GetInsID());
            NextState(S_STATE_WORK);        
        }
            break;
        case EV_REQ_DISCONNECT:
        {
            /* �������Daemon������� */
            OspPost(MAKEIID(GetAppID(), CServerInstance::DAEMON),
                EV_REQ_DISCONNECT, pMsg, sizeof(CMessage), 0,
                MAKEIID(GetAppID(), GetInsID()));
        }
            break;
        case EV_ACK_DISCONNECT:
        {
            OspPost(((CMessage *)pMsg->content)->srcid,
                EV_ACK_DISCONNECT, NULL, 0,
                ((CMessage *)pMsg->content)->srcnode);
            printf("�û�%s���˳�����\n", m_ptCurUser.m_achAlias);
            printf("������ʵ��%d�������״̬\n", GetInsID());
            NextState(S_STATE_IDLE);
        }
            break;
        default:
            break;
    }
}

/*
    Work״̬�Ը��¼������ѡ��
*/
void CServerInstance::WorkFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case OSP_DISCONNECT:
        {
            printf("�ͻ����쳣�Ͽ���,���������Ϣ\n");
            SaveDisConnectInfo();
            printf("������ʵ��%d�������״̬\n", GetInsID());
            NextState(S_STATE_IDLE);
        }
            break;
        case EV_TERM_CATOTHERS:
        {
            printf("������ʵ��%d������ֹ״̬\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        case EV_TERM_TRANSINFO:
        {
            printf("������ʵ��%d������ֹ״̬\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_SENDFILE:
        {
            RcvFile(pMsg);
        }
            break;
        case EV_TERM_SENDFILE:
        {
            printf("������ʵ��%d������ֹ״̬\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_SENDCHAR:
        {
            RcvChar(pMsg);
        }
            break;
        case EV_TERM_SENDCHAR:
        {
            printf("������ʵ��%d������ֹ״̬\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        default:
            break;
    }
}

/*
    Term״̬�Ը��¼������ѡ��
*/
void CServerInstance::TermFunction(CMessage *const pMsg)
{   
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_REQ_DISCONNECT:
        {
            OspPost(MAKEIID(GetAppID(), CServerInstance::DAEMON),
                EV_REQ_DISCONNECT, NULL, 0, 0);
        }
            break;
        default:
            printf("������ʵ��%d����Ack״̬\n", GetInsID());
            NextState(S_STATE_ACK);
            break;
    }
}

/* ����ִ�к��� */

/* �鿴�����û� */
void CServerInstance::CatOthers(CMessage *const pMsg)
{
    for (s32 i = 0; i < m_nCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EV_ACK_CATOTHERS, &m_ptUserInfo[i],
            sizeof(m_ptUserInfo[i]), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EV_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
}

/* �����ı���Ϣ */
void CServerInstance::RcvChar(CMessage *pMsg)
{
    printf("%s���͵���Ϣ : %s\n", m_ptCurUser.m_achAlias, pMsg->content);
    OspPost(pMsg->srcid, EV_TERM_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EV_TERM_SENDCHAR, NULL, 0, 0);
}

/* ת���ı���Ϣ */
void CServerInstance::TransInfo(CMessage *const pMsg)
{
    for (s32 i = 0; i < m_nCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EV_ACK_TRANSINFO, &m_ptUserInfo[i],
            sizeof(m_ptUserInfo[i]), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EV_TERM_TRANSINFO, NULL, 0, pMsg->srcnode,
        MAKEIID(GetAppID(), GetInsID()));

}