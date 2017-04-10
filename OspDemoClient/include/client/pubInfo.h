#ifndef _PUBINFO_H_
#define _PUBINFO_H_
/*
    OSP的消息类型是16字节整形，OSP用户定义的消息类型应该大于该基准值，
    该基准值之下是OSP保留消息号
    #define   OSP_USEREVENT_BASE		(u16)0x0400
    尤其是客户端、服务端模式的程序，必须确保统一消息号
    event
*/

/*
    不同实例间传播
*/

/*
    事件相关，并非事件本体
*/
#define EVENT_T                 (u16)(7)
#define EVENT_MAIN_MAXN         (u16)(8)
#define EVENT_BRAN_MAXN         (u16)((65535 - 1000) / 7)

#define EVENT_Y                 (u16)(1)
#define EVENT_N                 (u16)(0)

/*
    具体事件
*/

#define EVENT_REQ               (u16)(1 + OSP_USEREVENT_BASE)           //1
#define EVENT_ACK               (u16)(2 + OSP_USEREVENT_BASE)           //2
#define EVENT_TERM              (u16)(3 + OSP_USEREVENT_BASE)           //3
#define EVENT_TIMEOUT           (u16)(4 + OSP_USEREVENT_BASE)           //4
                                                                        //5
                                                                        //6
                                                                        //7

#define EVENT_REQ_INSCONNECT           (u16)(EVENT_T*1 + EVENT_REQ)
#define EVENT_ACK_INSCONNECT           (u16)(EVENT_T*1 + EVENT_ACK)
#define EVENT_TERM_INSCONNECT          (u16)(EVENT_T*1 + EVENT_TERM)
#define EVENT_TIMEOUT_INSCONNECT       (u16)(EVENT_T*1 + EVENT_TIMEOUT)

#define EVENT_REQ_CATOTHERS            (u16)(EVENT_T*1 + EVENT_REQ)
#define EVENT_ACK_CATOTHERS            (u16)(EVENT_T*1 + EVENT_ACK)
#define EVENT_TERM_CATOTHERS           (u16)(EVENT_T*1 + EVENT_TERM)
#define EVENT_TIMEOUT_CATOTHERS        (u16)(EVENT_T*1 + EVENT_TIMEOUT)


/*
    用户自定义传播
*/




/*
	OSP应用程序之间需要遵循统一的APP编号规则：
	1、不同的APP有不同的APP编号，通过编号来指定APP；
	2、APP编号为非0的16位整形数；
	3、为了确保OSP消息通讯能够正常进行，所有OSP应用程序对于APP编号的约定必须一致；
	4、用户可以自定义APP编号，但要确保全局范围内APP号不产生冲突。通常有公用头文件来让用户自定义APP编号；
    appno
*/
/*
    server appid
*/
#define SRV_APP_NO     (u16)(2) 
/*
    APP优先级
*/
#define SRV_APP_PRI         100         /*APP优先级*/

/*
    最大等待消息数
*/
#define SRV_APP_QUE_SIZE    1000        /*最大等待消息数*/

/*
    APP任务栈的大小
*/
#define SRV_APP_STK_SIZE    (200<<10)  /*APP任务的栈大小*/

/*
    client  appid
*/
#define CLT_APP_NO     (u16)(5)

/*
    APP优先级
*/
#define CLT_APP_PRI         100         /*APP优先级*/

/*
    最大消息等待数
*/
#define CLT_APP_QUE_SIZE    1000        /*最大等待消息数*/

/*
    APP任务栈的大小
*/
#define CLT_APP_STK_SIZE    (200<<10)  /*APP任务的栈大小*/

/*
    实例状态的定义
    服务端status
*/

#define S_STATE_IDLE (u16)(0)
#define S_STATE_REQ  (u16)(1)
#define S_STATE_ACK  (u16)(2)
#define S_STATE_WORK (u16)(3)
#define S_STATE_TERM (u16)(4)

/*
    实例状态的定义
    客户端status
*/

#define C_STATE_IDLE        (u16)(0)
#define C_STATE_CONNECT     (u16)(1)
#define C_STATE_REQ         (u16)(2)
#define C_STATE_WORK        (u16)(3)
#define C_STATE_TERM        (u16)(4)

/*
    IP字串大小
*/
#define IPSTR_MAX 50

#define MAXINS 8

#endif //pubInfo.h