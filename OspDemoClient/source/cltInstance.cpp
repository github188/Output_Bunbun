#include "cltInstance.h"

/*******************************************************************************/

static void printE(u16 wEvent)
{
    printf("该事件是%hd\n", wEvent);
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
    u16 wCurState = CurState(); /* 得到当前实例的状态 */
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
    Idle状态对各事件处理的选择
*/
void CClientInstance::IdleFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_REQ_INSCONNECT:
        {
            /* 向服务器Daemon请求分配实例 */
            s32 rtn = -1;
            rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance::DAEMON),
                EV_REQ_INSCONNECT, ((PTCUserInfo)pMsg->content)->m_achUsername, 
                sizeof(((PTCUserInfo)pMsg->content)->m_achUsername),
                g_ptConnectInfo->m_nDstnode, MAKEIID(CLT_APP_NO, 1));
            if (!rtn)
            {       
                printf("向服务器发送连接实例请求成功\n");
            }
            else
            {
                printf("向服务器发送连接实例请求失败!\n");
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
            printf("尚未连接服务器实例，无法终止\n");
        }
            break;
        case EV_TIMEOUT_INSCONNECT:
        {
            printf("连接服务器实例超时\n");
        }
            break;
        default:
            break;
    }
    
}

void CClientInstance::IdleAckInsConnect(CMessage *const pMsg)
{  
    if (!(pMsg->content))    //第一次连接，记录连接信息
    {
        memcpy(g_ptConnectInfo->m_pMsg, pMsg, sizeof(CMessage));
        printf("已连接至服务器实例,");
    }
    if (!(pMsg->content) || !strcmp("local", (s8 *)pMsg->content))
    {
        printf("进入连接状态\n");
        NextState(C_STATE_CONNECT);
        OspTaskCreate(UserInterface, "userInterface", 100, 200<<10, 0, 0);
    }
    else if (pMsg->content)/* 重连响应注册 */
    {
        printf("已重连服务器\n");
        g_ptConnectInfo->m_pMsg->srcid = pMsg->srcid;
        g_ptConnectInfo->m_pMsg->srcnode = pMsg->srcnode;

        if (!(((PTCRcdInfo)pMsg->content)->m_bFinish)) /* 上次传送未完成 */
        {
            printf("是否续传上次的文件(y/n)?\n");
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
                    printf("输入正确的指令\n");
                }
            
            }
            if (bCmd)          /* 选择续传 */
            {
                printf("进入工作状态\n");
                NextState(C_STATE_WORK);
                OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDFILE,
                    pMsg->content, sizeof(CRcdInfo), 0, 0);
            }
            else
            {
                printf("进入空闲状态\n");
                NextState(C_STATE_IDLE);
                OspPost(MAKEIID(GetAppID(),GetInsID()), EV_ACK_INSCONNECT,
                    "local", 6, 0, MAKEIID(GetAppID(),GetInsID()));
            }
        }
    }
}

/*
    连接状态下对各事件处理的选择
*/
void CClientInstance::ConnectFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if (!strcmp((s8 *)pMsg->content, "user"))
    {
        printf("该消息是用户自定义的消息%hd\n", wCurEvent);       
        OspPost(g_ptConnectInfo->m_pMsg->srcid, (EV_REQ + wCurEvent*EV_T),
            NULL, 0, g_ptConnectInfo->m_pMsg->srcnode,
            MAKEIID(CLT_APP_NO, 1));
        printf("进入请求状态\n");
        NextState(C_STATE_REQ);
    }
    else
    {
        switch (wCurEvent)
        {
            case OSP_DISCONNECT:
            {
                printf("与服务器发生断链，进入空闲状态\n");
                NextState(C_STATE_IDLE);
            }
                break;
            case EV_REQ_TRANSINFO:
            {
                printf("有来自%s的新消息 : %s\n",
                    ((PTCTransInfoBuffer)pMsg->content)->m_achUserName,
                    ((PTCTransInfoBuffer)pMsg->content)->m_achContent);
            }
                break;
            default:
                break;
        }
    }
}

/*
    请求状态下对各事件处理的选择
*/
void CClientInstance::ReqFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_ACK_CATOTHERS:
        {
            printf("服务器同意查看用户请求\n");
            printf("进入工作状态\n");
            NextState(C_STATE_WORK); 
        }
            break;
        case EV_ACK_TRANSINFO:
        {
            printf("服务器正生成用户列表\n");
            printf("进入工作状态\n");
            NextState(C_STATE_WORK);
        }
            break;
        case EV_ACK_SENDFILE:
        {
            printf("服务器同意接收发送文件\n");
            printf("进入工作状态\n");
            NextState(C_STATE_WORK);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDFILE,
                NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_SENDCHAR:
        {
            printf("服务器同意发送文字消息\n");
            printf("进入工作状态");
            NextState(C_STATE_WORK);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EV_ACK_SENDCHAR,
                NULL, 0, 0, 0);
        }
            break;
        case EV_ACK_DISCONNECT:
        {
            printf("你已断开与服务器连接\n");
            printf("进入空闲状态\n");
            NextState(C_STATE_IDLE);
        }
            break;
        default:
            break;
    }
}

/*
    工作状态下对各事件处理的选择
*/
void CClientInstance::WorkFunction(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    switch (wCurEvent)
    {
        case EV_ACK_CATOTHERS:
        {
            printf("用户:%s 状态:%s\n",
                ((PTUserInfo)pMsg->content)->m_achAlias,
                ((PTUserInfo)pMsg->content)->m_nState?"在线":"离线");
        }
            break;
        case EV_ACK_TRANSINFO:
        {
            printf("序号: %d 用户:%s 状态:%s\n",
                ((PTUserInfo)pMsg->content)->m_nNumber,
                ((PTUserInfo)pMsg->content)->m_achAlias,
                ((PTUserInfo)pMsg->content)->m_nState?"在线":"离线"); 
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
            printf("显示用户信息完毕\n");
            printf("进入终止状态\n");
            NextState(C_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1025, NULL, 0, 0);
        }
            break;
        case EV_TERM_TRANSINFO:
            WorkTermTransInfo(pMsg);
            break;
        case EV_TERM_SENDFILE:
        {
            printf("100%%已传输\n");
            printf("文件上传完毕\n");
            printf("进入终止状态\n");
            NextState(C_STATE_TERM);
            OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
        }
            break;
        case EV_TERM_SENDCHAR:
        {
            printf("服务器已接收消息\n");
            printf("进入终止状态\n");
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
    PTCTransInfoBuffer buffer = new CTransInfoBuffer;
    s8 pchInfo[BUFFSIZE];
    printf("请输入你要发送的消息:\n");
    scanf("%s", pchInfo);
    printf("请输入你要转发的用户序号:\n");
    s32 nUserNum;
    scanf("%d", &nUserNum);
    memcpy(buffer->m_achContent, pchInfo, strlen(pchInfo) + 1);
    buffer->m_nUserNum = nUserNum;
    buffer->m_nCurIns = GETINS(g_ptConnectInfo->m_pMsg->srcid);
    memcpy(buffer->m_achUserName, g_ptCltUserInfo->m_achUsername,
        strlen(g_ptCltUserInfo->m_achUsername) + 1);
    OspPost(pMsg->srcid, EV_ACK_TRANSINFO, buffer,
        sizeof(CTransInfoBuffer), pMsg->srcnode, pMsg->dstid);
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1024, NULL, 0, 0);
}

/*
    终止状态下对各事件处理的选择
*/
void CClientInstance::TermFunction(CMessage *const pMsg)
{
    printE(pMsg->event);
    printf("输入'q'终结此次业务\n");
    while (getchar() != 'q')
    {
    }
    printf("此次业务终结\n");
    printf("进入空闲状态\n");
    NextState(C_STATE_IDLE);
    OspPost(MAKEIID(GetAppID(),GetInsID()), EV_ACK_INSCONNECT,
        "local", 6, 0, MAKEIID(GetAppID(),GetInsID()));
}

/* 功能处理函数 */

/* 发送文字消息 */
void CClientInstance::SendChar(CMessage *pMsg)
{
    s8 SENDCHARBUFF[BUFFSIZE];
    printf("请输入文字消息:\n");
    scanf("%s", SENDCHARBUFF);
    OspPost(g_ptConnectInfo->m_pMsg->srcid, EV_ACK_SENDCHAR, SENDCHARBUFF,
        strlen(SENDCHARBUFF), g_ptConnectInfo->m_pMsg->srcnode,
        g_ptConnectInfo->m_pMsg->dstid);
}