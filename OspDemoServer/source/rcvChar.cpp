#include "rcvChar.h"


void CServerInstance::rcvChar(CMessage *pMsg)
{
    printf("%s���͵���Ϣ : %s\n", m_pCurUser.pByAlias, pMsg->content);
    OspPost(pMsg->srcid, EVENT_TERM_SENDCHAR, NULL, 0, pMsg->srcnode, pMsg->dstid);
    OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_TERM_SENDCHAR, NULL, 0, 0);
}