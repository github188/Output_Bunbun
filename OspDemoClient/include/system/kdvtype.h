/*****************************************************************************
   模块名      : KDV type 
   文件名      : kdvtype.h
   相关文件    : 
   文件实现功能: KDV宏定义
   作者        : 魏治兵
   版本        : V3.0  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2004/02/17  3.0         魏治兵        创建
   2012/08/21  4.0         曾昭鸣        修正相关定义
   2014/07/15  5.0         曾昭鸣        修正s8定义为signed char
******************************************************************************/
#ifndef _KDV_TYPE_H_
#define _KDV_TYPE_H_

#ifndef _MSC_VER
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	  /* Type definition */
/*-----------------------------------------------------------------------------
系统公用文件，开发人员严禁修改
------------------------------------------------------------------------------*/

/* if using visual studio compiler */
#if defined(_MSC_VER)
/* "s8 = signed char" is the right choice, but our apps heavily use s8 as char,
   define as this will cause type conversion compile errors.
   So give up to set "s8 = char".

   This alias is fault in Linux ARM platform, where char == unsigned char
*/
//typedef signed char     s8;
#define s8              char
typedef unsigned char   u8;
typedef signed short    s16;
typedef unsigned short  u16;
typedef signed int      s32;
typedef unsigned int    u32;
typedef __int64         s64;
typedef unsigned __int64 u64;

/* if using gcc, xcoder, intel or other C99 support compiler */
#else
#define s8              char
/* see comments above */
//typedef int8_t          s8;
typedef uint8_t         u8;
typedef int16_t         s16;
typedef uint16_t        u16;
typedef int32_t         s32;
typedef uint32_t        u32;
typedef int64_t         s64;
typedef uint64_t        u64;
#endif // end of _MSC_VER
typedef char            kdvchar;
typedef s32             BOOL32;

#ifndef _MSC_VER
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _KDV_def_H_ */

/* end of file kdvdef.h */
