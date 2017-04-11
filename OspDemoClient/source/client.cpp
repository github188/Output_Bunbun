#include "cltApp.h"

struct ConnectInfo *g_pConnectInfo;

void UserInterface()
{
    printf("-------------------------------------\n\
            1.δʵ��\n\
            2.�鿴�����û�\n\
            3.ת����Ϣ\n\
            4.Ⱥ����Ϣ\n\
            5.�����ļ�\n\
            *6.����������Ϣ\n\
            *7.�˳�\n\
            *0.����\n-------------------------------------\n\
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
            break;
        case U_SENDCHAR:
            break;
        case U_DISCONNECT:
            break;
        case U_CLEAR:
            break;
        default:
            break;
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
    printf("��������Ҫ���ӵķ�������ַ�Ͷ˿�(��ַ��˿ڼ�ո�ָ�):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*���ӷ���������*/
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