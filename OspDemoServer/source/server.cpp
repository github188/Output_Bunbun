#include "srvApp.h"


#define SRV_APP_PRI         100         /*APP优先级*/
#define SRV_APP_QUE_SIZE    1000        /*最大等待消息数*/
#define SRV_APP_STK_SIZE    (200<<10)  /*APP任务的栈大小*/

CServerApp *Srv = new CServerApp;
/*
    OSP初始化
*/
void UserInit()
{
    
    s32 rtn = -1;
    OspInit(TRUE, 0, "server");     /* 初始化Osp,默认0的情况下，在2500开始的端口启动Telnet，此端口是telnet服务所用的端口 */
    OspCreateTcpNode(inet_addr("0.0.0.0"), 6682);	    /* 在6682端口上创建本地监听结点 */
    rtn = Srv->CreateApp("SrvApp", 
                        SRV_APP_NO, 
                        SRV_APP_PRI, 
                        SRV_APP_QUE_SIZE, 
                        SRV_APP_STK_SIZE);
}


int main()
{

    UserInit();
    printf("serverApp is running\nand SrvAppId is %d\n", Srv->appId);

    while(1)
    {
        Sleep(1000);
    }



    return 0;
}