#ifndef _RFID_H_
#define _RFID_H_ 

#include <termios.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

int tty_init(int fd);
char get_bcc(int n,char *buf);
int rfid_send_A(int tty1);
int rfid_send_B(int tty1);
#endif