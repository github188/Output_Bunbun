#ifndef _CLIENT_H_
#define _CLIENT_H_
#endif
/*重要的全局变量*/



/*用户操作指令*/
#define INVALID         1
#define CAT_OLCLT       2
#define TRAN_SIG_INFO   3
#define TRAN_ALL_INFO   4
#define SEND_FILE       5
#define SEND_INFO       6
#define QUIT            7



/*用户操作界面*/
void UserInterface();



/*用户指令实现*/

/*
    1.保留
*/



/*
    2.查看其他在线客户
*/
void CatOlCltFun(u32 cmd);





