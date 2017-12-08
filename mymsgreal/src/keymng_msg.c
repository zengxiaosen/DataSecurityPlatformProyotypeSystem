#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "keymng_msg.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"

//encode
int Teacher_Encode_(Teacher *pstruct, ITCAST_ANYBUF **tmpNode){
	int ret = 0;
	ITCAST_ANYBUF *pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF *pTmpBuf = NULL;
	ITCAST_ANYBUF *pOutData = NULL;

	unsigned char *tmpOut = NULL;
	int tmpOutLen = 0;

	//encode name
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->name, strlen(pstruct->name));
	if(ret != 0){
		printf("func DER_ITCAST_String_To_AnyBuf() err:%d\n", ret);
		return ret;
	}

	//TLV for name
	ret = DER_ItAsn1_WritePrintableString(pTmp, &pHeadBuf);
	if(ret != 0){
		DER_ITCAST_FreeQueue(pTmp);
		printf("func DER_ItAsn1_WritePrintableString() err:%d\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pHeadBuf;

	//encode age
	ret = DER_ItAsn1_WriteInteger(pstruct->age, &(pTmpBuf->next));
	if(ret != 0){
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}
	pTmpBuf = pTmpBuf->next;

	
}