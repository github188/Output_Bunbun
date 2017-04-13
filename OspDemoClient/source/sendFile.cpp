#include "sendFile.h"

void CClientInstance :: sendFile(CMessage *const pMsg)
{
    CFileMessage *pFMsg;
    pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
    s8 pFilePath[MAXFILENAME];
    if(NULL == pMsg->content)
    {
        printf("请输入需要传送的文件的路径:\n");
        scanf("%s", pFilePath);
        strcpy(pFMsg->pFileName, GetFileName(pFilePath));
        pFMsg->curLocal = 1;
    }
    else
    {

        pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
        pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
        strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);
        //pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal + 1;    //当前文件定位
        printf("curlocal:%lld filesize:%lld\n", ((CFileMessage *)pMsg->content)->curLocal, ((CFileMessage *)pMsg->content)->fileSize);
        printf("curlocal:%lld filesize:%lld\n", pFMsg->curLocal, pFMsg->fileSize);
        printf("%.2f%%已传输\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
        pFMsg->curLocal++;
        
    }
    FILE *fp;
    fp = fopen(pFilePath, "r");
    fseek(fp, 0, SEEK_END);
    pFMsg->fileSize = ftell(fp);
    printf("filesize : %d\n", pFMsg->fileSize);
    fclose(fp);
    fp = fopen(pFilePath, "r");
    if(NULL == fp)
    {
        printf("打开文件出错\n");
    }
    else
    {

        printf("打开文件成功\n");
        if(fread(pFMsg->pBuf, BUFFSIZE, 1, fp) <= 1)
        {
            int rtn = -1;
            if(OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), g_pConnectInfo->pMsg->srcnode,\
                g_pConnectInfo->pMsg->dstid))printf("fail\n");else printf("success\n");
            //printf();
        
        }
    
    
    }

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