#include "cltInstance.h"

class CClientApp : public zTemplate <CClientInstance, 1, CAppNoData, 0>{};

extern PTConnectInfo g_ptConnectInfo = NULL;            /* 连接信息 */
extern PTClientUserInfo g_ptCltUserInfo = NULL;              /* 客户端信息 */

extern void UserInterface(void)
{
    u16 wCmd;
    printf("-------------------------------------\n\
            *1.保留业务\n\
            *2.查看其他用户B\n\
            *3.转发群发消息\n\
            4.转发群发文件\n\
            *5.发送文件B\n\
            *6.发送文字信息B\n\
            *7.退出\n\
            *0.清屏\n-------------------------------------\n\
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
                printf("不存在的指令\n");
                UserInterface();
                break;
        }

}


int main(int argc, char *argv[])
{
   
    /* Osp初始化 */
    s32 nRtn = -1;
    s32 nNode;
    OspInit(false, 0, "client");
    printf("请输入监听节点号(基准6682,本机测试请选择不一样的端口,避免冲突) : \n");
    scanf("%d", &nNode);
    OspCreateTcpNode(0, nNode); 
    CClientApp *pcClt = new CClientApp();
    nRtn = pcClt->CreateApp("CltApp2", 
                        CLT_APP_NO, 
                        CLT_APP_PRI, 
                        CLT_APP_QUE_SIZE, 
                        CLT_APP_STK_SIZE);

    /* 初始化连接信息 */
    g_ptConnectInfo = (PTConnectInfo)malloc(sizeof(TConnectInfo));
    g_ptConnectInfo->m_pMsg = (CMessage *)malloc(sizeof(CMessage));
    g_ptCltUserInfo = (PTClientUserInfo)malloc(sizeof(TClientUserInfo));
    s8 achAddr[IPSTR_MAX];
    u16 wTcpPort;
    printf("输入你需要连接的服务器地址和端口(地址与端口间空格分隔):\n");
    scanf("%s %d", achAddr, &wTcpPort);
    printf("请输入你的用户名: \n");
    scanf("%s", g_ptCltUserInfo->m_achUsername);
    getchar();
    u32 dwIpv4Addr = inet_addr(achAddr);

    /* 连接服务器测试 */
    g_ptConnectInfo->m_nDstnode = OspConnectTcpNode(dwIpv4Addr, wTcpPort,
        10, 3, 1, 0);      
    system("cls");

    /* 判断是否连接成功 */
    if (g_ptConnectInfo->m_nDstnode != 0)
    {
        printf("connect server success\n");
        /*在该节点设置断链检测, 2s一次， 2次失败就断链*/
        printf("设置断链检测%s\n", (OspSetHBParam(g_ptConnectInfo->m_nDstnode,
            2, 2)?"成功":"失败"));
        /*设置断链通知的对象*/
        printf("设置断链回调%s\n", 
            (OSP_OK == OspNodeDiscCBReg(g_ptConnectInfo->m_nDstnode, CLT_APP_NO, 1)
            ?"成功":"失败"));
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