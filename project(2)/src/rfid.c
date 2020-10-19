#include <termios.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

char get_bcc(int n,char *buf);
int tty_init(int fd);
int rfid_send_A(int tty1);
int rfid_send_B(int tty1);


int buf[3] = {0, 0x4de6ebf7, 0x1c0c349b};

//串口初始化
int tty_init(int fd)
{	
	//1、定义串口配置结构体
	struct termios new_cfg;
	//2、激活串口设置	
	cfmakeraw(&new_cfg);
	//3、本地连接和接收使能
	new_cfg.c_cflag|=CLOCAL|CREAD;
	//4、设置波特率输入输出都为115200
	cfsetispeed(&new_cfg, B9600);
 	cfsetospeed(&new_cfg, B9600);
 	//5、用数据位掩码清空数据位设置，设置为8个数据位
	new_cfg.c_cflag&=~CSIZE; 
	new_cfg.c_cflag|=CS8;	
	//6、无奇偶校验位
	new_cfg.c_cflag&=~PARENB;
	//7、设置一个停止位
	new_cfg.c_cflag&=~CSTOPB;
	//8、设置等待时间或者获取几个字符后退出read的阻塞状态
	new_cfg.c_cc[VTIME]=0;
	new_cfg.c_cc[VMIN]=1;
	//9、清空串口缓冲区
	tcflush(fd, TCIFLUSH);
	 //10、配置激活
	tcsetattr(fd,TCSANOW,&new_cfg);	 

	return 0;
}

char get_bcc(int n,char *buf)
{
	int i;
	char BCC = 0;
	for(i=0; i<n; i++) 
	{
		BCC ^= buf[i];
	}
	return  (~BCC);	
}




//获取周围所有的卡，请求,参数fd是串口的描述符
int rfid_send_A(int tty1)
{
	char rbuf[8]={0};
	char wbuf[8]={0};
	wbuf[0]=0x07;	//数据帧长度
	wbuf[1]=0x02; 	//使用ISO14443A协议
	wbuf[2]= 0x41;	//使用A命令
	wbuf[3]=0x01;	//数据长度
	wbuf[4]=0x52;	//选择所有
	wbuf[5]=get_bcc(wbuf[0]-2,wbuf);	//校验位
	wbuf[6]=0x03;	//结束符

	while(1)
	{
		//清空缓冲区
		tcflush(tty1, TCIFLUSH);
		write(tty1,wbuf,7);
		rbuf[2]=-1;
		read(tty1,rbuf,sizeof(rbuf));
		//判断rfid模块返回数据是否正确
		if (rbuf[2]==0)
		{
			printf("send a ok!\n");
			break;
		}
		else
		{
			sleep(1);
			printf("No card information around!\n");		
		}
	}	

	return 0;
}

int rfid_send_B(int tty1)
{
	int cardid=0;
	char qbuf[10]={0};
	char wbuf[8]={0};

	//防碰撞（获取卡号信息），
	wbuf[0]=0x08;	//数据帧长度
	wbuf[1]=0x02; 	//使用ISO14443A协议
	wbuf[2]= 'B';	//使用B命令
	wbuf[3]=0x02;	//数据长度
	wbuf[4]=0x93;	//第一级防碰撞
	wbuf[5]=0x00; //位计数为0
	wbuf[6]=get_bcc(wbuf[0]-2,wbuf);	//校验位
	wbuf[7]=0x03;	//结束符

	while(1)
	{

		tcflush(tty1, TCIFLUSH);
		write(tty1,wbuf,8);
		qbuf[2]=-1;
		read(tty1,qbuf,sizeof(qbuf));//scanf
		if (qbuf[2]==0)
		{
			printf("send b ok!\n");
			cardid=qbuf[4]<<0 | qbuf[5]<<8 | qbuf[6]<<16 | qbuf[7]<<24 ;
			return cardid;
		}
		else
		{
			printf("faield\n");
			sleep(1);
		}

	}	

	return 0;
}


