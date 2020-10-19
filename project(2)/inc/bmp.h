#ifndef _BMP_H
#define _BMP_H

//printf、scanf函数
#include <stdio.h>
//open、lseek函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//read、close、write、lseek函数
#include <unistd.h>
//mmap函数
#include <sys/mman.h>

//1、打开lcd屏幕文件、申请映射内存
int open_lcd_bmp(void);

//2、显示任意位置大小bmp图片
int show_bmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y);

//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_bmp(void);
#endif