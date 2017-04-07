#include "osp.h"
#include "pubInfo.h"

#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#endif
/*
    ����Osp��Ӧ�ñ���Ӹ�CInstance��������һ�������Instance�࣬
    ������һ��InstanceEntry()��Ա�����ľ���ʵ��
	CInstance��һ��״̬��ʵ���Ļ��࣬�ṩ������һ��״̬������Ļ���������ӿڣ�
    ״̬��¼��״̬��Ǩ��
    ��ʱ����
    �¼�����Ϣ������ӿڣ�
    �����¼�����Ϣ���ӿڵȣ�
*/

class CServerInstance : public CInstance
{
public:
    CServerInstance();

private:
    virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    virtual void InstanceEntry(CMessage *const pMsg);

private:
    u16 GetMain(u16);
    u16 GetBran(u16);

    void (CServerInstance :: *IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance :: *AckEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance :: *WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance :: *TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);

    virtual void Idle_Function(CMessage *const pMsg);
    virtual void Ack_Function(CMessage *const pMsg);
    virtual void Work_Function(CMessage *const pMsg);
    virtual void Term_Function(CMessage *const pMsg);

    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);
};
