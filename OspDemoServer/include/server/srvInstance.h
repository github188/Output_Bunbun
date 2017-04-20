#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "transInfo.h"


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
/*====================================================================
        理论上来说，DaemonIns和普通Ins是有区分的，由于用户手册的设计
    与限制，本Demo对DaemonIns和普通Ins在声明与实现上是一个类型，无区别，
    只是在他们的执行上做一些区分
====================================================================*/
    UserInfo m_pUserInfo[MAXINS];       //DaemonIns用来保存在线的客户信息
    UserInfo m_pCurUser;                //普通Ins存放当前相连的用户信息
    s32 m_dwCurInsNum;                  //DaemonIns存放当前已连接的用户数
    CRcdInfo m_Rcd;                     //存放文件交互信息
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

    /*Idle状态*/
    virtual void Idle_Function(CMessage *const pMsg);

    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);

    /*Ack状态*/
    virtual void Ack_Function(CMessage *const pMsg);

    void Ack_Req_CatOthers(CMessage *const pMsg);

    void Ack_Req_TransInfo(CMessage *const pMsg);

    void Ack_Req_SendFile(CMessage *const pMsg); 

    void Ack_Req_SendChar(CMessage *const pMsg);

    void Ack_Req_DisConnect(CMessage *const pMsg);

    void Ack_Ack_DisConnect(CMessage *const pMsg);

    /*Work状态*/
    virtual void Work_Function(CMessage *const pMsg);
    void Work_Ack_SendFile(CMessage *const pMsg);
    void Work_Ack_SendChar(CMessage *const pMsg);
    void Work_Term_CatOthers(CMessage *const pMsg);
    void Work_Term_TransInfo(CMessage *const pMsg);
    void Work_Term_SendFile(CMessage *const pMsg);
    void Work_Term_SendChar(CMessage *const pMsg);

    void Work_OspDisConnect(CMessage *const pMsg);
    /*Term状态*/
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