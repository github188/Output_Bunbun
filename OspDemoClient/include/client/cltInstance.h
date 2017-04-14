#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_

#include "osp.h"
#include "pubInfo.h"
#include "client.h"




/*====================================================================
    基于Osp的应用必须从该CInstance基类派生一个具体的Instance类，
    并给出一个InstanceEntry()成员函数的具体实现
	CInstance是一个状态机实例的基类，提供了描述一个状态机所需的基本数据与接口：
    状态记录，状态变迁；
    定时器；
    事件（消息）处理接口；
    发送事件（消息）接口等；
====================================================================*/

/*====================================================================
        客户端实例类声明
====================================================================*/
class CClientInstance : public CInstance
{
public:
    CClientInstance();

private:
    virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    virtual void InstanceEntry(CMessage *const pMsg);

private:
    u16 GetMain(u16);           //得到消息主分支
    u16 GetBran(u16);           //得到消息辅助分支

    void (CClientInstance :: *IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //空闲状态处理消息函数簇
    void (CClientInstance :: *ConnectEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);        //连接状态处理消息函数簇
    void (CClientInstance :: *ReqEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);            //请求状态处理消息函数簇
    void (CClientInstance :: *WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //工作状态处理消息函数簇
    void (CClientInstance :: *TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //终止状态处理消息函数簇

    void Idle_Function(CMessage *const pMsg);                   //空闲状态处理消息入口
    void Connect_Function(CMessage *const pMsg);                //连接状态处理消息入口
    void Req_Function(CMessage *const pMsg);                    //请求状态处理消息入口
    void Work_Function(CMessage *const pMsg);                   //工作状态处理消息入口
    void Term_Function(CMessage *const pMsg);                   //终止状态处理消息入口

/*====================================================================
        函数簇的具体函数声明
====================================================================*/

/*====================================================================
        实例连接，一对一连接相关
====================================================================*/
    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);
    void Idle_Term_InsConnect(CMessage *const pMsg);
    void Idle_Timeout_InsConnect(CMessage *const pMsg);

/*====================================================================
        查看其他用户
====================================================================*/
    void Connect_Req_CatOthers(CMessage *const pMsg);
    void Connect_Ack_CatOthers(CMessage *const pMsg);
    void Connect_Term_CatOthers(CMessage *const pMsg);
    void Connect_Timeout_CatOthers(CMessage *const pMsg);

    void Req_Req_CatOthers(CMessage *const pMsg);
    void Req_Ack_CatOthers(CMessage *const pMsg);
    void Req_Term_CatOthers(CMessage *const pMsg);
    void Req_Timeout_CatOthers(CMessage *const pMsg);

    void Work_Ack_CatOthers(CMessage *const pMsg);
    void Work_Term_CatOthers(CMessage *const pMsg);

/*====================================================================
        消息转发
====================================================================*/
    void Connect_Req_TransInfo(CMessage *const pMsg);
    void Connect_Ack_TransInfo(CMessage *const pMsg);
    void Connect_Term_TransInfo(CMessage *const pMsg);
    void Connect_Timeout_TransInfo(CMessage *const pMsg);

    void Req_Req_TransInfo(CMessage *const pMsg);
    void Req_Ack_TransInfo(CMessage *const pMsg);
    void Req_Term_TransInfo(CMessage *const pMsg);
    void Req_Timeout_TransInfo(CMessage *const pMsg);

/*====================================================================
        文件上传
====================================================================*/
    void Req_Req_SendFile(CMessage *const pMsg);
    void Req_Ack_SendFile(CMessage *const pMsg);
    void Req_Term_SendFile(CMessage *const pMsg);
    void Req_Timeout_SendFile(CMessage *const pMsg);

    void Work_Ack_SendFile(CMessage *const pMsg);
    void Work_Term_SendFile(CMessage *const pMsg);

/*====================================================================
        文字消息发送
====================================================================*/
    void Req_Req_SendChar(CMessage *const pMsg);
    void Req_Ack_SendChar(CMessage *const pMsg);
    void Req_Term_SendChar(CMessage *const pMsg);
    void Req_Timeout_SendChar(CMessage *const pMsg);

/*====================================================================
        用户退出，断开连接
====================================================================*/
    void Req_Req_Quit(CMessage *const pMsg);
    void Req_Ack_Quit(CMessage *const pMsg);
    void Req_Term_Quit(CMessage *const pMsg);
    void Req_Timeout_Quit(CMessage *const pMsg);
    
    

/*====================================================================
        相关业务的具体处理
====================================================================*/
private:
    void sendFile(CMessage *const pMsg);
};


#endif //cltInstance.h