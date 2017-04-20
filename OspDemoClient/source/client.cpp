#include "cltApp.h"

struct ConnectInfo *g_pConnectInfo;
CUserInfo *pCltUserInfo;
void UserInterface()
{
    u16 byCmd;
    printf("-------------------------------------\n\
            *1.保留业务\n\
            *2.查看其他用户B\n\
            *3.转发群发消息\n\
            4.转发群发文件\n\
            *5.发送文件B\n\
            *6.发送文字信息B\n\
            *7.退出\n\
            *0.清屏\n-------------------------------------\n\
Please intput your command:\n");

        scanf("%hd", &byCmd);
        switch(byCmd)
        {
            case U_INVALID:
                break;
            case U_CATOTHERS:       
                OspPost(MAKEIID(CLT_APP_NO, 1), U_CATOTHERS, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_TRANSINFO:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_TRANSINFO, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_TRANSFILE:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_TRANSFILE, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_SENDFILE:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDFILE, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_SENDCHAR:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDCHAR, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_DISCONNECT:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_DISCONNECT, "user", 5, 0, MAKEIID(CLT_APP_NO, 1), 0);
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
    
    s32 dwNode;
    OspInit(false, 0, "client");         /* 初始化Osp, 在端口x启动Telnet */
    printf("请输入监听节点号(基准6682,本机测试请选择不一样的端口,避免冲突) : \n");
    scanf("%d", &dwNode);
    OspCreateTcpNode(0, dwNode); 
    rtn = clt->CreateApp("CltApp2", 
                        CLT_APP_NO, 
                        CLT_APP_PRI, 
                        CLT_APP_QUE_SIZE, 
                        CLT_APP_STK_SIZE);

    clt->SetInstAlias(1, "Ins2", 5);
}

int main()
{
    //printf("ok = %d dis = %d\n", OSP_OK, OSP_DISCONNECT);
    s32 rtn = -1;
    g_pConnectInfo = (struct ConnectInfo *)malloc(sizeof(struct ConnectInfo));
    g_pConnectInfo->pMsg = (CMessage *)malloc(sizeof(CMessage));

    pCltUserInfo = (CUerInfo *)malloc(sizeof(CUerInfo));
    UserInit();

    s8 pbyAddr[IPSTR_MAX];
    u16 wTcpPort;
    printf("输入你需要连接的服务器地址和端口(地址与端口间空格分隔):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    printf("请输入你的用户名: \n");
    scanf("%s", pCltUserInfo->username);
    getchar();
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*连接服务器测试*/
    g_pConnectInfo->dstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort, 10, 3, 1, 0);

    /*在该节点设置断链检测, 2s一次， 2次失败就断链*/
    printf("设置断链检测%s\n", OspSetHBParam(g_pConnectInfo->dstnode, 2, 2)?"成功":"失败");
    /*设置断链通知的对象*/
    printf("设置断链回调%s\n", OSP_OK == OspNodeDiscCBReg(g_pConnectInfo->dstnode, CLT_APP_NO, 1)?"成功":"失败");
   
    

    system("cls");
    if(g_pConnectInfo->dstnode != 0)
    {
        printf("connect server success\n");
        rtn = OspPost(MAKEIID(CLT_APP_NO, 1), EVENT_REQ_INSCONNECT, pCltUserInfo, sizeof(CUerInfo), 0);
        
    }
    else
    {
        printf("connect server fail\n");
        Sleep(1000);
        exit(-1);
    } 

    while(1)
    {
        Sleep(1000);
    }
    return 0;
}