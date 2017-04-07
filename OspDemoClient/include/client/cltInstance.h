#include "osp.h"
#include "pubInfo.h"
#include "client.h"

#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_
#endif

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
    void ReqFunction(CMessage *const pMsg);
    void AckFuction(CMessage *const pMsg);
    void Term_Fuction(CMessage *const pMsg);
    void Timeout_Function();
    void Def_Fuction(CMessage *const pMsg);
    void StartWork(CMessage *const pMsg);
    void Printf_Function(CMessage *const pMsg);
};
