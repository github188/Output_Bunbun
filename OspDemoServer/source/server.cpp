#include "srvApp.h"


#define SRV_APP_PRI         100         /*APP���ȼ�*/
#define SRV_APP_QUE_SIZE    1000        /*���ȴ���Ϣ��*/
#define SRV_APP_STK_SIZE    (200<<10)  /*APP�����ջ��С*/

CServerApp *Srv = new CServerApp;
/*
    OSP��ʼ��
*/
void UserInit()
{
    
    s32 rtn = -1;
    OspInit(TRUE, 0, "server");     /* ��ʼ��Osp,Ĭ��0������£���2500��ʼ�Ķ˿�����Telnet���˶˿���telnet�������õĶ˿� */
    OspCreateTcpNode(inet_addr("0.0.0.0"), 6682);	    /* ��6682�˿��ϴ������ؼ������ */
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