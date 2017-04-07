#include "cltApp.h"



void UserInterface()
{
    printf("-------------------------------------\n\
            1.未实现\n\
            2.查看其他在线客户\n\
            3.转发消息\n\
            4.群发消息\n\
            5.发送文件\n\
            *6.输入字符信息\n\
            *7.退出\n\
            *0.清屏\n-------------------------------------\n\
Please intput your command:\n");
    s32 byCmd = -1;

    scanf("%d", &byCmd);
    if(7 == byCmd)
    {
        OspQuit();
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
    rtn = clt->CreateApp("CltApp", 
                        CLT_APP_NO, 
                        CLT_APP_PRI, 
                        CLT_APP_QUE_SIZE, 
                        CLT_APP_STK_SIZE);
}

int main()
{

    s32 rtn = -1;

    UserInit();

    s8 pbyAddr[IPSTR_MAX];
    u16 wTcpPort;
    printf("输入你需要连接的服务器地址和端口(地址与端口间空格分隔):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*连接服务器测试*/
    s32 dstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort, 10, 3, 1, 0);

    system("cls");
    if(dstnode != 0)
    {
        printf("connect server success\n");
    }
    else
    {
        printf("connect server fail\n");
        Sleep(1000);
        exit(-1);
    } 

    /*连接同时，需要服务器分配一个实例处理该客户端,以便后面通讯使用*/
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: PENDING), EVENT_REQ_INS, NULL, 0, dstnode, MAKEIID(CLT_APP_NO, 1));
    if(0 == rtn)
    {
        printf("请求分配成功!\n");
    }
    else
    {
        printf("请求分配失败!\n");
    }
    while(1);
    return 0;
}