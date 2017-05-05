#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "client.h"

/* =============================================================================
���� ��CClientInstance
���� ���̳�OSPģ����CInstance�Ŀͻ�����Ϣ������
��Ҫ�ӿڣ�void InstanceEntry:ʵ����Ϣ������ں���
��ע ��
-------------------------------------------------------------------------------
�޸ļ�¼�� 
�� ��             �汾    �޸���             �߶���             �޸ļ�¼ 

============================================================================= */ 
class CClientInstance : public CInstance
{
public:
    CClientInstance();
    virtual ~CClientInstance(){};
private:
    void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    void InstanceEntry(CMessage *const pMsg);

private:
  
    void IdleFunction(CMessage *const pMsg);      /* ����״̬������Ϣ��� */
    void ConnectFunction(CMessage *const pMsg);   /* ����״̬������Ϣ��� */
    void ReqFunction(CMessage *const pMsg);       /* ����״̬������Ϣ��� */
    void WorkFunction(CMessage *const pMsg);      /* ����״̬������Ϣ��� */
    void TermFunction(CMessage *const pMsg);      /* ��ֹ״̬������Ϣ��� */

    /* ʵ�����ӽ������� */
    void IdleAckInsConnect(CMessage *const pMsg);    

    /* ��Ϣת���������� */
    void WorkTermTransInfo(CMessage *const pMsg);
       
private:
    /* �����ļ� */
    void SendFile(CMessage *const pMsg);
    /* �����ı���Ϣ */
    void SendChar(CMessage *const pMsg);
};


#endif /* cltInstance.h */