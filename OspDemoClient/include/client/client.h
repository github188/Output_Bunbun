#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "pubInfo.h"


/*
    与服务器连接信息的保存
*/
typedef struct ConnectInfo
{
    /* 目的节点 */
    s32 m_nDstnode;
    /* 目的包 */
    CMessage *m_pMsg;
}TConnectInfo, *PTConnectInfo;

/* 服务器连接信息保存 */
extern PTConnectInfo g_ptConnectInfo;

/* 自用户信息保存 */
extern CUserInfo *g_ptCltUserInfo;

/* ==================================================================== 
函数名 ：UserInterface
功能 ：打印出用户操作界面
算法实现 ： 
参数说明 ： 
---------------------------------------------------------------------- 
修改记录 ： 
日 期          版本 修改人 走读人 修改记录 
==================================================================== */
extern void UserInterface(void);           

/*
    用户操作指令
*/
#define U_INVALID     (u16)(1)  /* 预留指令 */
#define U_CATOTHERS   (u16)(2)  /* 查看其他用户 */
#define U_TRANSINFO   (u16)(3)  /* 转发信息 */
#define U_TRANSFILE   (u16)(4)  /* 转发文件 */
#define U_SENDFILE    (u16)(5)  /* 发送文件至服务器 */
#define U_SENDCHAR    (u16)(6)  /* 发送文本信息至服务器 */
#define U_DISCONNECT  (u16)(7)  /* 与服务器断开连接 */
#define U_CLEAR       (u16)(0)  /* 清屏 */

#endif  /* client.h */



