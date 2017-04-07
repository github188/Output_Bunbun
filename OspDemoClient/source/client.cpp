#include "cltApp.h"



void UserInterface()
{
    printf("-------------------------------------\n\
            1.δʵ��\n\
            2.�鿴�������߿ͻ�\n\
            3.ת����Ϣ\n\
            4.Ⱥ����Ϣ\n\
            5.�����ļ�\n\
            *6.�����ַ���Ϣ\n\
            *7.�˳�\n\
            *0.����\n-------------------------------------\n\
Please intput your command:\n");
    s32 byCmd = -1;

    scanf("%d", &byCmd);
    if(7 == byCmd)
    {
        OspQuit();
    }

}

/*
    OSP��ʼ��
*/
CClientApp *clt = new CClientApp;
void UserInit()
{
    
    s32 rtn = -1;
    /*
        API BOOL32 
        OspInit(BOOL32 TelNetEnable=FALSE, 
	    u16 TelNetPort=0, char* pchModuleName = NULL );
        TelNetPort����ʾtelnet�������õĶ˿ڣ�һ�����������ȱʡֵ0������OSP���2500��ʼ�󶨶˿ڣ�ֱ���ɹ�����Ϊtelnet�Ķ˿ڣ�
	    pchModuleName��ģ��������Ӧ�ó�������֣�
    */


    OspInit(false, 0, "client");         /* ��ʼ��Osp, �ڶ˿�x����Telnet */
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
    printf("��������Ҫ���ӵķ�������ַ�Ͷ˿�(��ַ��˿ڼ�ո�ָ�):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*���ӷ���������*/
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

    /*����ͬʱ����Ҫ����������һ��ʵ������ÿͻ���,�Ա����ͨѶʹ��*/
    rtn = OspPost(MAKEIID(SRV_APP_NO, CClientInstance :: PENDING), EVENT_REQ_INS, NULL, 0, dstnode, MAKEIID(CLT_APP_NO, 1));
    if(0 == rtn)
    {
        printf("�������ɹ�!\n");
    }
    else
    {
        printf("�������ʧ��!\n");
    }
    while(1);
    return 0;
}