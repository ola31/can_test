#ifndef CAN_OLA_H
#define CAN_OLA_H

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
//#include <unistd.h>


extern int open_port(const char *port);
extern int send_port(struct can_frame *frame);
extern void read_port();
extern int close_port();


#endif // CAN_OLA_H
