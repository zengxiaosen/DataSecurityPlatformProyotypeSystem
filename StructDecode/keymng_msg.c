

#include "keymng_msg.h"


int MsgEncode(
	void			*pStruct, /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen)
{
	// itype tlv

	//teacher tlv sequence

	//all sequence tlv
	return 0;
}



int MsgDecode(
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/)
{
	return 0;
}



int MsgMemFree(void **point, int type)
{
	return 0;
}