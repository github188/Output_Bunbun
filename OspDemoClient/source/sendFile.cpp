#include "sendFile.h"

void CClientInstance :: sendFile(CMessage *const pMsg)
{
    CFileMessage *pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
    s8 pFilePath[MAXFILENAME];
    FILE *fp;
    if(NULL == pMsg->content)
    {
        printf("请输入需要传送的文件的路径:\n");
        scanf("%s", pFilePath);
        strcpy(pFMsg->pFilePath, pFilePath);
        strcpy(pFMsg->pFileName, GetFileName(pFilePath));
        pFMsg->curLocal = 1;
        /*
             获取文件大小
        */
        fp = fopen(pFilePath, "r");
        fseek(fp, 0, SEEK_END);
        pFMsg->fileSize = ftell(fp);
        printf("filesize : %d\n", pFMsg->fileSize);
        fclose(fp);
    }
    else
    {

        pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
        pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
        strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
        strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);
        //pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal + 1;    //当前文件定位
        //printf("curlocal:%I64u filesize:%I64u\n", ((CFileMessage *)pMsg->content)->curLocal, ((CFileMessage *)pMsg->content)->fileSize);
        //printf("curlocal:%I64u filesize:%I64u\n", pFMsg->curLocal, pFMsg->fileSize);
        printf("%.2f%%已传输\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
        pFMsg->curLocal++;
        
    }
    
    fp = fopen(pFMsg->pFilePath, "r");
    fseek(fp, (pFMsg->curLocal-1)*BUFFSIZE, SEEK_SET);
    if(NULL == fp)
    {
        printf("打开文件出错\n");
    }
    else
    {
        pFMsg->curBufSize = pFMsg->fileSize - (pFMsg->curLocal - 1)*BUFFSIZE;
        if(BUFFSIZE*pFMsg->curLocal >= pFMsg->fileSize)
        {
            if(fread(pFMsg->pBuf, pFMsg->curBufSize, 1, fp) <= 1)
            {
                OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), g_pConnectInfo->pMsg->srcnode,\
                    g_pConnectInfo->pMsg->dstid);
            } 
        }
        else
        {
            pFMsg->curBufSize = BUFFSIZE;
            if(fread(pFMsg->pBuf, BUFFSIZE, 1, fp) <= 1)
            {
                OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), g_pConnectInfo->pMsg->srcnode,\
                    g_pConnectInfo->pMsg->dstid);
            }
        }
    
    
    }
    fclose(fp);

}

s8 *GetFileName(s8 pFilePath[])
{
    s32 len = strlen(pFilePath);
    s8 downFileName[MAXFILENAME];
    s8 FileName[MAXFILENAME];
    s32 fileLen = 0;
    for(s32 i = len - 1; i >= 0; i--, fileLen++)
    {
        if(pFilePath[i] == '\\')
        {
            break;
        }
        downFileName[len - i - 1] = pFilePath[i];
    }
    
    for(s32 i = 0; i < fileLen; i++)
    {
        FileName[i] = downFileName[fileLen - i - 1];
    }
    FileName[fileLen] = '\0';
    return FileName;
}