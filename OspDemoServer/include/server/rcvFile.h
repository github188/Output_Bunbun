#ifndef _READFILE_H_
#define _READFILE_H_
#include "srvInstance.h"



s8 READBUFF[BUFFSIZE];

FILE *fp = NULL;
FILE *fpRcd = NULL;
void createUserFile(s8 [], s8[]);

s8 pFilePath[MAXFILENAME];
s8 pRcdFilePath[MAXFILENAME];

CFileMessage *pFMsg = NULL;

#endif// readFile.h