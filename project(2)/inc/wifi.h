#ifndef _WIFI_H
#define _WIFI_H

#include <stdio.h>
#include "jpeglib.h"
#include "yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int udp_init();

void *myrecvmsg(void *arg);


extern struct sockaddr_in phoneaddr ;

extern int iphonex;


extern int addrsize ;

#endif /* wifi.h */