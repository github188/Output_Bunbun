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

    if(!fp)     //��һ�ν���
    {
        if(1 == ((CFileMessage *)pMsg->content)->curLocal)  //��һ���������գ���¼�ļ���Ϣ
        {
            pFMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
            pFMsg->fileSize = ((CFileMessage *)pMsg->content)->fileSize;
            strcpy(pFMsg->pFilePath, ((CFileMessage *)pMsg->content)->pFilePath);
            strcpy(pFMsg->pFileName, ((CFileMessage *)pMsg->content)->pFileName);

            m_Rcd.fMsg.fileSize = pFMsg->fileSize;//��¼�ϴ��ļ��Ŀͻ���
            strcpy(m_Rcd.fMsg.pFileName, ((CFileMessage *)pMsg->content)->pFileName);
            strcpy(m_Rcd.fMsg.pFilePath, pFMsg->pFilePath);

            strcpy(pFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");
            strcpy(pRcdFilePath, "C:\\Users\\Reisen\\Desktop\\test\\");

            strcat(pFilePath, m_pCurUser.pByAlias);
            strcat(pRcdFilePath, m_pCurUser.pByAlias);
            strcpy(m_Rcd.pUsername, m_pCurUser.pByAlias);
        
            m_Rcd.bFinish = false;  //��¼״̬δ����ϴ�

            createUserFile(pFilePath, pRcdFilePath);
            strcat(pFilePath, "\\");
            strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);
            printf("%s\n", pFilePath);

            

            fp = fopen(pFilePath, "wb");
            fpRcd = fopen(strcat(pRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);
        }
        else                //��һ����������
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
        
            m_Rcd.bFinish = false;  //��¼״̬δ����ϴ�

            createUserFile(pFilePath, pRcdFilePath);
            strcat(pFilePath, "\\");
            strcat(pFilePath, ((CFileMessage*)pMsg->content)->pFileName);
            printf("%s\n", pFilePath);

            strcpy(m_Rcd.fMsg.pFilePath, pFilePath);//��¼�ϴ��ļ��Ŀͻ���·��

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
        printf("�½��ļ�����\n");
    }
    if(fwrite(((CFileMessage*)pMsg->content)->pBuf, pFMsg->curBufSize, 1, fp) <= 1)
    {

        if(BUFFSIZE*pFMsg->curLocal >= pFMsg->fileSize)
        {
            printf("100%%д��ɹ�\n");
            fclose(fp);
            OspPost(pMsg->srcid, EVENT_TERM_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
            OspPost(MAKEIID(GetAppID(), GetInsID()), EVENT_TERM_SENDFILE, NULL, 0, 0);
        }
        else
        {
            printf("%.2f%%д��ɹ�\n", ((float)pFMsg->curLocal)/((float)pFMsg->fileSize/BUFFSIZE)*100);
            OspPost(pMsg->srcid, EVENT_ACK_SENDFILE, pFMsg, sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
        }

        m_Rcd.dwAldRcdSeek = pFMsg->curLocal;   //��¼�ɹ����͵��ļ�ƫ����
    
    }
    
}

void CServerInstance::saveDisConnectInfo()
{
    fpRcd = fopen(pRcdFilePath, "wb");
    if(fwrite(&m_Rcd, sizeof(m_Rcd), 1, fpRcd) <= 1)
    {
        printf("��¼������Ϣ�ɹ�\n");
    }
    else
    {
        printf("��¼������Ϣʧ��\n");
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