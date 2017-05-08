#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "transInfo.h"


/* =============================================================================
���� ��CServerInstance
���� ���̳�OSPģ����CInstance�ķ������Ϣ������
��Ҫ�ӿڣ�void InstanceEntry:ʵ����Ϣ������ں���
��ע ��
-------------------------------------------------------------------------------
�޸ļ�¼�� 
�� ��             �汾    �޸���             �߶���             �޸ļ�¼ 

============================================================================= */
class CServerInstance : public CInstance
{
public:
    CServerInstance();
    virtual~CServerInstance(){};
public:
/* ====================================================================
    ��������˵��DaemonIns����ͨIns�������ֵģ������û��ֲ�����
    �����ƣ���Demo��DaemonIns����ͨIns��������ʵ������һ�����ͣ�������
    ֻ�������ǵ�ִ����������һЩ����
==================================================================== */
    TUserInfo m_ptUserInfo[MAXINS];   /* DaemonIns�������ߵĿͻ���Ϣ */
    TUserInfo m_ptCurUser;            /* ��ͨIns��ŵ�ǰ�������û���Ϣ */
    s32 m_nCurInsNum;                 /* DaemonIns��ŵ�ǰ�����ӵ��û��� */
    TRcdInfo m_tRcd;                 /* ����ļ�������Ϣ */
private:
    void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    void InstanceEntry(CMessage *const pMsg);

private:

    /* Idle״̬,��Ϣ��ں��� */
    void IdleFunction(CMessage *const pMsg);
    /* Idle״̬�����û����ӣ����� */
    void IdleReqInsConnect(CMessage *const pMsg);

    /* Ack״̬,��Ϣ��ں��� */
    void AckFunction(CMessage *const pMsg);

    /* Work״̬,��Ϣ��ں��� */
    void WorkFunction(CMessage *const pMsg);

    /* Term״̬,��Ϣ��ں��� */
    void TermFunction(CMessage *const pMsg);

private:
    /* ����ִ�к��� */
    /* �鿴�����û� */
    void CatOthers(CMessage *const pMsg);

    /* ת���ı���Ϣ */
    void TransInfo(CMessage *const pMsg);

    /* �����ļ� */
    void RcvFile(CMessage *const pMsg);

    /* �����ı���Ϣ */
    void RcvChar(CMessage *const pMsg);

    /* ���������Ϣ */
    void SaveDisConnectInfo(void);
};

#endif/* srvInstance.h */