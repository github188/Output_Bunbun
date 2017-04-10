#ifndef _CLIENT_H_
#define _CLIENT_H_


/*重要的全局变量*/
struct ConnectInfo
{
    s32 dstnode;
    CMessage *pMsg;
};

extern struct ConnectInfo *g_pConnectInfo;

/*用户操作指令*/
#define INVALID         (u16)(0)




/*用户操作界面*/
void UserInterface();



/*用户指令实现*/

/*
    1.保留
*/

#define U_INVALID     (u16)(1)
#define U_CATOTHERS   (u16)(2)
#define U_TRANSINFO   (u16)(3)
#define U_TRANSALL    (u16)(4)
#define U_SENDFILE    (u16)(5)
#define U_SENDCHAR    (u16)(6)
#define U_DISCONNECT  (u16)(7)
#define U_CLEAR       (u16)(0)

/*
    2.查看其他在线客户
*/


#endif  //client.h



