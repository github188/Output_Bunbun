#include "transInfo.h"

void CServerInstance::transInfo(CMessage *const pMsg)
{

    for(s32 i = 0; i < m_dwCurInsNum; i++)
    {
        OspPost(pMsg->srcid, EVENT_ACK_TRANSINFO, &m_pUserInfo[i], sizeof(m_pUserInfo[i]), pMsg->srcnode);
    }
    OspPost(pMsg->srcid, EVENT_TERM_TRANSINFO, NULL, 0, pMsg->srcnode, MAKEIID(GetAppID(), GetInsID()));


}