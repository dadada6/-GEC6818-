#include "ts.h"

static int ts_fd;
int ts_x = 0;
int ts_y = 0;

//1、打开触摸屏文件
int open_ts(void)
{
	ts_fd = open("/dev/input/event0", O_RDONLY);
	if (ts_fd == -1)
	{
		printf("open ts error!\n");
		return -1;
	}
	return 0;
}

//2、读取触摸屏的xy数据
int read_ts_xy(void)
{	
	struct input_event ts_buf;
	int count = 0;

	while(1)
	{
		read(ts_fd, &ts_buf, sizeof(ts_buf)); //scanf();
		//x轴坐标
		if(ts_buf.type==EV_ABS && ts_buf.code==ABS_X)
		{
			count++;
			//ts_x = ts_buf.value/1024.0*800;//黑色板
			ts_x = ts_buf.value;//蓝色板
			//printf("x = %d\n",ts_buf.value);
		}
		//y轴坐标
		if(ts_buf.type==EV_ABS && ts_buf.code==ABS_Y)
		{
			count++;
			//ts_y = ts_buf.value/600.0*480;//黑色板
			ts_y = ts_buf.value;//蓝色板		
		}
		if (count == 2)
		{
			printf("(%d, %d)\n", ts_x, ts_y);
			count = 0;
			break;
		}
	}
	
	return 0;
}

//3、关闭触摸屏文件
int close_ts(void)
{
	close(ts_fd);
	return 0;
}

