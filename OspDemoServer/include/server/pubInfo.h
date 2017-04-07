#ifndef _PUBINFO_H_
#define _PUBINFO_H_
#endif

/*
    OSP����Ϣ������16�ֽ����Σ�OSP�û��������Ϣ����Ӧ�ô��ڸû�׼ֵ��
    �û�׼ֵ֮����OSP������Ϣ��
    #define   OSP_USEREVENT_BASE		(u16)0x0400
    �����ǿͻ��ˡ������ģʽ�ĳ��򣬱���ȷ��ͳһ��Ϣ��
    event
*/

/*
    ��ͬʵ���䴫��
*/

/*
    �¼���أ������¼�����
*/
#define EVENT_T                 (u16)(7)
#define EVENT_MAIN_MAXN         (u16)(8)
#define EVENT_BRAN_MAXN         (u16)((65535 - 1000) / 7)

#define EVENT_Y                 (u16)(1)
#define EVENT_N                 (u16)(0)

/*
    �����¼���   
*/

#define EVENT_REQ               (u16)(1 + OSP_USEREVENT_BASE)
#define EVENT_ACK               (u16)(2 + OSP_USEREVENT_BASE)
#define EVENT_TERM              (u16)(3 + OSP_USEREVENT_BASE)
#define EVENT_TIMEOUT           (u16)(4 + OSP_USEREVENT_BASE)


#define EVENT_REQ_INSCONNECT           (u16)(EVENT_T*1 + EVENT_REQ)
#define EVENT_ACK_INSCONNECT           (u16)(EVENT_T*1 + EVENT_ACK)



/*
    �û��Զ��崫��
*/




/*
	OSPӦ�ó���֮����Ҫ��ѭͳһ��APP��Ź���
	1����ͬ��APP�в�ͬ��APP��ţ�ͨ�������ָ��APP��
	2��APP���Ϊ��0��16λ��������
	3��Ϊ��ȷ��OSP��ϢͨѶ�ܹ��������У�����OSPӦ�ó������APP��ŵ�Լ������һ�£�
	4���û������Զ���APP��ţ���Ҫȷ��ȫ�ַ�Χ��APP�Ų�������ͻ��ͨ���й���ͷ�ļ������û��Զ���APP��ţ�
    appno
*/
/*
    server appid
*/
#define SRV_APP_NO     (u16)(2) 
/*
    APP���ȼ�
*/
#define SRV_APP_PRI         100         /*APP���ȼ�*/

/*
    ���ȴ���Ϣ��
*/
#define SRV_APP_QUE_SIZE    1000        /*���ȴ���Ϣ��*/

/*
    APP����ջ�Ĵ�С
*/
#define SRV_APP_STK_SIZE    (200<<10)  /*APP�����ջ��С*/

/*
    client  appid
*/
#define CLT_APP_NO     (u16)(5)

/*
    APP���ȼ�
*/
#define CLT_APP_PRI         100         /*APP���ȼ�*/

/*
    �����Ϣ�ȴ���
*/
#define CLT_APP_QUE_SIZE    1000        /*���ȴ���Ϣ��*/

/*
    APP����ջ�Ĵ�С
*/
#define CLT_APP_STK_SIZE    (200<<10)  /*APP�����ջ��С*/

/*
    ʵ��״̬�Ķ���
    �����status
*/

#define S_STATE_IDLE (u16)(0)
#define S_STATE_REQ  (u16)(1)
#define S_STATE_ACK  (u16)(2)
#define S_STATE_WORK (u16)(3)
#define S_STATE_TERM (u16)(4)

/*
    ʵ��״̬�Ķ���
    �ͻ���status
*/

#define C_STATE_IDLE        (u16)(0)
#define C_STATE_CONNECT     (u16)(1)
#define C_STATE_REQ         (u16)(2)
#define C_STATE_WORK        (u16)(3)
#define C_STATE_TERM        (u16)(4)

/*
    IP�ִ���С
*/
#define IPSTR_MAX 50