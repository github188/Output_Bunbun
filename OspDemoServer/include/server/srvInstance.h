#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "transInfo.h"


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
/*====================================================================
        ��������˵��DaemonIns����ͨIns�������ֵģ������û��ֲ�����
    �����ƣ���Demo��DaemonIns����ͨIns��������ʵ������һ�����ͣ�������
    ֻ�������ǵ�ִ������һЩ����
====================================================================*/
    UserInfo m_pUserInfo[MAXINS];       //DaemonIns�����������ߵĿͻ���Ϣ
    UserInfo m_pCurUser;                //��ͨIns��ŵ�ǰ�������û���Ϣ
    s32 m_dwCurInsNum;                  //DaemonIns��ŵ�ǰ�����ӵ��û���
    CRcdInfo m_Rcd;                     //����ļ�������Ϣ
private:
    virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    virtual void InstanceEntry(CMessage *const pMsg);

private:
    u16 GetMain(u16);
    u16 GetBran(u16);

    void (CServerInstance::*IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance::*AckEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance::*WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CServerInstance::*TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);

    /*Idle״̬*/
    virtual void Idle_Function(CMessage *const pMsg);

    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);

    /*Ack״̬*/
    virtual void Ack_Function(CMessage *const pMsg);

    void Ack_Req_CatOthers(CMessage *const pMsg);

    void Ack_Req_TransInfo(CMessage *const pMsg);

    void Ack_Req_SendFile(CMessage *const pMsg); 

    void Ack_Req_SendChar(CMessage *const pMsg);

    void Ack_Req_DisConnect(CMessage *const pMsg);

    void Ack_Ack_DisConnect(CMessage *const pMsg);

    /*Work״̬*/
    virtual void Work_Function(CMessage *const pMsg);
    void Work_Ack_SendFile(CMessage *const pMsg);
    void Work_Ack_SendChar(CMessage *const pMsg);
    void Work_Term_CatOthers(CMessage *const pMsg);
    void Work_Term_TransInfo(CMessage *const pMsg);
    void Work_Term_SendFile(CMessage *const pMsg);
    void Work_Term_SendChar(CMessage *const pMsg);

    void Work_OspDisConnect(CMessage *const pMsg);
    /*Term״̬*/
    virtual void Term_Function(CMessage *const pMsg);
    void Term_Req_DisConnect(CMessage *const pMsg);

private:
    void catOthers(CMessage *const pMsg);
    void transInfo(CMessage *const pMsg);
    void rcvFile(CMessage *const pMsg);
    void rcvChar(CMessage *const pMsg);
    void saveDisConnectInfo();


    
};

#endif//srvInstance.h