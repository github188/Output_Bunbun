#include "readFile.h"

void CServerInstance :: ReadFile(CMessage *const pMsg)
{
   
    CFileMessage *pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
    pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
    pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
    pFMsg->curBufSize = ((CFileMessage *)pMsg->content)->curBufSize;
    strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
    strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);

    s8 pFilePath[MAXFILENAME] = "C:\\Users\\Reisen\\Desktop\\test\\";
    strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);

    //printf("filename : %s\n", ((CFileMessage*)pMsg->content)->pFileName);
    //printf("filepath : %s\n", pFilePath);
    FILE *fp;
    fp = fopen(pFilePath, "a+");
    if(NULL == fp)
    {
        printf("新建文件出错\n");
    }
    if(fwrite(((CFileMessage*)pMsg->content)->pBuf, pFMsg->curBufSize, 1, fp) <= 1)
    {

        if(BUFFSIZE*pFMsg->curLocal >= pFMsg->fileSize)
        {
            printf("100%%写入成功\n");
            OspPost(pMsg->srcid, EVENT_TERM_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_TERM_SENDFILE, NULL, 0, 0);
        }
        else
        {
            printf("%.2f%%写入成功\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
            OspPost(pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
        }
        
    
    }
    fclose(fp);



}