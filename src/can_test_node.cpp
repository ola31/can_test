#include "can_test/can_test_node.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"

void print_CAN(struct can_frame frame);


/************************
 *
 * This <can_test_node> is example of how to use <can_test> package.
 * To use <can_test>, socket CAN device(such as "CANable") is needed.
 *
 * author : ola31
 *
 **********************/

int main(int argc, char **argv)
{
  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  ros::Rate loop_rate(10);

  CAN can("can0","ttyACM0");             //CAN object declearation

  struct can_frame send_frame1;
  struct can_frame send_frame2;

  struct can_frame recv_frame;

  can.CAN_initialize(_250k);
  can.set_can_frame(send_frame1,0x01,8,true);
  can.set_can_frame(send_frame2,0x02,8,true);

  BYTE can_array1[8]={'H','e','l','l','o','I','D','1'};
  BYTE can_array2[8]={'H','e','l','l','o','I','D','2'};

  //CAN filter adding
  can.add_CAN_Filter(0x100,false);
  can.add_CAN_Filter(0xB7AC01,true);
  //CAN filter apply
  can.set_CAN_Filter();

  while (ros::ok())
  {
    recv_frame = can.CAN_read();
    print_CAN(recv_frame);

    can.CAN_write(send_frame1, can_array1);
    can.CAN_write(send_frame2, can_array2);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}



void print_CAN(struct can_frame frame){
  printf("\n");
  printf("can_id     : %x\n",frame.can_id);
  printf("can_length : %d\n",frame.can_dlc);
  printf("can_data   :\n");
  printf("[%c,%c,%c,%c,%c,%c,%c,%c]\n",
         frame.data[0],
         frame.data[1],
         frame.data[2],
         frame.data[3],
         frame.data[4],
         frame.data[5],
         frame.data[6],
         frame.data[7] );
  printf("\n");
}
