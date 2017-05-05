#include "cltInstance.h"

static s8 SENDBUFF[BUFFSIZE];
static s8 *GetFileName(s8 []);
static FILE *s_fp = NULL;
static CFileMessage *s_tfMsg = NULL;


void CClientInstance::SendFile(CMessage *const pMsg)
{
    
    s8 m_achFilePath[MAXFILENAME];
    if (!s_fp)                    /* ��һ���ϴ���������ʼ�ļ���Ϣ */
    {
        s_tfMsg = (CFileMessage *)malloc(sizeof(CFileMessage));
        if (!(pMsg->content))        /* ��һ�������ϴ� */
        {
            printf("��������Ҫ���͵��ļ���·��:\n");
            scanf("%s", m_achFilePath);
            strcpy(s_tfMsg->m_achFilePath, m_achFilePath);
            strcpy(s_tfMsg->m_achFileName, GetFileName(m_achFilePath));
            s_tfMsg->m_curLocal = 1;
            s_fp = fopen(m_achFilePath, "rb");
            if (!s_fp)
            {
                printf("���ļ�����\n");
            }
            else
            {
                /* ��ȡ�ļ���С */
                fseek(s_fp, 0, SEEK_END);
                s_tfMsg->m_fileSize = ftell(s_fp);
                printf("m_fileSize : %d\n", s_tfMsg->m_fileSize);
                fseek(s_fp, 0, SEEK_SET);
            }
        }
        else                /* ��һ������ */
        {
            printf("��һ������\n");
            s_tfMsg->m_curLocal = ((CRcdInfo *)pMsg->content)->m_nAldRcdSeek;
            strcpy(s_tfMsg->m_achFilePath, ((CRcdInfo *)pMsg->content)->m_tfMsg.m_achFilePath);
            strcpy(s_tfMsg->m_achFileName, ((CRcdInfo *)pMsg->content)->m_tfMsg.m_achFileName);
            s_tfMsg->m_fileSize = ((CRcdInfo *)pMsg->content)->m_tfMsg.m_fileSize;
            printf("m_fileSize = %d\n", s_tfMsg->m_fileSize);
            s_fp = fopen(s_tfMsg->m_achFilePath, "rb");
            fseek(s_fp, BUFFSIZE*s_tfMsg->m_curLocal, SEEK_SET);
            s_tfMsg->m_curLocal++;

        }
    }
    else    /* ��ӡ�ϴ��ļ��Ľ��� */
    {

        s_tfMsg->m_curLocal = ((CFileMessage *)pMsg->content)->m_curLocal;
        s_tfMsg->m_fileSize = ((CFileMessage *)pMsg->content)->m_fileSize;
        strcpy(s_tfMsg->m_achFilePath, ((CFileMessage *)pMsg->content)->m_achFilePath);
        strcpy(s_tfMsg->m_achFileName, ((CFileMessage *)pMsg->content)->m_achFileName);
        printf("%.2f%%�Ѵ���\n", ((float)s_tfMsg->m_curLocal)/((float)s_tfMsg->m_fileSize/BUFFSIZE)*100);
        s_tfMsg->m_curLocal++;
        
    }
    
    if (s_fp)  /* �ļ��򿪳ɹ� */
    {   
        /* ��¼�����ϴ����Ĵ�С */
        s_tfMsg->m_curBufSize = s_tfMsg->m_fileSize - (s_tfMsg->m_curLocal - 1) * BUFFSIZE;   
        if (BUFFSIZE * s_tfMsg->m_curLocal >= s_tfMsg->m_fileSize)         /* ���һ���ϴ� */
        {
            if (fread(s_tfMsg->m_achBuf, s_tfMsg->m_curBufSize, 1, s_fp) <= 1)
            {
                
                OspPost(g_ptConnectInfo->m_pMsg->srcid, EV_ACK_SENDFILE,
                    s_tfMsg, sizeof(CFileMessage),
                    g_ptConnectInfo->m_pMsg->srcnode,
                    MAKEIID(GetAppID(), GetInsID()));
                fclose(s_fp);
                
            } 
        }
        else
        {
            s_tfMsg->m_curBufSize = BUFFSIZE;
            if (fread(s_tfMsg->m_achBuf, BUFFSIZE, 1, s_fp) <= 1)
            {
              
                OspPost(g_ptConnectInfo->m_pMsg->srcid, EV_ACK_SENDFILE,
                    s_tfMsg, sizeof(CFileMessage),
                    g_ptConnectInfo->m_pMsg->srcnode,
                    MAKEIID(GetAppID(), GetInsID()));
            }
        }   
    }
}

s8 *GetFileName(s8 pchFilePath[])
{
    s32 nLen = strlen(pchFilePath);
    s8 achDownFileName[MAXFILENAME];
    s8 m_achFileName[MAXFILENAME];
    s32 nFileLen = 0;
    for (s32 i = nLen - 1; i >= 0; i--, nFileLen++)
    {
        if (pchFilePath[i] == '\\')
        {
            break;
        }
        achDownFileName[nLen - i - 1] = pchFilePath[i];
    }
    
    for (s32 i = 0; i < nFileLen; i++)
    {
        m_achFileName[i] = achDownFileName[nFileLen - i - 1];
    }
    m_achFileName[nFileLen] = '\0';
    return m_achFileName;
}