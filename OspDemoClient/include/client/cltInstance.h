#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_

#include "osp.h"
#include "pubInfo.h"
#include "client.h"




/*
    基于Osp的应用必须从该CInstance基类派生一个具体的Instance类，
    并给出一个InstanceEntry()成员函数的具体实现
	CInstance是一个状态机实例的基类，提供了描述一个状态机所需的基本数据与接口：
    状态记录，状态变迁；
    定时器；
    事件（消息）处理接口；
    发送事件（消息）接口等；
*/
class CClientInstance : public CInstance
{
public:
    CClientInstance();

private:
    virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    virtual void InstanceEntry(CMessage *const pMsg);

private:
    u16 GetMain(u16);
    u16 GetBran(u16);

    void (CClientInstance :: *IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CClientInstance :: *ConnectEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CClientInstance :: *ReqEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CClientInstance :: *WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);
    void (CClientInstance :: *TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);

    void Idle_Function(CMessage *const pMsg);
    void Connect_Function(CMessage *const pMsg);
    void Req_Function(CMessage *const pMsg);
    void Work_Function(CMessage *const pMsg);
    void Term_Function(CMessage *const pMsg);

    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);
    void Idle_Term_InsConnect(CMessage *const pMsg);
    void Idle_Timeout_InsConnect(CMessage *const pMsg);

    void Connect_Req_CatOthers(CMessage *const pMsg);
    void Connect_Ack_CatOthers(CMessage *const pMsg);
    void Connect_Term_CatOthers(CMessage *const pMsg);
    void Connect_Timeout_CatOthers(CMessage *const pMsg);

    void Connect_Req_TransInfo(CMessage *const pMsg);
    void Connect_Ack_TransInfo(CMessage *const pMsg);
    void Connect_Term_TransInfo(CMessage *const pMsg);
    void Connect_Timeout_TransInfo(CMessage *const pMsg);

    //void Connect_Req_TransAll(CMessage *const pMsg);
    void Req_Req_CatOthers(CMessage *const pMsg);
    void Req_Ack_CatOthers(CMessage *const pMsg);
    void Req_Term_CatOthers(CMessage *const pMsg);
    void Req_Timeout_CatOthers(CMessage *const pMsg);

    void Req_Req_TransInfo(CMessage *const pMsg);
    void Req_Ack_TransInfo(CMessage *const pMsg);
    void Req_Term_TransInfo(CMessage *const pMsg);
    void Req_Timeout_TransInfo(CMessage *const pMsg);

    void Req_Req_SendFile(CMessage *const pMsg);
    void Req_Ack_SendFile(CMessage *const pMsg);
    void Req_Term_SendFile(CMessage *const pMsg);
    void Req_Timeout_SendFile(CMessage *const pMsg);

    void Req_Req_SendChar(CMessage *const pMsg);
    void Req_Ack_SendChar(CMessage *const pMsg);
    void Req_Term_SendChar(CMessage *const pMsg);
    void Req_Timeout_SendChar(CMessage *const pMsg);

    void Req_Req_Quit(CMessage *const pMsg);
    void Req_Ack_Quit(CMessage *const pMsg);
    void Req_Term_Quit(CMessage *const pMsg);
    void Req_Timeout_Quit(CMessage *const pMsg);
    
    //WORK STATE
    void Work_Ack_CatOthers(CMessage *const pMsg);
    void Work_Term_CatOthers(CMessage *const pMsg);

    void Work_Ack_SendFile(CMessage *const pMsg);
    void Work_Term_SendFile(CMessage *const pMsg);

private:
    void sendFile(CMessage *const pMsg);
};


#endif //cltInstance.h