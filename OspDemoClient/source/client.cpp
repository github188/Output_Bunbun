#include "cltInstance.h"

class CClientApp : public zTemplate <CClientInstance, 1, CAppNoData, 0>{};

extern PTConnectInfo g_ptConnectInfo = NULL;            /* ������Ϣ */
extern PTClientUserInfo g_ptCltUserInfo = NULL;              /* �ͻ�����Ϣ */

extern void UserInterface(void)
{
    u16 wCmd;
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

        scanf("%hd", &wCmd);
        switch (wCmd)
        {
            case U_INVALID:
                break;
            case U_CATOTHERS:       
                OspPost(MAKEIID(CLT_APP_NO, 1), U_CATOTHERS, "user",
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_TRANSINFO:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_TRANSINFO, "user", 
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_TRANSFILE:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_TRANSFILE, "user",
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_SENDFILE:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDFILE, "user",
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_SENDCHAR:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_SENDCHAR, "user",
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
                break;
            case U_DISCONNECT:
                OspPost(MAKEIID(CLT_APP_NO, 1), U_DISCONNECT, "user",
                    5, 0, MAKEIID(CLT_APP_NO, 1), 0);
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


int main(int argc, char *argv[])
{
   
    /* Osp��ʼ�� */
    s32 nRtn = -1;
    s32 nNode;
    OspInit(false, 0, "client");
    printf("����������ڵ��(��׼6682,����������ѡ��һ���Ķ˿�,�����ͻ) : \n");
    scanf("%d", &nNode);
    OspCreateTcpNode(0, nNode); 
    CClientApp *pcClt = new CClientApp();
    nRtn = pcClt->CreateApp("CltApp2", 
                        CLT_APP_NO, 
                        CLT_APP_PRI, 
                        CLT_APP_QUE_SIZE, 
                        CLT_APP_STK_SIZE);

    /* ��ʼ��������Ϣ */
    g_ptConnectInfo = (PTConnectInfo)malloc(sizeof(TConnectInfo));
    g_ptConnectInfo->m_pMsg = (CMessage *)malloc(sizeof(CMessage));
    g_ptCltUserInfo = (PTClientUserInfo)malloc(sizeof(TClientUserInfo));
    s8 achAddr[IPSTR_MAX];
    u16 wTcpPort;
    printf("��������Ҫ���ӵķ�������ַ�Ͷ˿�(��ַ��˿ڼ�ո�ָ�):\n");
    scanf("%s %d", achAddr, &wTcpPort);
    printf("����������û���: \n");
    scanf("%s", g_ptCltUserInfo->m_achUsername);
    getchar();
    u32 dwIpv4Addr = inet_addr(achAddr);

    /* ���ӷ��������� */
    g_ptConnectInfo->m_nDstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort,
        10, 3, 1, 0);      
    system("cls");

    /* �ж��Ƿ����ӳɹ� */
    if (g_ptConnectInfo->m_nDstnode != 0)
    {
        printf("connect server success\n");
        /*�ڸýڵ����ö������, 2sһ�Σ� 2��ʧ�ܾͶ���*/
        printf("���ö������%s\n", (OspSetHBParam(g_ptConnectInfo->m_nDstnode,
            2, 2)?"�ɹ�":"ʧ��"));
        /*���ö���֪ͨ�Ķ���*/
        printf("���ö����ص�%s\n", 
            (OSP_OK == OspNodeDiscCBReg(g_ptConnectInfo->m_nDstnode, CLT_APP_NO, 1)
            ?"�ɹ�":"ʧ��"));
        nRtn = OspPost(MAKEIID(CLT_APP_NO, 1), EV_REQ_INSCONNECT,
            g_ptCltUserInfo, sizeof(TUserInfo), 0);
    }
    else
    {
        printf("connect server fail\n");
        Sleep(1000);
        exit(-1);
    } 

    while (1)
    {
        Sleep(1000);
    }

    delete pcClt;
    return 0;
}