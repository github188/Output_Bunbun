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
#define SRV_APP_PRI         100             /* APP���ȼ� */
#define SRV_APP_QUE_SIZE    1000            /* ���ȴ���Ϣ�� */
#define SRV_APP_STK_SIZE    (200 * 1024)    /* APP�����ջ��С200K */

CServerApp *Svr = NULL;

int main()
{

    s32 rtn = -1;
    /* ��ʼ��Osp,Ĭ��0������£���2500��ʼ�Ķ˿�����Telnet���˶˿���telnet�������õĶ˿� */
    OspInit(TRUE, 0, "server");     
    OspCreateTcpNode(inet_addr("0.0.0.0"), 6682);	    /* ��6682�˿��ϴ������ؼ������ */
    Svr = new CServerApp;
    rtn = Svr->CreateApp("SvrApp", 
                        SRV_APP_NO, 
                        SRV_APP_PRI, 
                        SRV_APP_QUE_SIZE, 
                        SRV_APP_STK_SIZE);

    printf("serverApp is running\nand SrvAppId is %d\n", Svr->appId);

    /* ��������� */
    while (1)
    {
        Sleep(1000);
    }

    delete Svr;

    return 0;
}