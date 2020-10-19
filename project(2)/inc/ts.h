#ifndef _TS_H
#define _TS_H

//printf、scanf函数
#include <stdio.h>
//open、lseek函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//read、close、write、lseek函数
#include <unistd.h>
//ts的头文件
#include <linux/input.h>

//1、打开触摸屏文件
int open_ts(void);

//2、读取触摸屏的xy数据
int read_ts_xy(void);

//3、关闭触摸屏文件
int close_ts(void);

extern int ts_x;
extern int ts_y;

#endif /* ts.h */