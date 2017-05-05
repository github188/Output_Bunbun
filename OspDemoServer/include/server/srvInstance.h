#ifndef _SRVINSTANCE_H_
#define _SRVINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "transInfo.h"


/* =============================================================================
类名 ：CServerInstance
功能 ：继承OSP模板类CInstance的服务端消息处理类
主要接口：void InstanceEntry:实例消息处理入口函数
备注 ：
-------------------------------------------------------------------------------
修改记录： 
日 期             版本    修改人             走读人             修改记录 

============================================================================= */
class CServerInstance : public CInstance
{
public:
    CServerInstance();
    virtual~CServerInstance(){};
public:
/* ====================================================================
    理论上来说，DaemonIns和普通Ins是有区分的，由于用户手册的设计
    与限制，本Demo对DaemonIns和普通Ins在声明与实现上是一个类型，无区别，
    只是在他们的执行任务上做一些区分
==================================================================== */
    TUserInfo m_ptUserInfo[MAXINS];   /* DaemonIns保存在线的客户信息 */
    TUserInfo m_ptCurUser;            /* 普通Ins存放当前相连的用户信息 */
    s32 m_nCurInsNum;                 /* DaemonIns存放当前已连接的用户数 */
    TCRcdInfo m_tRcd;                 /* 存放文件交互信息 */
private:
    void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    void InstanceEntry(CMessage *const pMsg);

private:

    /* Idle状态,消息入口函数 */
    void IdleFunction(CMessage *const pMsg);
    /* Idle状态处理用户连接，重连 */
    void IdleReqInsConnect(CMessage *const pMsg);

    /* Ack状态,消息入口函数 */
    void AckFunction(CMessage *const pMsg);

    /* Work状态,消息入口函数 */
    void WorkFunction(CMessage *const pMsg);

    /* Term状态,消息入口函数 */
    void TermFunction(CMessage *const pMsg);

private:
    /* 功能执行函数 */
    /* 查看其他用户 */
    void CatOthers(CMessage *const pMsg);

    /* 转发文本信息 */
    void TransInfo(CMessage *const pMsg);

    /* 接收文件 */
    void RcvFile(CMessage *const pMsg);

    /* 接收文本信息 */
    void RcvChar(CMessage *const pMsg);

    /* 保存断链信息 */
    void SaveDisConnectInfo(void);
};

#endif/* srvInstance.h */