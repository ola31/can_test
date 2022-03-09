#ifndef CAN_OLA_H
#define CAN_OLA_H

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

typedef unsigned char  BYTE;  //8bit
typedef unsigned short BYTE2; //16bit
typedef unsigned int BYTE4;   //32bit


/**
 * struct can_frame - basic CAN frame structure
 * @can_id:  the CAN ID of the frame and CAN_*_FLAG flags, see above.
 * @can_dlc: the data length field of the CAN frame
 * @data:    the CAN frame payload.
 */
/// https://sites.uclouvain.be/SystInfo/usr/include/linux/can.h.html

/**
 *struct can_filter
 * @can_id
 * @can_mask
 */

enum Bit_rate {
  _10k = 0,
  _20k,
  _50k,
  _100k,
  _125k,
  _250k,
  _500k,
  _750k,
  _1M
};

class CAN
{

  private:
    int soc;
    int read_can_port;
    string port_name;        //socket CAN port name. such as "can0"
    string device_name;  //Port of USB_to_CAN device port such as "ttyACM0". Can find it in "/dev" directory.

  private:
    bool is_filter_has_id = false;

    struct can_filter *filter_list;

  public:
    CAN(string port_name_, string device_name_);
    ~CAN();

    int open_port(const char *port);
    int send_port(struct can_frame *frame);
    void read_port();
    int close_port();

    void CAN_initialize(int bit_rate_mode);
    void set_can_frame(struct can_frame &canframe, u_int32_t CAN_id, u_int8_t CAN_data_len, bool is_ext_mode);
    void add_CAN_Filter(unsigned int can_id_, bool is_ext_id);
    void set_CAN_Filter(void);
    void CAN_write(struct can_frame &frame, BYTE data_array[]);
    struct can_frame CAN_read(void);


};



#endif // CAN_OLA_H
