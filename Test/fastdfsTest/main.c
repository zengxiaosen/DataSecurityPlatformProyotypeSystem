#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fdfs_api.h"
#include "fdfs_upload_file.h"

int main(int argc, char* argv[])
{
    char fileid[1024] = {0};
    fdfs_upload_file("/etc/fdfs/client.conf", argv[1], fileid);
    printf("fileID = %s\n", fileid);

    char fileID[2014] = {0};
    upload_file2(argv[1], argv[2], fileid, sizeof(fileid));
    printf("fileID = %d\n", fileID);
    


    return 0;
}
