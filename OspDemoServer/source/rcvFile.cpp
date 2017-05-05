#include "srvInstance.h"
#include "io.h"
#include "direct.h"

/* 本地文件指针 */
static FILE *fp = NULL;

/* 记录上传进度文件指针 */
static FILE *fpRcd = NULL;

/* 本地文件路径 */
static s8 s_achFilePath[MAXFILENAME];

/* 进度文件路径 */
static s8 s_achRcdFilePath[MAXFILENAME];

/* 记录文件信息 */
static PTCFileMessage s_tfMsg = NULL;

/* 创建本地文件与进度保存文件 */
static void CreateUserFile(s8 [], s8 []);

void CServerInstance::RcvFile(CMessage *const pMsg)
{

    if (!fp)     /* 第一次接收 */
    {
        /* 第一次正常接收，记录文件信息 */
        if (1 == ((PTCFileMessage)pMsg->content)->m_curLocal)  
        {
            s_tfMsg = (PTCFileMessage)malloc(sizeof(CFileMessage));
            s_tfMsg->m_fileSize = ((PTCFileMessage)pMsg->content)->m_fileSize;
            strcpy(s_tfMsg->m_achFilePath,
                ((PTCFileMessage)pMsg->content)->m_achFilePath);
            strcpy(s_tfMsg->m_achFileName,
                ((PTCFileMessage)pMsg->content)->m_achFileName);

            /* 记录上传文件的客户端信息 */
            m_tRcd.m_tfMsg.m_fileSize = s_tfMsg->m_fileSize;
            strcpy(m_tRcd.m_tfMsg.m_achFileName,
                ((PTCFileMessage)pMsg->content)->m_achFileName);
            strcpy(m_tRcd.m_tfMsg.m_achFilePath, s_tfMsg->m_achFilePath);

            strcpy(s_achFilePath, ".\\test\\");
            strcpy(s_achRcdFilePath, ".\\test\\");

            strcat(s_achFilePath, m_ptCurUser.m_achAlias);
            strcat(s_achRcdFilePath, m_ptCurUser.m_achAlias);
            strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);
        
            m_tRcd.m_bFinish = false;  /* 记录状态未完成上传 */

            CreateUserFile(s_achFilePath, s_achRcdFilePath);
            strcat(s_achFilePath, "\\");
            strcat(s_achFilePath, ((PTCFileMessage)pMsg->content)->m_achFileName);
            printf("%s\n", s_achFilePath);

            fp = fopen(s_achFilePath, "wb");
            fpRcd = fopen(strcat(s_achRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);
        }
        else                /* 第一次重连接收 */
        {
            s_tfMsg = (PTCFileMessage)malloc(sizeof(CFileMessage));
            s_tfMsg->m_fileSize = ((PTCFileMessage)pMsg->content)->m_fileSize;
            strcpy(s_tfMsg->m_achFilePath,
                ((PTCFileMessage)pMsg->content)->m_achFilePath);
            strcpy(s_tfMsg->m_achFileName,
                ((PTCFileMessage)pMsg->content)->m_achFileName);
       
            strcpy(s_achFilePath, ".\\test\\");
            strcpy(s_achRcdFilePath, ".\\test\\");

            strcat(s_achFilePath, m_ptCurUser.m_achAlias);
            strcat(s_achRcdFilePath, m_ptCurUser.m_achAlias);
            strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);
        
            m_tRcd.m_bFinish = false;  /* 记录状态未完成上传 */

            CreateUserFile(s_achFilePath, s_achRcdFilePath);
            strcat(s_achFilePath, "\\");
            strcat(s_achFilePath, ((PTCFileMessage)pMsg->content)->m_achFileName);
            printf("%s\n", s_achFilePath);

            strcpy(m_tRcd.m_tfMsg.m_achFilePath, s_achFilePath);/* 记录上传文件的客户机路径 */

            fp = fopen(s_achFilePath, "wb");
            fseek(fp, BUFFSIZE*((PTCFileMessage)pMsg->content)->m_curLocal, SEEK_SET);
            fpRcd = fopen(strcat(s_achRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);       
        }
    }
      
    s_tfMsg->m_curLocal = ((PTCFileMessage)pMsg->content)->m_curLocal;
    s_tfMsg->m_curBufSize = ((PTCFileMessage)pMsg->content)->m_curBufSize;

    if (!fp)
    {
        printf("新建文件出错\n");
    }
    else
    {
        s32 nRtn = fwrite(((PTCFileMessage)pMsg->content)->m_achBuf, s_tfMsg->m_curBufSize, 1, fp);
        if (nRtn <= 1)
        {
            /* 接收最后一包 */
            if (BUFFSIZE * s_tfMsg->m_curLocal >= s_tfMsg->m_fileSize)
            {
                printf("100%%写入成功\n");
                fclose(fp);
                OspPost(pMsg->srcid, EV_TERM_SENDFILE, s_tfMsg,
                    sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
                OspPost(MAKEIID(GetAppID(), GetInsID()), EV_TERM_SENDFILE,
                    NULL, 0, 0);
            }
            else
            {
                printf("%.2f%%写入成功\n",
                    ((float)s_tfMsg->m_curLocal)/((float)s_tfMsg->m_fileSize/BUFFSIZE)*100);
                OspPost(pMsg->srcid, EV_ACK_SENDFILE, s_tfMsg,
                    sizeof(CFileMessage), pMsg->srcnode, pMsg->dstid);
            }

            m_tRcd.m_nAldRcdSeek = s_tfMsg->m_curLocal;   /* 记录成功传送的文件偏移量 */
        }
        else
        {
            printf("接收文件出错\n");
        }
    }
}

void CServerInstance::SaveDisConnectInfo(void)
{
    fpRcd = fopen(s_achRcdFilePath, "wb");
    s32 nRtn = fwrite(&m_tRcd, sizeof(m_tRcd), 1, fpRcd);
    if (nRtn <= 1)
    {
        printf("记录断链信息成功\n");
    }
    else
    {
        printf("记录断链信息失败\n");
    }
    fclose(fpRcd);
}

    


void CreateUserFile(s8 filePath[], s8 pRcdFilePath[])
{

    FILE *tfp = fopen(filePath, "rb");
    if (!tfp)
    {
        mkdir(filePath);
        mkdir(strcat(pRcdFilePath, "\\Rcd"));
    }
    else
    {
        fclose(tfp);
    }
}