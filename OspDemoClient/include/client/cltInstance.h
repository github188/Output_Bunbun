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

/*====================================================================
    Osp消息是有一个基准值的，用户在该基准值之上设定了几个新基准值。
也就是说，对于一个具体的业务请求，他只可能出现这几个基于基准值的消息。
例如，基准值有REQ,ACK,WORK,TERM...，对于一个具体的业务请求，他只可能出
现这几种情况的消息，所以可以设定一个这类消息的数量。那么对于发送文件
这个具体业务来说，有REQ_SENDFILE, ACK_SENDFILE...这些具体消息。而这些消
息在定义的时候是按基准消息的顺序放一起的，称为一个消息簇。
    那么ins在收到消息之后，会根据当前ins的状态选择处理该消息的函数。只
要将该消息号减去基准消息号，除以基准消息簇的长度，就可以得到该消息簇对
于基准消息簇的偏移量，即具体消息的位置。同样对消息长度取模之后，可以得
到该消息在消息簇中的位置。
====================================================================*/
    u16 GetMain(u16);           //得到消息主分支
    u16 GetBran(u16);           //得到消息辅助分支

    void (CClientInstance::*IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //空闲状态处理消息函数簇
    void (CClientInstance::*ConnectEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);        //连接状态处理消息函数簇
    void (CClientInstance::*ReqEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);            //请求状态处理消息函数簇
    void (CClientInstance::*WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //工作状态处理消息函数簇
    void (CClientInstance::*TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //终止状态处理消息函数簇

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
    //void Connect_Ack_DisConncet(CMessage *const pMsg);

    void Req_Req_DisConnect(CMessage *const pMsg);
    void Req_Ack_DisConnect(CMessage *const pMsg);
    void Req_Term_DisConnect(CMessage *const pMsg);
    void Req_Timeout_DisConnect(CMessage *const pMsg);
    
    

/*====================================================================
        相关业务的具体处理
====================================================================*/
private:
    void sendFile(CMessage *const pMsg);
};


#endif //cltInstance.h