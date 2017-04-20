#ifndef _PUBINFO_H_
#define _PUBINFO_H_


/*====================================================================
        公用的一些重要变量buf，大小限制
====================================================================*/

#define IPSTR_MAX   50                  //IP字串
#define MAXINS      8                   //服务器实例最大连接数
#define MAXFILENAME 50                  //文件名最大长度
#define BUFFSIZE    1024                //发送文件包buf最大限制
#define MAXUSERNAME 32                  //用户名最大长度
#define MD5SIZE     65                  //摘要后的MD5最长长度
/*====================================================================
定义块:
    1、OSP的消息类型是16字节整形，OSP用户定义的消息类型应该大于该基准值，
    该基准值之下是OSP保留消息号
        #define   OSP_USEREVENT_BASE		(u16)0x0400
    2、 尤其是客户端、服务端模式的程序，必须确保统一消息号
====================================================================*/
/*====================================================================
        消息换算限制相关，并非消息本体
====================================================================*/

#define EVENT_T                 (u16)(7)                            //消息周期
#define EVENT_MAIN_MAXN         (u16)(8)                            //消息主类型最大值
#define EVENT_BRAN_MAXN         (u16)((65535 - 1000) / 7)           //消息辅助类型最大值
#define EVENT_Y                 (u16)(1)                            //同意
#define EVENT_N                 (u16)(0)                            //拒绝

/*====================================================================
        基准消息，所有实用消息都是基于该消息
        不同消息顺序也是按基准消息的顺序排的
====================================================================*/

#define EVENT_REQ               (u16)(1 + OSP_USEREVENT_BASE)           //请求类型
#define EVENT_ACK               (u16)(2 + OSP_USEREVENT_BASE)           //应答类型
#define EVENT_TERM              (u16)(3 + OSP_USEREVENT_BASE)           //终止类型
#define EVENT_TIMEOUT           (u16)(4 + OSP_USEREVENT_BASE)           //超时类型
                                                                        //预留
                                                                        //预留
                                                                        //预留


/*====================================================================
        客户与服务器实例连接消息定义
====================================================================*/
#define EVENT_REQ_INSCONNECT           (u16)(EVENT_T*1 + EVENT_REQ)
#define EVENT_ACK_INSCONNECT           (u16)(EVENT_T*1 + EVENT_ACK)
#define EVENT_TERM_INSCONNECT          (u16)(EVENT_T*1 + EVENT_TERM)
#define EVENT_TIMEOUT_INSCONNECT       (u16)(EVENT_T*1 + EVENT_TIMEOUT)

/*====================================================================
        客户查看与服务器连接的其他用户
====================================================================*/
#define EVENT_REQ_CATOTHERS            (u16)(EVENT_T*2 + EVENT_REQ)
#define EVENT_ACK_CATOTHERS            (u16)(EVENT_T*2 + EVENT_ACK)
#define EVENT_TERM_CATOTHERS           (u16)(EVENT_T*2 + EVENT_TERM)
#define EVENT_TIMEOUT_CATOTHERS        (u16)(EVENT_T*2 + EVENT_TIMEOUT)

/*====================================================================
        转发消息相关定义
====================================================================*/
#define EVENT_REQ_TRANSINFO            (u16)(EVENT_T*3 + EVENT_REQ)
#define EVENT_ACK_TRANSINFO            (u16)(EVENT_T*3 + EVENT_ACK)
#define EVENT_TERM_TRANSINFO           (u16)(EVENT_T*3 + EVENT_TERM)
#define EVENT_TIMEOUT_TRANSINFO        (u16)(EVENT_T*3 + EVENT_TIMEOUT)

typedef struct CTransInfoBuffer
{
    s8 userName[MAXUSERNAME];
    s8 content[BUFFSIZE];
    s32 dwUserNum;                  //目的用户序号
    s32 dwCurIns;                   //与当前用户连接的实例号

}CTransInfoBuffer;
/*====================================================================
        发送文件相关定义
====================================================================*/
#define EVENT_REQ_SENDFILE             (u16)(EVENT_T*5 + EVENT_REQ)
#define EVENT_ACK_SENDFILE             (u16)(EVENT_T*5 + EVENT_ACK)
#define EVENT_TERM_SENDFILE            (u16)(EVENT_T*5 + EVENT_TERM)
#define EVENT_TIMEOUT_SENDFILE         (u16)(EVENT_T*5 + EVENT_TIMEOUT)

/*====================================================================
        发送文字相关定义
====================================================================*/
#define EVENT_REQ_SENDCHAR             (u16)(EVENT_T*6 + EVENT_REQ)
#define EVENT_ACK_SENDCHAR             (u16)(EVENT_T*6 + EVENT_ACK)
#define EVENT_TERM_SENDCHAR            (u16)(EVENT_T*6 + EVENT_TERM)
#define EVENT_TIMEOUT_SENDCHAR         (u16)(EVENT_T*6 + EVENT_TIMEOUT)

/*====================================================================
        用户断开连接相关定义
====================================================================*/
#define EVENT_REQ_DISCONNECT           (u16)(EVENT_T*7 + EVENT_REQ)
#define EVENT_ACK_DISCONNECT           (u16)(EVENT_T*7 + EVENT_ACK)
#define EVENT_TERM_DISCONNECT          (u16)(EVENT_T*7 + EVENT_TERM)
#define EVENT_TIMEOUT_DISCONNECT       (u16)(EVENT_T*7 + EVENT_TIMEOUT)

/*====================================================================
结构体名：UserInfo
功能：服务器存储连接客户信息
====================================================================*/
typedef struct UserInfo
{
    CMessage *pMsg;
    s8 pByAlias[20];
    s32 dwNumber;
    s32 dwState;
    s32 dwInsID;

}UserInfo;


/*====================================================================
定义块：Osp应用程序之间需要遵循的规则:
    1、不同的APP有不同的APP编号，通过编号来指定APP；
	2、APP编号为非0的16位整形数；
	3、为了确保OSP消息通讯能够正常进行，所有OSP应用程序对于APP编号的约定必须一致；
	4、用户可以自定义APP编号，但要确保全局范围内APP号不产生冲突。通常有公用头文件来让用户自定义APP编号；
====================================================================*/

/*====================================================================
        服务器APP相关定义规则
====================================================================*/
#define SRV_APP_NO     (u16)(2)         //服务器APP号

#define SRV_APP_PRI         100         //APP优先级

#define SRV_APP_QUE_SIZE    1000        //最大等待消息数

#define SRV_APP_STK_SIZE    (200<<10)   //APP任务的栈大小

/*====================================================================
        客户端APP相关定义规则
====================================================================*/
#define CLT_APP_NO     (u16)(5)         //客户端APP号

#define CLT_APP_PRI         100         //APP优先级

#define CLT_APP_QUE_SIZE    1000        //最大等待消息数

#define CLT_APP_STK_SIZE    (200<<10)   //APP任务的栈大小

/*====================================================================
        服务器实例状态的定义
====================================================================*/
#define S_STATE_IDLE (u16)(0)           //空闲状态
#define S_STATE_REQ  (u16)(1)           //请求状态，暂时无用
#define S_STATE_ACK  (u16)(2)           //应答状态
#define S_STATE_WORK (u16)(3)           //工作状态
#define S_STATE_TERM (u16)(4)           //终止状态

/*====================================================================
        客户端实例状态定义
====================================================================*/
#define C_STATE_IDLE        (u16)(0)    //空闲状态
#define C_STATE_CONNECT     (u16)(1)    //连接状态
#define C_STATE_REQ         (u16)(2)    //请求状态
#define C_STATE_WORK        (u16)(3)    //工作状态
#define C_STATE_TERM        (u16)(4)    //终止状态


/*====================================================================
结构体名：CFileMessage
功能：客户端及服务端交互文件时候所使用的文件的包
====================================================================*/
typedef struct CFileMessage
{
    s8 pFileName[MAXFILENAME];          //文件名
    s8 pFilePath[MAXFILENAME];          //文件路径
    s8 pBuf[BUFFSIZE];                  //该次包的内容
    s64 fileSize;                       //文件大小
    s64 curLocal;                       //当前定位
    s64 curBufSize;                     //当前包包含有用信息的大小
}CFileMessage;

/*====================================================================
结构体名：CRcdInfo
功能：客户端和服务器存放文件交互信息
====================================================================*/
typedef struct CRcdInfo
{
    s32 dwAldRcdSeek;
    s8 pMD5[MD5SIZE];
    s8 pUsername[MAXUSERNAME];
    CFileMessage fMsg;
    bool bFinish;
}CRcdInfo;
/*====================================================================
结构体名：CUserInfo
功能：客户端向服务器提供的个人信息包
====================================================================*/
#define ONLINE      (u16)(1)
#define OFFLINE     (u16)(0)
typedef struct  CUserInfo
{
    s8 username[MAXUSERNAME];
    CMessage pMsg;

}CUerInfo;

/*====================================================================
结构体名：CSUserInfo
功能：服务器内部注册用户信息的包
====================================================================*/
typedef struct CSUserInfo
{
    CMessage *pMsg;
    u16 wFlag;
}CSUserInfo;



#endif //pubInfo.h