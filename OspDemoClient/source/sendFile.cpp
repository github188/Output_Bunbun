#include "sendFile.h"
#include "md5.h"
void CClientInstance::sendFile(CMessage *const pMsg)
{
    
    s8 pFilePath[MAXFILENAME];
    if(!fp)                    //第一次上传交互，初始文件信息
    {
        pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
        if(!(pMsg->content))        //第一次正常上传
        {
            printf("请输入需要传送的文件的路径:\n");
            scanf("%s", pFilePath);
            strcpy(pFMsg->pFilePath, pFilePath);
            strcpy(pFMsg->pFileName, GetFileName(pFilePath));
            pFMsg->curLocal = 1;
            fp = fopen(pFilePath, "rb");
            if(!fp)
            {
                printf("打开文件出错\n");
            }
            else
            {
           /*获取文件大小*/
                fseek(fp, 0, SEEK_END);
                pFMsg->fileSize = ftell(fp);
                printf("filesize : %d\n", pFMsg->fileSize);
                fseek(fp, 0, SEEK_SET);
            }
        }
        else                //第一次续传
        {
            printf("第一次续传\n");
            pFMsg->curLocal = ((CRcdInfo *)pMsg->content)->dwAldRcdSeek;
            strcpy(pFMsg->pFilePath, ((CRcdInfo *)pMsg->content)->fMsg.pFilePath);
            strcpy(pFMsg->pFileName, ((CRcdInfo *)pMsg->content)->fMsg.pFileName);
            pFMsg->fileSize = ((CRcdInfo *)pMsg->content)->fMsg.fileSize;
            printf("filesize = %d\n", pFMsg->fileSize);
            fp = fopen(pFMsg->pFilePath, "rb");
            fseek(fp, BUFFSIZE*pFMsg->curLocal, SEEK_SET);
            pFMsg->curLocal++;

        }
 
    }
    else    //打印上传文件的进度
    {

        pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
        pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
        strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
        strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);
        printf("%.2f%%已传输\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
        pFMsg->curLocal++;
        
    }
    
    if(fp)  //文件打开成功
    {
        
        pFMsg->curBufSize = pFMsg->fileSize - (pFMsg->curLocal - 1)*BUFFSIZE;   //记录本次上传包的大小
        if(BUFFSIZE*pFMsg->curLocal >= pFMsg->fileSize)         //最后一次上传
        {
            if(fread(pFMsg->pBuf, pFMsg->curBufSize, 1, fp) <= 1)
            {
                
                OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), g_pConnectInfo->pMsg->srcnode,\
                    MAKEIID(GetAppID(), GetInsID()));
                fclose(fp);
                
            } 
        }
        else
        {
            pFMsg->curBufSize = BUFFSIZE;
            if(fread(pFMsg->pBuf, BUFFSIZE, 1, fp) <= 1)
            {
              
                OspPost(g_pConnectInfo->pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), g_pConnectInfo->pMsg->srcnode,\
                    MAKEIID(GetAppID(), GetInsID()));
            }
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