#include "srvInstance.h"

void CServerInstance::catOthers(CMessage *const pMsg)
{

    for(s32 i = 0; i < m_dwCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EVENT_ACK_CATOTHERS, m_pUserInfo[i].pByAlias, sizeof(m_pUserInfo[i].pByAlias), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EVENT_TERM_CATOTHERS, NULL, 0, pMsg->srcnode);
    
}