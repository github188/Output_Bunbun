#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_
#include "osp.h"
#include "pubInfo.h"
#include "client.h"

/* =============================================================================
类名 ：CClientInstance
功能 ：继承OSP模板类CInstance的客户端消息处理类
主要接口：void InstanceEntry:实例消息处理入口函数
备注 ：
-------------------------------------------------------------------------------
修改记录： 
日 期             版本    修改人             走读人             修改记录 

============================================================================= */ 
class CClientInstance : public CInstance
{
public:
    CClientInstance();
    virtual ~CClientInstance(){};
private:
    void DaemonInstanceEntry( CMessage *const pMsg, CApp *pApp);
    void InstanceEntry(CMessage *const pMsg);

private:
  
    void IdleFunction(CMessage *const pMsg);      /* 空闲状态处理消息入口 */
    void ConnectFunction(CMessage *const pMsg);   /* 连接状态处理消息入口 */
    void ReqFunction(CMessage *const pMsg);       /* 请求状态处理消息入口 */
    void WorkFunction(CMessage *const pMsg);      /* 工作状态处理消息入口 */
    void TermFunction(CMessage *const pMsg);      /* 终止状态处理消息入口 */

    /* 实例连接交互处理 */
    void IdleAckInsConnect(CMessage *const pMsg);    

    /* 消息转发交互处理 */
    void WorkTermTransInfo(CMessage *const pMsg);
       
private:
    /* 发送文件 */
    void SendFile(CMessage *const pMsg);
    /* 发送文本信息 */
    void SendChar(CMessage *const pMsg);
};


#endif /* cltInstance.h */