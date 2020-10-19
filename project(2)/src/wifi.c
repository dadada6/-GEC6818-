#include "wifi.h"

int iphonex;

struct sockaddr_in phoneaddr ;
struct sockaddr_in bindaddr;


int addrsize ;
//接收从手机Qt端发送的数据
void *myrecvmsg(void *arg)
{
	char buf[200];
	addrsize=sizeof(phoneaddr);
	while(1)
	{
		usleep(1000);
		bzero(buf,200);//清零
		//聊天，收发数据，接收手机发送过来的控制命令
		recvfrom(iphonex,buf,200,0,(struct sockaddr *)&phoneaddr,&addrsize);
		printf("iphoneX send msg is:%s ip:%s  port:%hu\n",buf,inet_ntoa(phoneaddr.sin_addr),ntohs(phoneaddr.sin_port));
		//打印收到的命令
		if(strcmp(buf,"MUSIC_PLAY")==0)//字符串的比较
		{
			printf("PLAY\n");
		}
		
	}
}


int udp_init()
{
	
	int ret;
	//初始化该结构体
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=inet_addr("192.168.1.100");////开发板无线网卡的ip地址
	bindaddr.sin_port=htons(2234);//端口号也需要转换

	//买手机---》创建一个udp套接字(数据报套接字)用于通信
	iphonex=socket(AF_INET,SOCK_DGRAM,0);
	if(iphonex==-1)
	{
		printf("create socket failed!\n");
		return -1;
	}
	//绑定 ---》自己ip和端口号   
	ret=bind(iphonex,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		printf("bind failed!\n");
		return -1;
	}

	return 0;
}