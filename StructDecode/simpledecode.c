#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"

typedef struct _Teacher{
	char name[64];
	int age;
	char *p;
	int plen;
}Teacher;

int mywritefile(unsigned char *buf, int len){
	FILE *fp = NULL;
	fp = open("./teacher.ber", "wb+");
	if(fp == NULL){
		printf("fopen file error \n");
		return -1;
	}
	fwrite(buf, 1, len, fp);
	fclose(fp);
	return 0;
}

//bao wen bian ma
int Teacher_Encode(Teacher *pstruct, unsigned char **p, int *len){
	int ret = 0;
	ITCAST_ANYBUF *pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF *pTmpBuf = NULL;
	ITCAST_ANYBUF *pOutData = NULL;
	unsigned char *tmpOut = NULL;
	int tmpOutLen = 0;

	//encode name
	//char of c transform to buf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->name, strlen(pstruct->name));
	if(ret != 0){
		printf("func DER_ITCAST_String_To_AnyBuf() err:%d\n", ret);
		return ret;
	}
	//do TLV operation of name
	ret = DER_ItAsn1_WritePrintableString(pTmp, &pHeadBuf);
	if(ret != 0){
		DER_ITCAST_FreeQueue(pTmp);
		printf("func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}

	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pHeadBuf;

	//encode age
	ret = DER_ItAsn1_WriteInteger(pstruct->age, &(pTmpBuf->next));
	if(ret != 0){
		DER_ITCAST_FreeQueue(pHeadBuf); //
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}
	pTmpBuf = pTmpBuf->next;

	//encode p
	ret = EncodeChar(pstruct->p, pstruct->plen, &pTmpBuf->next);
	if(ret != 0){
		printf("func EncodeChar() err:%d \n", ret);
		return ret;
	}

	pTmpBuf = pTmpBuf->next;
	//encode plen
	ret = DER_ItAsn1_WriteInteger(pstruct->plen, &(pTmpBuf->next));
	if(ret != 0){
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}

	//TLV struct
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if(ret != 0){
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}

	DER_ITCAST_FreeQueue(pHeadBuf);
	//assign mem
	tmpOut = (unsigned char *)malloc(pOutData->dataLen);
	memcpy(tmpOut, pOutData->pData, pOutData->dataLen);

	*p = tmpOut;
	*len = pOutData->dataLen;

	DER_ITCAST_FreeQueue(pOutData);
	return ret;
}

int Teacher_Decode(unsigned char *p, int len, Teacher **pstruct){
	int ret = 0;
	ITCAST_ANYBUF *pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF *pTmpDABuf = NULL;
	ITCAST_ANYBUF *pOutData = NULL;
	ITCAST_ANYBUF *inAnyBuf = NULL;
	int tmpNum = 0;
	Teacher *pTmpStru = NULL;

	//*pTmpStru = (Teacher *)malloc(sizeof(Teacher));
	inAnyBuf = (ITCAST_ANYBUF *)malloc(sizeof(ITCAST_ANYBUF));
	if(inAnyBuf == NULL){
		ret = 1;
		printf("func Teacher_Decode() err: %d. malloc err \n", ret);
		return ret;
	}

	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF)); // fuzhi 
	inAnyBuf->pData = (unsigned char *)malloc(len);
	if(inAnyBuf->pData == NULL){
		ret = 2;
		DER_ITCAST_FreeQueue(inAnyBuf);
		printf("func Teacher_Decode() err: %d. malloc err \n", ret);
		return ret;
	}
	inAnyBuf->dataLen = len;
	memcpy(inAnyBuf->pData, p, len);

	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if(ret != 0){
		DER_ITCAST_FreeQueue(inAnyBuf);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}

	DER_ITCAST_FreeQueue(inAnyBuf);
	pTmp = pHead;
	pTmpStru


}

int main(){
	int ret = 0;
	Teacher t1;

	unsigned char *out = NULL;
	int outLen = 0;
	Teacher *outPstruct = NULL;

	memset(&t1, 0, sizeof(Teacher));
	strcpy(t1.name, "myname");
	t1.age = 32;
	t1.p = (char *)malloc(100);
	strcpy(t1.p, "aaaaaa");
	t1.plen = 6;

	ret = Teacher_Encode(&t1, &out, &outLen);
	if(ret != 0){
		printf("func Teacher_Encode() err:%d \n", ret);
		return ret;
	}

	ret = Teacher_Decode(out, outLen, &outPstruct);
	if(ret != 0){
		printf("func Teacher_Decode() err:%d \n", ret);
		return ret;
	}

	if(t1.age != outPstruct->age || (memcmp(t1.p, outPstruct->p, t1.plen) != 0) ){
		printf("the encode data is different with the decode data! \n");
		return -2
	}else{
		printf("the encode data is same with the decode data! \n");
	}



	return 0;

}