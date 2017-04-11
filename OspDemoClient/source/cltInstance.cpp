#include "cltInstance.h"


CClientInstance :: CClientInstance()
{
    NextState(C_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CClientInstance :: Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CClientInstance :: Idle_Ack_InsConnect;

    ReqEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance :: Req_Ack_CatOthers;

    WorkEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance :: Work_Ack_CatOthers;
    WorkEventFunction[GetMain(EVENT_TERM_CATOTHERS)][GetBran(EVENT_TERM_CATOTHERS)] = &CClientInstance :: Work_Term_CatOthers;

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
            printf("进入请求状态\n");
            NextState(C_STATE_REQ);
            //SetTimer(EVENT_TIMEOUT, 1000);
            break;
        case C_STATE_REQ:
            Req_Function(pMsg);
            printf("进入工作状态\n");
            NextState(C_STATE_WORK);
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
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}

void CClientInstance :: Idle_Req_InsConnect(CMessage *const pMsg)
{

    /*连接同时，需要服务器分配一个实例处理该客户端,以便后面通讯使用*/
    s32 rtn = -1;
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: DAEMON), EVENT_REQ_INSCONNECT, NULL, 0, g_pConnectInfo->dstnode, MAKEIID(CLT_APP_NO, 1));
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: PENDING), EVENT_REQ_INSCONNECT, NULL, 0, g_pConnectInfo->dstnode, MAKEIID(CLT_APP_NO, 1));
    if(0 == rtn)
    {
        
        printf("服务器分配实例成功!\n");
    }
    else
    {
        printf("服务器分配实例失败!\n");
    }
    
    

}

void CClientInstance :: Idle_Ack_InsConnect(CMessage *const pMsg)
{
   
    if(pMsg->event == EVENT_ACK_INSCONNECT)
    {
        memcpy(g_pConnectInfo->pMsg, pMsg, sizeof(CMessage));
        printf("已连接至服务器实例，进入连接状态\n");
        NextState(C_STATE_CONNECT);
        UserInterface();
    }
    
    
}

void CClientInstance :: Idle_Term_InsConnect(CMessage *const pMsg)
{
    printf("尚未连接服务器实例，无法终止\n");
}

void CClientInstance :: Idle_Timeout_InsConnect(CMessage *const pMsg)
{
    printf("连接服务器实例超时\n");
}

/*
    连接状态下对各事件处理的选择
*/
void CClientInstance :: Connect_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if(wCurEvent < 10)
    {
        printf("该消息是用户自定义的消息%hd\n", wCurEvent);
        
        OspPost(g_pConnectInfo->pMsg->srcid, EVENT_REQ_CATOTHERS, 0, 0, g_pConnectInfo->pMsg->srcnode, MAKEIID(CLT_APP_NO, 1));

    }
    else
    {
        
        u16 wCurEventMain = GetMain(wCurEvent);
        u16 wCurEventBran = GetBran(wCurEvent);
        (this->*ConnectEventFunction[wCurEventMain][wCurEventBran])(pMsg);
    }
}


/*
    请求状态下对各事件处理的选择
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
    printf("服务器同意查看用户请求\n");
    
}

/*
    工作状态下对各事件处理的选择
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
    printf("用户:%s   状态:在线\n", pMsg->dstAlias);
}

void CClientInstance :: Work_Term_CatOthers(CMessage *const pMsg)
{
    printf("显示用户信息完毕\n");

}
/*
    终止状态下对各事件处理的选择
*/
void CClientInstance :: Term_Function(CMessage *const pMsg)
{

}