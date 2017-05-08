#include "srvInstance.h"
#include "io.h"
#include "direct.h"

/* �����ļ�ָ�� */
static FILE *fp = NULL;

/* ��¼�ϴ������ļ�ָ�� */
static FILE *fpRcd = NULL;

/* �����ļ�·�� */
static s8 s_achFilePath[MAXFILENAME];

/* �����ļ�·�� */
static s8 s_achRcdFilePath[MAXFILENAME];

/* ��¼�ļ���Ϣ */
static PTFileMessage s_tfMsg = NULL;

/* ���������ļ�����ȱ����ļ� */
static void CreateUserFile(s8 [], s8 []);

void CServerInstance::RcvFile(CMessage *const pMsg)
{

    if (!fp)     /* ��һ�ν��� */
    {
        /* ��һ���������գ���¼�ļ���Ϣ */
        if (1 == ((PTFileMessage)pMsg->content)->m_curLocal)  
        {
            s_tfMsg = (PTFileMessage)malloc(sizeof(TFileMessage));
            s_tfMsg->m_fileSize = ((PTFileMessage)pMsg->content)->m_fileSize;
            strcpy(s_tfMsg->m_achFilePath,
                ((PTFileMessage)pMsg->content)->m_achFilePath);
            strcpy(s_tfMsg->m_achFileName,
                ((PTFileMessage)pMsg->content)->m_achFileName);

            /* ��¼�ϴ��ļ��Ŀͻ�����Ϣ */
            m_tRcd.m_tfMsg.m_fileSize = s_tfMsg->m_fileSize;
            strcpy(m_tRcd.m_tfMsg.m_achFileName,
                ((PTFileMessage)pMsg->content)->m_achFileName);
            strcpy(m_tRcd.m_tfMsg.m_achFilePath, s_tfMsg->m_achFilePath);

            strcpy(s_achFilePath, ".\\test\\");
            strcpy(s_achRcdFilePath, ".\\test\\");

            strcat(s_achFilePath, m_ptCurUser.m_achAlias);
            strcat(s_achRcdFilePath, m_ptCurUser.m_achAlias);
            strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);
        
            m_tRcd.m_bFinish = false;  /* ��¼״̬δ����ϴ� */
            
            /* �����ͻ�����ļ��� */
            CreateUserFile(s_achFilePath, s_achRcdFilePath);
            strcat(s_achFilePath, "\\");
            strcat(s_achFilePath, ((PTFileMessage)pMsg->content)->m_achFileName);
            printf("%s\n", s_achFilePath);
            fp = fopen(s_achFilePath, "wb");
            fpRcd = fopen(strcat(s_achRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);
        }
        else                /* ��һ���������� */
        {
            s_tfMsg = (PTFileMessage)malloc(sizeof(TFileMessage));
            s_tfMsg->m_fileSize = ((PTFileMessage)pMsg->content)->m_fileSize;
            strcpy(s_tfMsg->m_achFilePath,
                ((PTFileMessage)pMsg->content)->m_achFilePath);
            strcpy(s_tfMsg->m_achFileName,
                ((PTFileMessage)pMsg->content)->m_achFileName);
       
            strcpy(s_achFilePath, ".\\test\\");
            strcpy(s_achRcdFilePath, ".\\test\\");

            strcat(s_achFilePath, m_ptCurUser.m_achAlias);
            strcat(s_achRcdFilePath, m_ptCurUser.m_achAlias);
            strcpy(m_tRcd.m_achUsername, m_ptCurUser.m_achAlias);
        
            m_tRcd.m_bFinish = false;  /* ��¼״̬δ����ϴ� */

            CreateUserFile(s_achFilePath, s_achRcdFilePath);
            strcat(s_achFilePath, "\\");
            strcat(s_achFilePath, ((PTFileMessage)pMsg->content)->m_achFileName);
            printf("%s\n", s_achFilePath);
            strcpy(m_tRcd.m_tfMsg.m_achFilePath, s_achFilePath);/* ��¼�ϴ��ļ��Ŀͻ���·�� */

            fp = fopen(s_achFilePath, "wb");
            fseek(fp, BUFFSIZE*((PTFileMessage)pMsg->content)->m_curLocal, SEEK_SET);
            fpRcd = fopen(strcat(s_achRcdFilePath, "\\rcdInfo"), "wb");
            fclose(fpRcd);       
        }
    }

    s_tfMsg->m_curLocal = ((PTFileMessage)pMsg->content)->m_curLocal;
    s_tfMsg->m_curBufSize = ((PTFileMessage)pMsg->content)->m_curBufSize;
    
    if (!fp)
    {
        printf("�½��ļ�����\n");
    }
    else
    {
        
        s32 nRtn = fwrite(((PTFileMessage)pMsg->content)->m_achBuf, s_tfMsg->m_curBufSize, 1, fp);
        if (nRtn <= 1)
        {
            /* �������һ�� */
            if (BUFFSIZE * s_tfMsg->m_curLocal >= s_tfMsg->m_fileSize)
            {
                printf("100%%д��ɹ�\n");
                fclose(fp);
                OspPost(pMsg->srcid, EV_TERM_SENDFILE, s_tfMsg,
                    sizeof(TFileMessage), pMsg->srcnode, pMsg->dstid);
                OspPost(MAKEIID(GetAppID(), GetInsID()), EV_TERM_SENDFILE,
                    NULL, 0, 0);
            }
            else
            {
                printf("%.2f%%д��ɹ�\n",
                    ((float)s_tfMsg->m_curLocal) / ((float)s_tfMsg->m_fileSize / BUFFSIZE) * 100);
                OspPost(pMsg->srcid, EV_ACK_SENDFILE, s_tfMsg,
                    sizeof(TFileMessage), pMsg->srcnode, pMsg->dstid);
            }

            m_tRcd.m_nAldRcdSeek = s_tfMsg->m_curLocal;   /* ��¼�ɹ����͵��ļ�ƫ���� */
        }
        else
        {
            printf("�����ļ�����\n");
        }
    }
}

void CServerInstance::SaveDisConnectInfo(void)
{
    fpRcd = fopen(s_achRcdFilePath, "wb");
    s32 nRtn = fwrite(&m_tRcd, sizeof(m_tRcd), 1, fpRcd);
    if (nRtn <= 1)
    {
        printf("��¼������Ϣ�ɹ�\n");
    }
    else
    {
        printf("��¼������Ϣʧ��\n");
    }
    fclose(fpRcd);
}

    


void CreateUserFile(s8 filePath[], s8 pRcdFilePath[])
{
    /* ������пͻ���Ϣ���ļ����Ƿ���� */
    FILE *thfp = fopen(".\\test", "rb");
    if (!thfp)
    {
        mkdir(".\\test");
    }
    else
    {
        fclose(thfp);
    }
    /* ��ž����û���Ϣ���ļ��� */
    FILE *tfp = fopen(filePath, "rb");
    if (!tfp)
    {
        mkdir(filePath);
        mkdir(strcat(pRcdFilePath, "\\Rcd"));
    }
    else
    {
        printf("cunzai\n");
        fclose(tfp);
    }
}