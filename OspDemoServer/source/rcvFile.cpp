#include "rcvFile.h"

void CServerInstance::rcvFile(CMessage *const pMsg)
{
   
    CFileMessage *pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
    pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
    pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
    pFMsg->curBufSize = ((CFileMessage *)pMsg->content)->curBufSize;
    strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
    strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);

    s8 pFilePath[MAXFILENAME] = "C:\\Users\\Reisen\\Desktop\\test\\";
    strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);
    if(1 == pFMsg->curLocal)
    {
        fp = fopen(pFilePath, "wb");
    }
    if(!fp)
    {
        printf("新建文件出错\n");
    }
    if(fwrite(((CFileMessage*)pMsg->content)->pBuf, pFMsg->curBufSize, 1, fp) <= 1)
    {

        if(BUFFSIZE*pFMsg->curLocal >= pFMsg->fileSize)
        {
            printf("100%%写入成功\n");
            fclose(fp);
            OspPost(pMsg->srcid, EVENT_TERM_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_TERM_SENDFILE, NULL, 0, 0);
        }
        else
        {
            //printf("%s\n", ((CFileMessage*)pMsg->content)->pBuf);
            printf("%.2f%%写入成功\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
            OspPost(pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
        }
        
    
    }
    



}