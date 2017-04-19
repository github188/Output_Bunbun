#include "cltInstance.h"

void printE(u16 wEvent)
{
    printf("该事件是%hd\n", wEvent);
}

void itoa (s32 n, s8 ans[12])
{
    s32 i, j, k;
    s8 s[12];
    if(n < 0)
    {
        n = -n;
    }
    i = 0;
    do{
        s[i++] = n%10 + '0';
    }while((n /= 10) > 0);
    i--;
    for(k = 0; i >= 0; i--, k++)
    {
        ans[k] = s[i];
    }
    ans[k] = '\0';
} 

CClientInstance::CClientInstance()
{
    NextState(C_STATE_IDLE);
    IdleEventFunction[GetMain(EVENT_REQ_INSCONNECT)][GetBran(EVENT_REQ_INSCONNECT)] = &CClientInstance::Idle_Req_InsConnect;
    IdleEventFunction[GetMain(EVENT_ACK_INSCONNECT)][GetBran(EVENT_ACK_INSCONNECT)] = &CClientInstance::Idle_Ack_InsConnect;

    ConnectEventFunction[GetMain(EVENT_REQ_TRANSINFO)][GetBran(EVENT_REQ_TRANSINFO)] = &CClientInstance::Connect_Req_TransInfo;

    ReqEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance::Req_Ack_CatOthers;
    ReqEventFunction[GetMain(EVENT_ACK_TRANSINFO)][GetBran(EVENT_ACK_TRANSINFO)] = &CClientInstance::Req_Ack_TransInfo;
    ReqEventFunction[GetMain(EVENT_ACK_SENDFILE)][GetBran(EVENT_ACK_SENDFILE)] = &CClientInstance::Req_Ack_SendFile;
    ReqEventFunction[GetMain(EVENT_ACK_SENDCHAR)][GetBran(EVENT_ACK_SENDCHAR)] = &CClientInstance::Req_Ack_SendChar;
    ReqEventFunction[GetMain(EVENT_ACK_DISCONNECT)][GetBran(EVENT_ACK_DISCONNECT)] = &CClientInstance::Req_Ack_DisConnect;

    WorkEventFunction[GetMain(EVENT_ACK_CATOTHERS)][GetBran(EVENT_ACK_CATOTHERS)] = &CClientInstance::Work_Ack_CatOthers;
    WorkEventFunction[GetMain(EVENT_TERM_CATOTHERS)][GetBran(EVENT_TERM_CATOTHERS)] = &CClientInstance::Work_Term_CatOthers;

    WorkEventFunction[GetMain(EVENT_ACK_TRANSINFO)][GetBran(EVENT_ACK_TRANSINFO)] = &CClientInstance::Work_Ack_TransInfo;
    WorkEventFunction[GetMain(EVENT_TERM_TRANSINFO)][GetBran(EVENT_TERM_TRANSINFO)] = &CClientInstance::Work_Term_TransInfo;

    WorkEventFunction[GetMain(EVENT_ACK_SENDFILE)][GetBran(EVENT_ACK_SENDFILE)] = &CClientInstance::Work_Ack_SendFile;
    WorkEventFunction[GetMain(EVENT_TERM_SENDFILE)][GetBran(EVENT_TERM_SENDFILE)] = &CClientInstance::Work_Term_SendFile;

    WorkEventFunction[GetMain(EVENT_ACK_SENDCHAR)][GetBran(EVENT_ACK_SENDCHAR)] = &CClientInstance::Work_Ack_SendChar;
    WorkEventFunction[GetMain(EVENT_TERM_SENDCHAR)][GetBran(EVENT_TERM_SENDCHAR)] = &CClientInstance::Work_Term_SendChar;

    
}

void CClientInstance::DaemonInstanceEntry(CMessage *const  pMsg, CApp * pApp)
{
    log( 0, (char *)"Osp: message received in default daemon instance of app %d.\nCheck your daemon implementation, please!\n", GetAppID());

}

/*
    对事件的解析 主、分支函数
*/
u16 CClientInstance::GetMain(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) % EVENT_T;
}

u16 CClientInstance::GetBran(u16 wEvent)
{
    return (wEvent - OSP_USEREVENT_BASE) / EVENT_T;
}


void CClientInstance::InstanceEntry(CMessage *const pMsg)
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
    Idle状态对各事件处理的选择
*/
void CClientInstance::Idle_Function(CMessage *const pMsg)
{
    printE(pMsg->event);
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*IdleEventFunction[wCurEventMain][wCurEventBran])(pMsg);

}

void CClientInstance::Idle_Req_InsConnect(CMessage *const pMsg)
{

    /*向服务器Daemon请求分配实例*/
    s32 rtn = -1;
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance::DAEMON), EVENT_REQ_INSCONNECT, ((CUerInfo *)pMsg->content)->username, 
        sizeof(((CUerInfo *)pMsg->content)->username), g_pConnectInfo->dstnode, MAKEIID(CLT_APP_NO, 1));
    if(0 == rtn)
    {
        
        printf("向服务器发送连接实例请求成功\n");
    }
    else
    {
        printf("向服务器发送连接实例请求失败!\n");
    }
    
    

}

void CClientInstance::Idle_Ack_InsConnect(CMessage *const pMsg)
{
   
    if(!(pMsg->content))
    {
        memcpy(g_pConnectInfo->pMsg, pMsg, sizeof(CMessage));
        printf("已连接至服务器实例,");
    }
    printf("进入连接状态\n");
    NextState(C_STATE_CONNECT);
    OspTaskCreate(UserInterface, "userInterface", 100, 200<<10, 0, 0);
    
}

void CClientInstance::Idle_Term_InsConnect(CMessage *const pMsg)
{
    printf("尚未连接服务器实例，无法终止\n");
}

void CClientInstance::Idle_Timeout_InsConnect(CMessage *const pMsg)
{
    printf("连接服务器实例超时\n");
}

/*
    连接状态下对各事件处理的选择
*/
void CClientInstance::Connect_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    if(wCurEvent < 10)
    {
        printf("该消息是用户自定义的消息%hd\n", wCurEvent);
        
        OspPost(g_pConnectInfo->pMsg->srcid, (EVENT_REQ + wCurEvent*EVENT_T), NULL, 0, g_pConnectInfo->pMsg->srcnode,
            MAKEIID(CLT_APP_NO, 1));
        printf("进入请求状态\n");
        NextState(C_STATE_REQ);
    }
    else
    {
        u16 wCurEventMain = GetMain(wCurEvent);
        u16 wCurEventBran = GetBran(wCurEvent);
        (this->*ConnectEventFunction[wCurEventMain][wCurEventBran])(pMsg);
    }
}

void CClientInstance::Connect_Req_TransInfo(CMessage *const pMsg)
{
    printf("有来自%s的新消息 : %s\n", ((CTransInfoBuffer *)pMsg->content)->userName, ((CTransInfoBuffer *)pMsg->content)->content);


}
/*
    请求状态下对各事件处理的选择
*/
void CClientInstance::Req_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*ReqEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CClientInstance::Req_Ack_CatOthers(CMessage *const pMsg)
{
    printf("服务器同意查看用户请求\n");
    printf("进入工作状态\n");
    NextState(C_STATE_WORK);
    
}

void CClientInstance::Req_Ack_TransInfo(CMessage *const pMsg)
{
    printf("服务器正生成用户列表\n");
    printf("进入工作状态\n");
    NextState(C_STATE_WORK);
}

void CClientInstance::Req_Ack_SendFile(CMessage *const pMsg)
{
    printf("服务器同意接收发送文件\n");
    printf("进入工作状态\n");
    NextState(C_STATE_WORK);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_ACK_SENDFILE, NULL, 0, 0, 0);

}

void CClientInstance::Req_Ack_SendChar(CMessage *const pMsg)
{
    printf("服务器同意发送文字消息\n");
    printf("进入工作状态");
    NextState(C_STATE_WORK);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_ACK_SENDCHAR, NULL, 0, 0, 0);

}

void CClientInstance::Req_Ack_DisConnect(CMessage *const pMsg)
{
    printf("你已断开与服务器连接\n");
    printf("进入空闲状态\n");
    NextState(C_STATE_IDLE);

}
/*
    工作状态下对各事件处理的选择
*/
void CClientInstance::Work_Function(CMessage *const pMsg)
{
    u16 wCurEvent = pMsg->event;
    u16 wCurEventMain = GetMain(wCurEvent);
    u16 wCurEventBran = GetBran(wCurEvent);
    (this->*WorkEventFunction[wCurEventMain][wCurEventBran])(pMsg);
}

void CClientInstance::Work_Ack_CatOthers(CMessage *const pMsg)
{
    printf("用户:%s 状态:%s\n", ((UserInfo *)pMsg->content)->pByAlias, ((UserInfo *)pMsg->content)->dwState?"在线":"离线");
}

void CClientInstance::Work_Ack_TransInfo(CMessage *const pMsg)
{

    printf("序号: %d 用户:%s 状态:%s\n", ((UserInfo *)pMsg->content)->dwNumber, ((UserInfo *)pMsg->content)->pByAlias, ((UserInfo *)pMsg->content)->dwState?"在线":"离线");
    

}

void CClientInstance::Work_Ack_SendFile(CMessage *const pMsg)
{
    sendFile(pMsg);
}

void CClientInstance::Work_Ack_SendChar(CMessage *const pMsg)
{
    sendChar(pMsg);
}

void CClientInstance::Work_Term_CatOthers(CMessage *const pMsg)
{
    printf("显示用户信息完毕\n");
    printf("进入终止状态\n");
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1025, NULL, 0, 0);
}

void CClientInstance::Work_Term_TransInfo(CMessage *const pMsg)
{
    CTransInfoBuffer *buffer = new CTransInfoBuffer;
    s8 pInfo[BUFFSIZE];
    printf("请输入你要发送的消息:\n");
    scanf("%s", pInfo);
    printf("请输入你要转发的用户序号:\n");
    s32 dwUserNum;
    //s8 pUserNum[12];
    scanf("%d", &dwUserNum);
    //itoa(dwUserNum, pUserNum);
    memcpy(buffer->content, pInfo, strlen(pInfo) + 1);
    buffer->dwUserNum = dwUserNum;
    buffer->dwCurIns = GETINS(g_pConnectInfo->pMsg->srcid);
    memcpy(buffer->userName, pCltUserInfo->username, strlen(pCltUserInfo->username) + 1);
    OspPost(pMsg->srcid, EVENT_ACK_TRANSINFO, buffer, sizeof(CTransInfoBuffer), pMsg->srcnode, pMsg->dstid);
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1024, NULL, 0, 0);
}

void CClientInstance::Work_Term_SendFile(CMessage *const pMsg)
{
    printf("100%%已传输\n");
    printf("文件上传完毕\n");
    printf("进入终止状态\n");
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
}

void CClientInstance::Work_Term_SendChar(CMessage *const pMsg)
{
    printf("服务器已接收消息\n");
    printf("进入终止状态\n");
    NextState(C_STATE_TERM);
    OspPost(MAKEIID(GetAppID(), GetInsID()), 1, NULL, 0, 0);
}
/*
    终止状态下对各事件处理的选择
*/
void CClientInstance::Term_Function(CMessage *const pMsg)
{
    printE(pMsg->event);
    printf("输入'q'终结此次业务\n");
    while(getchar() != 'q')
    {
    }
    printf("此次业务终结\n");
    printf("进入空闲状态\n");
    NextState(C_STATE_IDLE);
    OspPost(MAKEIID(GetAppID(),GetInsID()), EVENT_ACK_INSCONNECT, "local", 6, 0, MAKEIID(GetAppID(),GetInsID()));
}


