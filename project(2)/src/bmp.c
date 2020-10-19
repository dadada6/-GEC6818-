#include "bmp.h"

static int lcd_fd;
static unsigned int *lcd_p;
static unsigned int *lcd_p_flag;

//1、打开lcd屏幕文件、申请映射内存
int open_lcd_bmp(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open lcd error!\n");
		return -2;
	}
	//b、写1.bmp图片数据到lcd屏幕上
	//write(lcd_fd, lcd_buf , 800*480*4);
	lcd_p  =	mmap(	
	 					NULL, 					//默认为NULL,让系统自动分配,首地址
						800*480*4,				//映射空间的大小，也就是开发板现存
				 		PROT_READ|PROT_WRITE,	//允许进行读写操作
				 		MAP_SHARED,				//此内存空间被共享
				 		lcd_fd, 				//lcd的文件描述符
				 		0						//默认为0偏移
					);

	lcd_p_flag = lcd_p;  //让lcd_p_flag记住lcd_p的初始位置
	return 0;

}

//2、显示任意位置大小bmp图片
int show_bmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y)
{
	//1、第一阶段：读取到1.bmp图片的数据
	//a、打开1.bmp图片的文件
	int bmp_fd = open(bmp_path, O_RDONLY);
	if (bmp_fd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	//b、lseek函数跳过54个头信息
	lseek(bmp_fd, 54, SEEK_SET);
	//c、读取1.bmp图片的数据
	char bmp_buf[bmp_x*bmp_y*3];
	read(bmp_fd, bmp_buf, bmp_x*bmp_y*3);
	//d、关闭1.bmp图片的文件
	close(bmp_fd);
		

	//2、第二阶段：
	//800*480*3的bmp图片数据转换成
	//800*480*4的lcd显示数据
	int lcd_buf[bmp_x*bmp_y];
	int i;
	for(i=0; i<bmp_x*bmp_y; i++)
	{
		lcd_buf[i] = (bmp_buf[3*i]<<0) + (bmp_buf[3*i+1]<<8) + (bmp_buf[3*i+2]<<16) + (0x00<<24);
		             //     B          +        G            +          R           +   人为的添加一位
	}

	//3、第三阶段：写入1.bmp图片的数据到lcd屏幕
	//c、调节图片开始刷的位置
	lcd_p = lcd_p + lcd_x;		//x轴
	lcd_p = lcd_p + lcd_y*800;	//y轴

	int x,y;
	for(x=0; x<bmp_x; x++)
	{
		for(y=0; y<bmp_y; y++)
		{
			*(lcd_p+y*800+x) = lcd_buf[(bmp_y-1-y)*bmp_x+x];
		}
	}

	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}


//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_bmp(void)
{
	
	munmap(lcd_p, 800*480*4);
	close(lcd_fd);

}