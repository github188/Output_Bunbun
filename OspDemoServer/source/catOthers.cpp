#include "srvInstance.h"

void CServerInstance :: CatOthers(CMessage *const pMsg)
{

    for(s32 i = 0; i < m_dwCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EVENT_ACK_CATOTHERS, m_pUserInfo[i].pMsg, sizeof(m_pUserInfo[i].pMsg), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EVENT_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
}