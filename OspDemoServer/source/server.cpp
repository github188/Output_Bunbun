#include "srvApp.h"

#include "srvInstance.h"

/*
template <class I, int maxins = 1, class A = CAppNoData, u8 maxAliasLen = 0>
*/
class CServerApp : public zTemplate <CServerInstance, 6, CAppNoData, 0>
{

public:
    CServerApp();


};



/****0123456789012345678901234567890123456789012345678901234567890123456789*|||||012345678901234567890123456789012345|||||*/
#define SRV_APP_PRI         100             /* APP优先级 */
#define SRV_APP_QUE_SIZE    1000            /* 最大等待消息数 */
#define SRV_APP_STK_SIZE    (200 * 1024)    /* APP任务的栈大小200K */

CServerApp *Svr = NULL;

int main()
{

    s32 rtn = -1;
    /* 初始化Osp,默认0的情况下，在2500开始的端口启动Telnet，此端口是telnet服务所用的端口 */
    OspInit(TRUE, 0, "server");     
    OspCreateTcpNode(inet_addr("0.0.0.0"), 6682);	    /* 在6682端口上创建本地监听结点 */
    Svr = new CServerApp;
    rtn = Svr->CreateApp("SvrApp", 
                        SRV_APP_NO, 
                        SRV_APP_PRI, 
                        SRV_APP_QUE_SIZE, 
                        SRV_APP_STK_SIZE);

    printf("serverApp is running\nand SrvAppId is %d\n", Svr->appId);

    /* 服务端阻塞 */
    while (1)
    {
        Sleep(1000);
    }

    delete Svr;

    return 0;
}