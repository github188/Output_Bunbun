#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"



/*
    基于Osp的应用必须从该CInstance基类派生一个具体的Instance类，
    并给出一个InstanceEntry()成员函数的具体实现
	CInstance是一个状态机实例的基类，提供了描述一个状态机所需的基本数据与接口：
    状态记录，状态变迁；
    定时器；
    事件（消息）处理接口；
    发送事件（消息）接口等；
*/

class CServerInstance : public CInstance
{
public:
    CServerInstance();


private:
    struct UserInfo{};
    UserInfo m_pUserInfo[MAXINS];
    s32 m_dwCurInsNum;


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

    /*Idle状态*/
    virtual void Idle_Function(CMessage *const pMsg);

    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);

    /*Ack状态*/
    virtual void Ack_Function(CMessage *const pMsg);

    void Ack_Req_CatOthers(CMessage *const pMsg);

    /*Work状态*/
    virtual void Work_Function(CMessage *const pMsg);

    /*Term状态*/
    virtual void Term_Function(CMessage *const pMsg);

private:
    void CatOthers(CMessage *const pMsg);


    
};

#endif//srvInstance.h