#include "osp.h"
#include "pubInfo.h"
#include "client.h"

#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_
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
class CClientInstance : public CInstance
{
public:
    CClientInstance();

private:
    virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    virtual void InstanceEntry(CMessage *const pMsg);

private:
    void ReqFunction(CMessage *const pMsg);
    void AckFuction(CMessage *const pMsg);
    void Term_Fuction(CMessage *const pMsg);
    void Timeout_Function();
    void Def_Fuction(CMessage *const pMsg);
    void StartWork(CMessage *const pMsg);
    void Printf_Function(CMessage *const pMsg);
};
