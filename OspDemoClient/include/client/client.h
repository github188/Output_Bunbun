#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "pubInfo.h"


/*
    �������������Ϣ�ı���
*/
typedef struct tagConnectInfo
{
    /* Ŀ�Ľڵ� */
    s32 m_nDstnode;
    /* Ŀ�İ� */
    CMessage *m_pMsg;
}TConnectInfo, *PTConnectInfo;

/* ������������Ϣ���� */
extern PTConnectInfo g_ptConnectInfo;

/* ���û���Ϣ���� */
extern PTClientUserInfo g_ptCltUserInfo;

/* ==================================================================== 
������ ��UserInterface
���� ����ӡ���û���������
�㷨ʵ�� �� 
����˵�� �� 
---------------------------------------------------------------------- 
�޸ļ�¼ �� 
�� ��          �汾 �޸��� �߶��� �޸ļ�¼ 
==================================================================== */
extern void UserInterface(void);           

/*
    �û�����ָ��
*/
#define U_INVALID     (u16)(1)  /* Ԥ��ָ�� */
#define U_CATOTHERS   (u16)(2)  /* �鿴�����û� */
#define U_TRANSINFO   (u16)(3)  /* ת����Ϣ */
#define U_TRANSFILE   (u16)(4)  /* ת���ļ� */
#define U_SENDFILE    (u16)(5)  /* �����ļ��������� */
#define U_SENDCHAR    (u16)(6)  /* �����ı���Ϣ�������� */
#define U_DISCONNECT  (u16)(7)  /* ��������Ͽ����� */
#define U_CLEAR       (u16)(0)  /* ���� */

#endif  /* client.h */



