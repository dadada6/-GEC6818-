#include "color.h"
#include "ts.h"
#include "bmp.h"
#include "lcd.h"
#include <stdio.h>
#include "jpeglib.h"
#include "yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>
#include "lcd.h"
#include "pthread.h"
#include "wifi.h"
#include "rfid.h"

struct jpg_data video_buf;
int flag=0;
int i=0;
int camera_init_flag=0;

int writeBinFile(char* name,char* buf, long nSize)
{
    FILE *fp = fopen(name,"wb");
    if (fp == NULL)
    {
        printf("无效文件句柄\n");
        return -1;
    }
    fwrite(buf,nSize,1,fp);
    fclose(fp);
    return 0;
}
void *touchscreen(){
	while(1){
		printf("enter to ts thread\n");
		printf("flag:%d\n",flag);
		flag=1;
		read_ts_xy();
		usleep(100);
	}
}

int rfid()
{
	int cardid=0;
	//打开串口1
	int tty1 = open("/dev/ttySAC1",O_RDWR|O_NOCTTY);
	if (tty1 == -1)
	{
		perror("open tty1 failed");
		return -1;
	}

	tty_init(tty1);

	rfid_send_A(tty1);

	cardid = rfid_send_B(tty1);

	printf("%#x\n", cardid);

	 if( cardid ==0x4f2bea67)
	{
		printf("card successful\n");
		return 1;
	}
	else
	{
		printf("card failed\n");
	}



	return 0;
}
int main(int argc, char const *argv[])
{
	//1、打开相关的文件
	open_lcd_color();
	open_lcd_bmp();
    open_ts();
	lcd_open();
	mmap_lcd();
	linux_v4l2_yuyv_init("/dev/video7");
	pthread_t thread;
	pthread_create(&thread, NULL, touchscreen, NULL);

    pthread_t tid5;
	pthread_create(&tid5,NULL,myrecvmsg,NULL);	//接收从手机Qt端发送的数据
	addrsize=sizeof(phoneaddr);
	udp_init();	
    show_bmp("./login.bmp", 0, 0, 800, 480);
		//rfid();
    
    while(1)
	{


			if(ts_x>320 && ts_x<440 && ts_y>250 && ts_y<375)
			{
	 			
	                rfid();
	        		show_bmp("./main.bmp", 0, 0, 800, 480);
	 				
	 		}


 		
	//2、使用相关的文件
	//show_bmp("./main.bmp", 0, 0, 800, 480);
	
			


			//1、广告屏幕
			if(ts_x>295 && ts_x<478 && ts_y>128 && ts_y<203)
			{
				printf("enter to adv\n");
				show_bmp("./adv.bmp", 0, 0, 800, 480);
				while(1)
				{
					//1、播放视频
					if(ts_x>650 && ts_x<740 && ts_y>149 && ts_y<180)
					{
						printf("-------播放视频-----------\n");
						system("madplay music1.mp3 &");
						system("mplayer -slave -quiet -geometry 0:0 -zoom -x 600 -y 400 1.mp4 &");
					}

					//2、结束播放
					if(ts_x>623 && ts_x<779 && ts_y>183 && ts_y<230)
					{
						system("killall -9 madplay");
						system("killall -9 mplayer");

						show_bmp("./adv.bmp", 0, 0, 800, 480);
					}

					//3、切换视频
					if(ts_x>623 && ts_x<779 && ts_y>233 && ts_y<264)
					{
						system("killall -9 mplayer");
						show_bmp("./adv.bmp", 0, 0, 800, 480);
						show_bmp("./switch-video.bmp", 140, 130, 320, 240);
						while(1){
							if(ts_x>221 && ts_x<376 && ts_y>200 && ts_y<240)
							{
								show_bmp("./adv.bmp", 0, 0, 800, 480);
								system("mplayer -slave -quiet -geometry 0:0 -zoom -x 600 -y 400 1.mp4 &");
								break;
							}
							if(ts_x>221 && ts_x<376 && ts_y>268 && ts_y<309)
							{
								show_bmp("./adv.bmp", 0, 0, 800, 480);
								system("mplayer -slave -quiet -geometry 0:0 -zoom -x 600 -y 400 2.mp4 &");
								break;
							}		
						}
					}
					//4、开启字幕
					if(ts_x>623 && ts_x<779 && ts_y>291 && ts_y<329)
					{
						show_bmp("./mask.bmp", 0, 400, 800, 80);
						system("./freetype test2.ttf");
					}
					//5、关闭字幕
					if(ts_x>612 && ts_x<789 && ts_y>341 && ts_y<400)
					{

						system("killall -9 madplay");
						show_bmp("./mask.bmp", 0, 400, 800, 80);
						system("killall -9 freetype");
						break;
					}


	                //音乐
				//	if(ts_x>650 && ts_x<790 && ts_y>425 && ts_y<490)
				//	{
					//	system("madplay music1.mp3 &");
						//printf("enter to broadcast\n");
						//printf("-------播放视频-----------\n");
						//system("mplayer -slave -quiet -geometry 0:0 -zoom -x 600 -y 400 1.mp4 &");
					//}




					//6、返回主界面
					if(ts_x>612 && ts_x<789 && ts_y>5 && ts_y<107)
					{
						system("killall -9 mplayer");
						show_bmp("./main.bmp", 0, 0, 800, 480);
						break;
					}

				}
			}
		


		
		//2、摄像头
		if(ts_x>230 && ts_x<550 && ts_y>210 && ts_y<330)
		{
			printf("enter to camera\n");
			show_bmp("./camera.bmp", 0, 0, 800, 480);
			while(1)
			{
				//1、录像
				if(ts_x>600 && ts_x<890 && ts_y>102 && ts_y<189)
				{
					printf("-------Record-----------\n");
				}

				//2、拍照
				if(ts_x>600 && ts_x<890 && ts_y>102 && ts_y<189)
				{
					//初始化摄像头
					//开启摄像头捕捉
					if(camera_init_flag==0)
					{
						linux_v4l2_start_yuyv_capturing();
						camera_init_flag=1;
					}

					flag=0;
					while(1)
					{

						linux_v4l2_get_yuyv_data (&video_buf);
						
						show_video_data(0, 0, video_buf.jpg_data , video_buf.jpg_size);
						//将摄像头数据发送到手机端
						sendto(iphonex,video_buf.jpg_data,video_buf.jpg_size,0,(struct sockaddr *)&phoneaddr,sizeof(phoneaddr));
	

						printf("-------shot-----------\n");
						printf("flag:%d",flag);
						if(flag!=0)
						{
							show_bmp("./camera.bmp", 0, 0, 800, 480);
							writeBinFile("./shot.jpg",video_buf.jpg_data,video_buf.jpg_size);

							
					       break;
						}
					//	if(flag<1){
						//	//show_bmp("./camera.bmp", 0, 0, 800, 480);
						//	writeBinFile("./shot.jpg2",video_buf.jpg_data,video_buf.jpg_size);
							

									//	}

                    } 

                 }

					if(ts_x>650 && ts_x<860 && ts_y>390 && ts_y<480)
					{
						//if(i!=0){

						
						lcd_draw_jpg(0,0,"./shot.jpg");
						//show_bmp("./shot.jpg", 0, 0, 800, 480);
						//break;
					}
  
					   
				       

				//3、返回主界面
					if(ts_x>650 && ts_x<860 && ts_y>1 && ts_y<100)
					{
						//printf("return to exit");
						system("killall -9 mplayer");
						show_bmp("./main.bmp", 0, 0, 800, 480);
						break;
					}
                
           }//w
			 
				
			
					//if(i<1){
					//lcd_draw_jpg(0,0,"./shot.jpg2");}	
					
					
			
		}//if

		//3、广播
	//	if(ts_x>295 && ts_x<478 && ts_y>332 && ts_y<409)
	//	{
	//		printf("enter to broadcast\n");
	//	}


	}
 			
	

	//3、关闭相关的文件
	close_lcd_color(); 
	close_ts();
	close_lcd_bmp();

	return 0;

}