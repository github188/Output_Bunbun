#include "cltApp.h"

struct ConnectInfo *g_pConnectInfo;

void UserInterface()
{
    printf("-------------------------------------\n\
            1.保留业务\n\
            2.查看其他用户\n\
            3.转发消息\n\
            4.群发消息\n\
            5.发送文件\n\
            *6.发送文字信息\n\
            *7.退出\n\
            *0.清屏\n-------------------------------------\n\
Please intput your command:\n");
    u16 byCmd;
    scanf("%hd", &byCmd);
    switch(byCmd)
    {
        case U_INVALID:
            break;
        case U_CATOTHERS:       
            OspPost(MAKEIID(CLT_APP_NO, 1), U_CATOTHERS, 0, 0, 0, MAKEIID(CLT_APP_NO, 1), 0);
            break;
        case U_TRANSINFO:
            break;
        case U_TRANSALL:
            break;
        case U_SENDFILE:
            OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDFILE, 0, 0, 0, MAKEIID(CLT_APP_NO, 1), 0);
            break;
        case U_SENDCHAR:
            OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDCHAR, 0, 0, 0, MAKEIID(CLT_APP_NO, 1), 0);
            break;
        case U_DISCONNECT:
            break;
        case U_CLEAR:
            system("cls");
            UserInterface();
            break;
        default:
            printf("不存在的指令\n");
            UserInterface();
            break;
    }


}

/*
    OSP初始化
*/
CClientApp *clt = new CClientApp;
void UserInit()
{
    
    s32 rtn = -1;
    /*
        API BOOL32 
        OspInit(BOOL32 TelNetEnable=FALSE, 
	    u16 TelNetPort=0, char* pchModuleName = NULL );
        TelNetPort，表示telnet服务所用的端口，一般情况下请用缺省值0，这样OSP会从2500开始绑定端口，直到成功，作为telnet的端口；
	    pchModuleName，模块名，该应用程序的名字；
    */
    

    OspInit(false, 0, "client");         /* 初始化Osp, 在端口x启动Telnet */
    OspCreateTcpNode(0, 6683); 
    rtn = clt->CreateApp("CltApp1", 
                        CLT_APP_NO, 
                        CLT_APP_PRI, 
                        CLT_APP_QUE_SIZE, 
                        CLT_APP_STK_SIZE);

    clt->SetInstAlias(1, "Ins1", 5);
}

int main()
{
    s32 rtn = -1;
    g_pConnectInfo = (struct ConnectInfo *)malloc(sizeof(struct ConnectInfo));
    g_pConnectInfo->pMsg = (CMessage *)malloc(sizeof(CMessage));
    UserInit();

    s8 pbyAddr[IPSTR_MAX];
    u16 wTcpPort;
    printf("输入你需要连接的服务器地址和端口(地址与端口间空格分隔):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*连接服务器测试*/
    g_pConnectInfo->dstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort, 10, 3, 1, 0);

    system("cls");
    if( g_pConnectInfo->dstnode != 0)
    {
        printf("connect server success\n");
        rtn = OspPost(MAKEIID(CLT_APP_NO, 1), EVENT_REQ_INSCONNECT, NULL, 0, 0);
        
    }
    else
    {
        printf("connect server fail\n");
        Sleep(1000);
        exit(-1);
    } 
    

    
    while(1);
    return 0;
}