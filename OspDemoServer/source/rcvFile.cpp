#include "rcvFile.h"

#ifdef _MSC_VER
#include "io.h"
#include "direct.h"
#endif

#ifdef _LINUX_
#include 
#endif
void CServerInstance::rcvFile(CMessage *const pMsg)
{

    if(!fp)     //第一次接收
    {
        if(1 == ((CFileMessage *)pMsg->content)->curLocal)  //第一次正常接收，记录文件信息
        {
            pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
            pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
            strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
            strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);

            m_Rcd.fMsg.fileSize = pFMsg->fileSize;//记录上传文件的客户机
            strcpy(m_Rcd.fMsg.pFileName, ((CFileMessage *)pMsg->content)->pFileName);
            strcpy(m_Rcd.fMsg.pFilePath, pFMsg->pFilePath);

            strcpy(pFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");
            strcpy(pRcdFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");

            strcat(pFilePath, m_pCurUser.pByAlias);
            strcat(pRcdFilePath, m_pCurUser.pByAlias);
            strcpy(m_Rcd.pUsername, m_pCurUser.pByAlias);
        
            m_Rcd.bFinish = false;  //记录状态未完成上传

            createUserFile(pFilePath, pRcdFilePath);
            strcat(pFilePath, "\\");
            strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);
            printf("%s\n", pFilePath);

            

            fp = fopen(pFilePath, "wb");
            fpRcd = fopen(strcat(pRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);
        }
        else                //第一次重连接收
        {
            pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
            pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
            strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
            strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);
       
            strcpy(pFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");
            strcpy(pRcdFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");

            strcat(pFilePath, m_pCurUser.pByAlias);
            strcat(pRcdFilePath, m_pCurUser.pByAlias);
            strcpy(m_Rcd.pUsername, m_pCurUser.pByAlias);
        
            m_Rcd.bFinish = false;  //记录状态未完成上传

            createUserFile(pFilePath, pRcdFilePath);
            strcat(pFilePath, "\\");
            strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);
            printf("%s\n", pFilePath);

            strcpy(m_Rcd.fMsg.pFilePath, pFilePath);//记录上传文件的客户机路径

            fp = fopen(pFilePath, "wb");
            fseek(fp, BUFFSIZE*((CFileMessage *)pMsg->content)->curLocal, SEEK_SET);
            fpRcd = fopen(strcat(pRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);

            
        }
    }
    
    
    pFMsg->curLocal = ((CFileMessage *)pMsg->content)->curLocal;
    pFMsg->curBufSize = ((CFileMessage *)pMsg->content)->curBufSize;

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
            printf("%.2f%%写入成功\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
            OspPost(pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
        }

        m_Rcd.dwAldRcdSeek = pFMsg->curLocal;   //记录成功传送的文件偏移量
    
    }
    
}

void CServerInstance::saveDisConnectInfo()
{
    fpRcd = fopen(pRcdFilePath, "wb");
    if(fwrite(&m_Rcd, sizeof(m_Rcd), 1, fpRcd) <= 1)
    {
        printf("记录断链信息成功\n");
    }
    else
    {
        printf("记录断链信息失败\n");
    }
    fclose(fpRcd);
}

    


void createUserFile(s8 filePath[], s8 pRcdFilePath[])
{

    FILE *tfp = fopen(filePath, "rb");
    if(!tfp)
    {
        mkdir(filePath);
        mkdir(strcat(pRcdFilePath, "\\Rcd"));
    }
    else
    {
        fclose(tfp);
    }

}