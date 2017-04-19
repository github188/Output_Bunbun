#include "sendChar.h"

void CClientInstance::sendChar(CMessage *pMsg)
{

    printf("请输入文字消息:\n");
    scanf("%s", SENDCHARBUFF);
    OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDCHAR, SENDCHARBUFF, strlen(SENDCHARBUFF), g_pConnectInfo->pMsg->srcnode,
        g_pConnectInfo->pMsg->dstid);
}