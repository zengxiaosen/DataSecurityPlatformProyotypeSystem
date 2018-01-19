#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fdfs_api.h"
#include "deal_mysql.h"
#include "make_log.h"
#include "upload.h"

char* memstr(char* full_data, int full_data_len, char* substr){
    if(full_data == NULL || full_data_len <= 0 || substr == NULL){
        return NULL;
    }
    if(*substr == '\0'){
        return NULL;
    }

    int sublen = strlen(substr);
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for(int i=0; i< last_possible; i++){
        if(*cur == *substr){
            if(memcmp(cur, substr, sublen) == 0){
                return cur;
            }
        }
        cur++;
    }
    return NULL;
}

int get_file_content(char* begin, char* end, int len, char* filename){
    char *p = NULL;
    //取出并保存分界线的字符串
    char boundary[256] = {0};
    //p指向该行的\r字符
    p = strstr(begin, "\r\n");
    strncpy(boundary, begin, p-begin);
    //p指向下一行行首
    p += 2
    //剩余字符串长度
    len -= (p-begin);
    printf("boundary: %s", boundary);

    //记录第二行行首位置
    begin = p;
    //content-disp, 保存到begin中
    p = strstr(begin, "\r\n");
    //移动到下一行行首，- 第三行
    p += 2
    //剩余数据长度
    len -= (p-begin);
    //将filename搞出来
    char* pt = strstr(begin, "filename=");
    pt += strlen("filename=");
    //指针指向文件名的第一个字符
    pt++;
    //找到文件爱你名的结尾字符
    char* q = strstr(pt, '"');
    //保存文件名
    strncpy(filename, pt, q-pt);
    printf("<br>filename:%s<br>", filename);

    // content-type 第三行
    begin = p;
    p = strstr(begin, "\r\n");
    //跳过\r\n和空行(\r\n)
    p += 4;
    //剩余数据长度
    len -= (p-begin);

    // 文件内容正是开始 - 图片正文开始
    begin = p;
    // 从剩余数据块中找到结束分界线
    p = memstr(begin, len, boundary);

    if(p == NULL){
        //减去\r\n长度
        p = end - 2;
    }

    else{
        //减去\r\n长度
        p -= 2;
    }

    //将上传文件内容写入文件中
    int fd = open(filename, O_CREAT|O_WRONLY, 0664);
    write(fd, begin, p-begin);
    close(fd);

    return 0;
}


int store_data(char* filename, char* fileid){
    //连接数据库
    MYSQL* conn = NULL;
    conn = mysql_conn("root", "root", "test");
    if(conn == NULL){
        LOG("upload_file", "mysql", "数据库链接失败!");
        return -1;
    }

    //设置数据库编码， 否则 中文插入乱码
    mysql_query(conn, "set names utf8");
    //插入数据
    //插入
    char buf[1024];
    sprintf(buf, "insert into file (name, filedid) values ('%s', '%s')", filename, fileid);
    printf("<br> sql: %s<br>\n", buf);
    //执行sql语句
    //如果执行成功， 它返回0
    if(mysql_query(conn, buf) != 0){
        LOG("upload_file", "mysql", "数据库插入失败");
        return -1;
    }

    //关闭数据库链接
    mysql_close(conn);
    return 0;
}

int main(){
    while(FCGI_Accept() >= 0){
        int len = 0;
        //pos数据的长度
        char *contentLength = getenv("CONTENT_LENGTH");
        printf("Content-type: text/html\r\n"
                                "\r\n");
        if(contentLength != NULL){
            len = strtol(contentLength, NULL, 10);
        }

        if(len <= 0){
            printf("No data from standard input.<p>\n");
        }

        else{
            int ch;
            char filename[128] = {0};
            //根据post数据长度，创建对应大小的数组
            char* file_data = (char*)malloc(len);
            char *begin, *end, *p;
            //指针初始化
            end = NULL;
            begin = p = file_data;

            //读post数据，存储到file_data中
            for(int i=0; i< len; i++){
                if((ch = getchar()) < 0){
                    printf("Error : Not enough bytes received on standard input <p>\n");
                    break;
                }
                *p = ch; // 赋值
                p++;
            }
            // end指向文件结束位置
            end = p;

            //得到文件内容
            get_file_content(begin, end, len, filename);

            //文件上传到fastdfs
            char fileid[1024] = {0};

            fdfs_upload_file(filename, fileid);
            printf("<br>fileid:%s\n<br>", fileid);

            //将数据存储到数据库中
            store_data(filename, fileid);

            //释放内存
            free(file_data);
            //删除文件
            unlink(filename);



        }
    }

    return 0;
}

