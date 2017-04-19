#ifndef _CLTINSTANCE_H_
#define _CLTINSTANCE_H_

#include "osp.h"
#include "pubInfo.h"
#include "client.h"




/*====================================================================
    ����Osp��Ӧ�ñ���Ӹ�CInstance��������һ�������Instance�࣬
    ������һ��InstanceEntry()��Ա�����ľ���ʵ��
	CInstance��һ��״̬��ʵ���Ļ��࣬�ṩ������һ��״̬������Ļ���������ӿڣ�
    ״̬��¼��״̬��Ǩ��
    ��ʱ����
    �¼�����Ϣ������ӿڣ�
    �����¼�����Ϣ���ӿڵȣ�
====================================================================*/

/*====================================================================
        �ͻ���ʵ��������
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
    Osp��Ϣ����һ����׼ֵ�ģ��û��ڸû�׼ֵ֮���趨�˼����»�׼ֵ��
Ҳ����˵������һ�������ҵ��������ֻ���ܳ����⼸�����ڻ�׼ֵ����Ϣ��
���磬��׼ֵ��REQ,ACK,WORK,TERM...������һ�������ҵ��������ֻ���ܳ�
���⼸���������Ϣ�����Կ����趨һ��������Ϣ����������ô���ڷ����ļ�
�������ҵ����˵����REQ_SENDFILE, ACK_SENDFILE...��Щ������Ϣ������Щ��
Ϣ�ڶ����ʱ���ǰ���׼��Ϣ��˳���һ��ģ���Ϊһ����Ϣ�ء�
    ��ôins���յ���Ϣ֮�󣬻���ݵ�ǰins��״̬ѡ�������Ϣ�ĺ�����ֻ
Ҫ������Ϣ�ż�ȥ��׼��Ϣ�ţ����Ի�׼��Ϣ�صĳ��ȣ��Ϳ��Եõ�����Ϣ�ض�
�ڻ�׼��Ϣ�ص�ƫ��������������Ϣ��λ�á�ͬ������Ϣ����ȡģ֮�󣬿��Ե�
������Ϣ����Ϣ���е�λ�á�
====================================================================*/
    u16 GetMain(u16);           //�õ���Ϣ����֧
    u16 GetBran(u16);           //�õ���Ϣ������֧

    void (CClientInstance::*IdleEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //����״̬������Ϣ������
    void (CClientInstance::*ConnectEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);        //����״̬������Ϣ������
    void (CClientInstance::*ReqEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);            //����״̬������Ϣ������
    void (CClientInstance::*WorkEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //����״̬������Ϣ������
    void (CClientInstance::*TermEventFunction[EVENT_MAIN_MAXN][EVENT_BRAN_MAXN])(CMessage *const pMsg);           //��ֹ״̬������Ϣ������

    void Idle_Function(CMessage *const pMsg);                   //����״̬������Ϣ���
    void Connect_Function(CMessage *const pMsg);                //����״̬������Ϣ���
    void Req_Function(CMessage *const pMsg);                    //����״̬������Ϣ���
    void Work_Function(CMessage *const pMsg);                   //����״̬������Ϣ���
    void Term_Function(CMessage *const pMsg);                   //��ֹ״̬������Ϣ���

/*====================================================================
        �����صľ��庯������
====================================================================*/

/*====================================================================
        ʵ�����ӣ�һ��һ�������
====================================================================*/
    void Idle_Req_InsConnect(CMessage *const pMsg);
    void Idle_Ack_InsConnect(CMessage *const pMsg);
    void Idle_Term_InsConnect(CMessage *const pMsg);
    void Idle_Timeout_InsConnect(CMessage *const pMsg);

/*====================================================================
        �鿴�����û�
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
        ��Ϣת��
====================================================================*/
    void Connect_Req_TransInfo(CMessage *const pMsg);
    void Connect_Ack_TransInfo(CMessage *const pMsg);
    void Connect_Term_TransInfo(CMessage *const pMsg);
    void Connect_Timeout_TransInfo(CMessage *const pMsg);

    void Req_Req_TransInfo(CMessage *const pMsg);
    void Req_Ack_TransInfo(CMessage *const pMsg);
    void Req_Term_TransInfo(CMessage *const pMsg);
    void Req_Timeout_TransInfo(CMessage *const pMsg);

    void Work_Ack_TransInfo(CMessage *const pMsg);
    void Work_Term_TransInfo(CMessage *const pMsg);

/*====================================================================
        �ļ��ϴ�
====================================================================*/
    void Req_Req_SendFile(CMessage *const pMsg);
    void Req_Ack_SendFile(CMessage *const pMsg);
    void Req_Term_SendFile(CMessage *const pMsg);
    void Req_Timeout_SendFile(CMessage *const pMsg);

    void Work_Ack_SendFile(CMessage *const pMsg);
    void Work_Term_SendFile(CMessage *const pMsg);

/*====================================================================
        ������Ϣ����
====================================================================*/
    void Req_Req_SendChar(CMessage *const pMsg);
    void Req_Ack_SendChar(CMessage *const pMsg);
    void Req_Term_SendChar(CMessage *const pMsg);
    void Req_Timeout_SendChar(CMessage *const pMsg);

    void Work_Ack_SendChar(CMessage *const pMsg);
    void Work_Term_SendChar(CMessage *const pMsg);
/*====================================================================
        �û��˳����Ͽ�����
====================================================================*/
    //void Connect_Ack_DisConncet(CMessage *const pMsg);

    void Req_Req_DisConnect(CMessage *const pMsg);
    void Req_Ack_DisConnect(CMessage *const pMsg);
    void Req_Term_DisConnect(CMessage *const pMsg);
    void Req_Timeout_DisConnect(CMessage *const pMsg);
    
    

/*====================================================================
        ���ҵ��ľ��崦��
====================================================================*/
private:
    void sendFile(CMessage *const pMsg);
    void sendChar(CMessage *const pMsg);
};


#endif //cltInstance.h