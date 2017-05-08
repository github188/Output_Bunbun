#include "cltInstance.h"


static void printE(u16 wEvent)
{
    printf("���¼���%hd\n", wEvent);
}

static void itoa (s32 n, s8 achAns[12])
{
    s32 i, k;
    s8 achS[12];
    if (n < 0)
    {
        n = -n;
    }
    i = 0;
    do{
        achS[i++] = n % 10 + '0';
    }while ((n /= 10) > 0);
    i--;
    for (k = 0; i >= 0; i--, k++)
    {
        achAns[k] = achS[i];
    }
    achAns[k] = '\0';
} 

CClientInstance::CClientInstance()
{
    NextState(C_STATE_IDLE);    
}

void CClientInstance::DaemonInstanceEntry(CMessage *const pMsg, CApp *const pApp)
{
    /* DaemonInstanceEntry */
}

void CClientInstance::InstanceEntry(CMessage *const pMsg)
{    
    u16 wCurState = CurState(); /* �õ���ǰʵ����״̬ */
    switch (wCurState)
    {
        case C_STATE_IDLE:
            IdleFunction(pMsg);
            break;
        case C_STATE_CONNECT:
            ConnectFunction(pMsg);
            break;
        case C_STATE_REQ:
            ReqFunction(pMsg);
            break;
        case C_STATE_WORK:
            WorkFunction(pMsg);
            break;
        case C_STATE_TERM:
            TermFunction(pMsg);
            break;
        default:           
            break;
    }
    
}

/*
    Idle״̬�Ը��¼������ѡ��
*/
void CClientInstance::IdleFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_REQ_INSCONNECT:
        {
            /* �������Daemon�������ʵ�� */
            s32 rtn = -1;
            rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance::DAEMON),
                EV_REQ_INSCONNECT, ((PTClientUserInfo)pMsg->content)->m_achUsername, 
                sizeof(((PTClientUserInfo)pMsg->content)->m_achUsername),
                g_ptConnectInfo->m_nDstnode, MAKEIID(CLT_APP_NO, 1));
            if (!rtn)
            {       
                printf("���������������ʵ������ɹ�\n");
            }
            else
            {
                printf("���������������ʵ������ʧ��!\n");
            }
        }
            break;
        case EV_ACK_INSCONNECT:
        {
            IdleAckInsConnect(pMsg);
        }
            break;
        case EV_TERM_INSCONNECT:
        {
            printf("��δ���ӷ�����ʵ�����޷���ֹ\n");
        }
            break;
        case EV_TIMEOUT_INSCONNECT:
        {
            printf("���ӷ�����ʵ����ʱ\n");
        }
            break;
        default:
            break;
    }
    
}

void CClientInstance::IdleAckInsConnect(CMessage *const pMsg)
{  
    if (!(pMsg->content))    //��һ�����ӣ���¼������Ϣ
    {
        memcpy(g_ptConnectInfo->m_pMsg, pMsg, sizeof(CMessage));
        printf("��������������ʵ��,");
    }
    if (!(pMsg->content) || !strcmp("local", (s8 *)pMsg->content))
    {
        printf("��������״̬\n");
        NextState(C_STATE_CONNECT);
        OspTaskCreate(UserInterface, "userInterface", 100, 200<<10, 0, 0);
    }
    else if (pMsg->content)/* ������Ӧע�� */
    {
        printf("������������\n");
        g_ptConnectInfo->m_pMsg->srcid = pMsg->srcid;
        g_ptConnectInfo->m_pMsg->srcnode = pMsg->srcnode;

        if (!(((PTRcdInfo)pMsg->content)->m_bFinish)) /* �ϴδ���δ��� */
        {
            printf("�Ƿ������ϴε��ļ�(y/n)?\n");
            bool bCmd;
            s8 chC;
            while (chC = getchar())
            {
                if ('y' == chC)
                {
                    bCmd = true;
                    break;
                }
                else if ('n' == chC)
                {
                    bCmd = false;
                    break;
                }
                else
                {
                    printf("������ȷ��ָ��\n");
                }
            
            }
            if (bCmd)          /* ѡ������ */
            {
                printf("���빤��״̬\n");
                NextState(C_STATE_WORK);
                OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDFILE,
                    pMsg->content, sizeof(TRcdInfo), 0, 0);
            }
            else
            {
                printf("�������״̬\n");
                NextState(C_STATE_IDLE);
                OspPost(MAKEIID(GetAppID(),GetInsID()), EV_ACK_INSCONNECT,
                    "local", 6, 0, MAKEIID(GetAppID(),GetInsID()));
            }
        }
    }
}

/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance::ConnectFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if (!strcmp((s8 *)pMsg->content, "user"))
    {
        printf("����Ϣ���û��Զ������Ϣ%hd\n", wCurEvent);       
        OspPost(g_ptConnectInfo->m_pMsg->srcid, (EV_REQ + wCurEvent*EV_T),
            NULL, 0, g_ptConnectInfo->m_pMsg->srcnode,
            MAKEIID(CLT_APP_NO, 1));
        printf("��������״̬\n");
        NextState(C_STATE_REQ);
    }
    else
    {
        switch (wCurEvent)
        {
            case OSP_DISCONNECT:
            {
                printf("������������������������״̬\n");
                NextState(C_STATE_IDLE);
            }
                break;
            case EV_REQ_TRANSINFO:
            {
                printf("������%s������Ϣ : %s\n",
                    ((PTTransInfoBuffer)pMsg->content)->m_achUserName,
                    ((PTTransInfoBuffer)pMsg->content)->m_achContent);
            }
                break;
            default:
                break;
        }
    }
}

/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance::ReqFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_ACK_CATOTHERS:
        {
            printf("������ͬ��鿴�û�����\n");
            printf("���빤��״̬\n");
            NextState(C_STATE_WORK); 
        }
            break;
        case EV_ACK_TRANSINFO:
        {
            printf("�������������û��б�\n");
            printf("���빤��״̬\n");
            NextState(C_STATE_WORK);
        }
            break;
        case EV_ACK_SENDFILE:
        {
            printf("������ͬ����շ����ļ�\n");
            printf("���빤��״̬\n");
            NextState(C_STATE_WORK);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDFILE,
                NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_SENDCHAR:
        {
            printf("������ͬ�ⷢ��������Ϣ\n");
            printf("���빤��״̬");
            NextState(C_STATE_WORK);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDCHAR,
                NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_DISCONNECT:
        {
            printf("���ѶϿ������������\n");
            printf("�������״̬\n");
            NextState(C_STATE_IDLE);
        }
            break;
        default:
            break;
    }
}

/*
    ����״̬�¶Ը��¼������ѡ��
*/
void CClientInstance::WorkFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_ACK_CATOTHERS:
        {
            printf("�û�:%s ״̬:%s\n",
                ((PTUserInfo)pMsg->content)->m_achAlias,
                ((PTUserInfo)pMsg->content)->m_nState?"����":"����");
        }
            break;
        case EV_ACK_TRANSINFO:
        {
            printf("���: %d �û�:%s ״̬:%s\n",
                ((PTUserInfo)pMsg->content)->m_nNumber,
                ((PTUserInfo)pMsg->content)->m_achAlias,
                ((PTUserInfo)pMsg->content)->m_nState?"����":"����"); 
        }
            break;
        case EV_ACK_SENDFILE:
        {
            SendFile(pMsg);
        }
            break;
        case EV_ACK_SENDCHAR:
        {
            SendChar(pMsg);
        }
            break;
        case EV_TERM_CATOTHERS:
        {
            printf("��ʾ�û���Ϣ���\n");
            printf("������ֹ״̬\n");
            NextState(C_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1025, NULL, 0, 0);
        }
            break;
        case EV_TERM_TRANSINFO:
            WorkTermTransInfo(pMsg);
            break;
        case EV_TERM_SENDFILE:
        {
            printf("100%%�Ѵ���\n");
            printf("�ļ��ϴ����\n");
            printf("������ֹ״̬\n");
            NextState(C_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
        }
            break;
        case EV_TERM_SENDCHAR:
        {
            printf("�������ѽ�����Ϣ\n");
            printf("������ֹ״̬\n");
            NextState(C_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
        }
            break;
        default:
            break;

    }
}

void CClientInstance::WorkTermTransInfo(CMessage *const pMsg)
{
    PTTransInfoBuffer buffer = new TTransInfoBuffer;
    s8 pchInfo[BUFFSIZE];
    printf("��������Ҫ���͵���Ϣ:\n");
    scanf("%s", pchInfo);
    printf("��������Ҫת�����û����:\n");
    s32 nUserNum;
    scanf("%d", &nUserNum);
    memcpy(buffer->m_achContent, pchInfo, strlen(pchInfo) + 1);
    buffer->m_nUserNum = nUserNum;
    buffer->m_nCurIns = GETINS(g_ptConnectInfo->m_pMsg->srcid);
    memcpy(buffer->m_achUserName, g_ptCltUserInfo->m_achUsername,
        strlen(g_ptCltUserInfo->m_achUsername) + 1);
    OspPost(pMsg->srcid, EV_ACK_TRANSINFO, buffer,
        sizeof(TTransInfoBuffer), pMsg->srcnode, pMsg->dstid);
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1024, NULL, 0, 0);
}

/*
    ��ֹ״̬�¶Ը��¼������ѡ��
*/
void CClientInstance::TermFunction(CMessage *const pMsg)
{
    printE(pMsg->event);
    printf("����'q'�ս�˴�ҵ��\n");
    while (getchar() != 'q')
    {
    }
    printf("�˴�ҵ���ս�\n");
    printf("�������״̬\n");
    NextState(C_STATE_IDLE);
    OspPost(MAKEIID(GetAppID(),GetInsID()), EV_ACK_INSCONNECT,
        "local", 6, 0, MAKEIID(GetAppID(),GetInsID()));
}

/* ���ܴ����� */

/* ����������Ϣ */
void CClientInstance::SendChar(CMessage *pMsg)
{
    s8 SENDCHARBUFF[BUFFSIZE];
    printf("������������Ϣ:\n");
    scanf("%s", SENDCHARBUFF);
    OspPost(g_ptConnectInfo->m_pMsg->srcid, EV_ACK_SENDCHAR, SENDCHARBUFF,
        strlen(SENDCHARBUFF), g_ptConnectInfo->m_pMsg->srcnode,
        g_ptConnectInfo->m_pMsg->dstid);
}