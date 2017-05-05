#include "srvInstance.h"

CServerInstance::CServerInstance()
{
    NextState(S_STATE_IDLE);
    m_nCurInsNum = 0;/* 起始状态，没有用户连接 */
}

void CServerInstance::DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    PTCSUserInfo ptSUer = new CSUserInfo;
    ptSUer->m_pMsg = new CMessage;
    u16 wCurEvent = pMsg->event;
    u16 wFlag = 0;              /* 该用户是否是重连用户 */
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
            if (m_nCurInsNum <= MAXINS) /* 记录连接的用户信息 */
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
                printf("服务器正忙（用户连接数超过最大限制），稍后再试\n"); 
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
        
        /* 转发给客户消息 */
        OspPost(m_ptUserInfo[((PTCTransInfoBuffer)pMsg->content)->m_nUserNum - 1].pMsg->srcid,
            EV_REQ_TRANSINFO,
            ((PTCTransInfoBuffer)pMsg->content),
            sizeof(CTransInfoBuffer),
            m_ptUserInfo[((PTCTransInfoBuffer)pMsg->content)->m_nUserNum - 1].pMsg->srcnode,
            MAKEIID(GetAppID(), GetInsID()));
       /* 告知实例，一次业务结束 */
        OspPost(MAKEIID(GetAppID() , ((PTCTransInfoBuffer)pMsg->content)->m_nCurIns),
            EV_TERM_TRANSINFO, NULL, 0, 0, MAKEIID(GetAppID(), GetInsID()));
    }
    else if (OSP_DISCONNECT == wCurEvent)
    {
        printf("用户异常断开\n");
    }
}

void CServerInstance::InstanceEntry(CMessage *const pMsg)
{
    /*获取当前实例的状态*/
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
    Idle状态对各事件处理的选择
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
    Idle状态对EV_INSCONNECT事件簇的处理
*/
void CServerInstance::IdleReqInsConnect(CMessage *const pMsg)
{
    /* 记录与该实例连接的客户端信息 */
    if (1 == m_ptCurUser.m_nState)     /* 该用户原本已存在 */
    {
        m_ptCurUser.m_nState = ONLINE;
        strcpy(m_ptCurUser.m_achAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_ptCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_ptCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /* 设置断链检测 */
        OspSetHBParam(m_ptCurUser.pMsg->srcnode, 2, 2);

        /* 断链时通知Daemon */
        OspNodeDiscCBReg(m_ptCurUser.pMsg->srcnode, GetAppID(), GetInsID());
        
        printf("用户 %s 已重连!\n", m_ptCurUser.m_achAlias);

        OspPost(((CMessage *)pMsg->content)->srcid, EV_ACK_INSCONNECT,
            &m_tRcd, sizeof(m_tRcd), ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));

        printf("进入工作状态\n");
        NextState(S_STATE_WORK);
    }
    else
    {
        m_ptCurUser.m_nState = ONLINE;
        strcpy(m_ptCurUser.m_achAlias, (s8 *)((CMessage *)pMsg->content)->content);
        m_ptCurUser.pMsg = (CMessage *)malloc(sizeof(CMessage));
        memcpy(m_ptCurUser.pMsg, (CMessage *)pMsg->content, sizeof(CMessage));

        /* 设置断链检测 */
        OspSetHBParam(m_ptCurUser.pMsg->srcnode, 2, 2);

        /* 断链时通知Daemon */
        OspNodeDiscCBReg(m_ptCurUser.pMsg->srcnode, GetAppID(), GetInsID());

        /* 记录用户名 */
        strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);

        printf("用户 %s 已连接!\n", m_ptCurUser.m_achAlias);
        OspPost(((CMessage *)pMsg->content)->srcid, EV_ACK_INSCONNECT,
            NULL, 0, ((CMessage *)pMsg->content)->srcnode, 
            MAKEIID(this->GetAppID(), this->GetInsID()));
        printf("服务器实例%d已进入ACK状态\n", GetInsID());
        NextState(S_STATE_ACK);
    }
}


/*
    Ack状态对各事件处理的选择
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
            printf("同意查看用户请求，服务器实例%d进入工作状态\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_TRANSINFO:
        {
            OspPost(pMsg->srcid, EV_ACK_TRANSINFO, 0, 0, pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(SRV_APP_NO, CServerInstance::DAEMON),
                EV_REQ_TRANSINFO, pMsg, sizeof(CMessage), 0,
                MAKEIID(SRV_APP_NO, GetInsID()));
            printf("Daemon正在给用户显示信息\n");
            printf("服务器实例%d进入工作状态\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_SENDFILE:
        {
            OspPost(pMsg->srcid, EV_ACK_SENDFILE, NULL, 0, pMsg->srcnode, pMsg->dstid);
            printf("同意接受用户传输的文件，服务器实例%d进入工作状态\n", GetInsID());
            NextState(S_STATE_WORK);
        }
            break;
        case EV_REQ_SENDCHAR:
        {
            OspPost(pMsg->srcid, EV_ACK_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
            printf("同意接受用户文字消息，服务器实例%d进入工作状态\n", GetInsID());
            NextState(S_STATE_WORK);        
        }
            break;
        case EV_REQ_DISCONNECT:
        {
            /* 向服务器Daemon请求断连 */
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
            printf("用户%s已退出连接\n", m_ptCurUser.m_achAlias);
            printf("服务器实例%d进入空闲状态\n", GetInsID());
            NextState(S_STATE_IDLE);
        }
            break;
        default:
            break;
    }
}

/*
    Work状态对各事件处理的选择
*/
void CServerInstance::WorkFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case OSP_DISCONNECT:
        {
            printf("客户端异常断开了,保存断链信息\n");
            SaveDisConnectInfo();
            printf("服务器实例%d进入空闲状态\n", GetInsID());
            NextState(S_STATE_IDLE);
        }
            break;
        case EV_TERM_CATOTHERS:
        {
            printf("服务器实例%d进入终止状态\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        case EV_TERM_TRANSINFO:
        {
            printf("服务器实例%d进入终止状态\n", GetInsID());
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
            printf("服务器实例%d进入终止状态\n", GetInsID());
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
            printf("服务器实例%d进入终止状态\n", GetInsID());
            NextState(S_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0, 0);
        }
            break;
        default:
            break;
    }
}

/*
    Term状态对各事件处理的选择
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
            printf("服务器实例%d进入Ack状态\n", GetInsID());
            NextState(S_STATE_ACK);
            break;
    }
}

/* 功能执行函数 */

/* 查看其他用户 */
void CServerInstance::CatOthers(CMessage *const pMsg)
{
    for (s32 i = 0; i < m_nCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EV_ACK_CATOTHERS, &m_ptUserInfo[i],
            sizeof(m_ptUserInfo[i]), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EV_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
}

/* 接收文本信息 */
void CServerInstance::RcvChar(CMessage *pMsg)
{
    printf("%s发送的消息 : %s\n", m_ptCurUser.m_achAlias, pMsg->content);
    OspPost(pMsg->srcid, EV_TERM_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EV_TERM_SENDCHAR, NULL, 0, 0);
}

/* 转发文本信息 */
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