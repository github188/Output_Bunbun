#include "srvInstance.h"

/*
template <class I, int maxins = 1, class A = CAppNoData, u8 maxAliasLen = 0>
*/
class CServerApp : public zTemplate <CServerInstance, 6, CAppNoData, 0>{};


int main()
{

    s32 rtn = -1;
    /* 
    ��ʼ��Osp,Ĭ��0������£���2500��ʼ�Ķ˿�����Telnet��
    �˶˿���telnet�������õĶ˿� 
    */
    OspInit(TRUE, 0, "server");    
    /* ��6682�˿��ϴ������ؼ������ */
    OspCreateTcpNode(inet_addr("0.0.0.0"), 6682);	    
    CServerApp *Svr = new CServerApp;
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