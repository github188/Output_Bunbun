#include "cltApp.h"

struct ConnectInfo *g_pConnectInfo;
CUserInfo *pCltUserInfo;
void UserInterface()
{
    u16 byCmd;
    printf("-------------------------------------\n\
            *1.����ҵ��\n\
            *2.�鿴�����û�B\n\
            *3.ת��Ⱥ����Ϣ\n\
            4.ת��Ⱥ���ļ�\n\
            *5.�����ļ�B\n\
            *6.����������ϢB\n\
            *7.�˳�\n\
            *0.����\n-------------------------------------\n\
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
                printf("�����ڵ�ָ��\n");
                UserInterface();
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
    
    s32 dwNode;
    OspInit(false, 0, "client");         /* ��ʼ��Osp, �ڶ˿�x����Telnet */
    printf("����������ڵ��(��׼6682,����������ѡ��һ���Ķ˿�,�����ͻ) : \n");
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
    printf("��������Ҫ���ӵķ�������ַ�Ͷ˿�(��ַ��˿ڼ�ո�ָ�):\n");
    scanf("%s %d", pbyAddr, &wTcpPort);
    printf("����������û���: \n");
    scanf("%s", pCltUserInfo->username);
    getchar();
    u32 dwIpv4Addr = inet_addr(pbyAddr);
    /*���ӷ���������*/
    g_pConnectInfo->dstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort, 10, 3, 1, 0);

    /*�ڸýڵ����ö������, 2sһ�Σ� 2��ʧ�ܾͶ���*/
    printf("���ö������%s\n", OspSetHBParam(g_pConnectInfo->dstnode, 2, 2)?"�ɹ�":"ʧ��");
    /*���ö���֪ͨ�Ķ���*/
    printf("���ö����ص�%s\n", OSP_OK == OspNodeDiscCBReg(g_pConnectInfo->dstnode, CLT_APP_NO, 1)?"�ɹ�":"ʧ��");
   
    

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