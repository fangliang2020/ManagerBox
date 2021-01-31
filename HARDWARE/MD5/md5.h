/**
*  @file     : md5.h
*  @Author   : Inhand Embedded Team
*  @brief    : 
*  
*  
*  @Copyright: InHand Networks Co., Ltd.
*  @Website  : www.inhand.com.cn
*  Modification history :
*  Date           Author          Notes
*  2020-03-13     zhaocj          the first version
**/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __MD5_H__
#define __MD5_H__
/* Includes -----------------------------------------------------------------*/
#include <system.h>
/* Exported types -----------------------------------------------------------*/
struct md5_ctx
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
};

typedef struct md5_ctx *md5_ctx_t;
/* Exported constants -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/

/* Exported functions -------------------------------------------------------*/
void MD5Init(md5_ctx_t context);
void MD5Update(md5_ctx_t context,unsigned char *input,unsigned int inputlen);
void MD5Final(md5_ctx_t context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);

#endif /*__MD5_H__*/
/* End of file****************************************************************/

