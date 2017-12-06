#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "keymng_msg.h"

int main(){
	int ret = 0;
	Teacher t1;
	unsigned char *out = NULL;
	int outlen = 0;
	Teacher *outPstruct = NULL;
	int iType = 0;
	memset(&t1, 0, sizeof(Teacher));
	strcpy(t1.name, "myname");
	t1.age = 32;
	t1.p = (char *)malloc(100);
	strcpy(t1.p, "aaaaaa");
	t1.plen = 6;

	ret = MsgEncode((void *)&t1, ID_MsgType_Teacher, &out, &outlen);
	if(ret != 0){
		
	}
}