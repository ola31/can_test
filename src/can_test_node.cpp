#include "can.h"

//struct can_frame can_f;

//struct can_frame frame;

//frame.can_id = 0xB7AC01 ;        //32bit
//frame.can_dlc=8;       //8bit
//frame.data[8]={1,1,1,1,1,1,1,1}; //8bit


int main(int argc, char **argv)
{
  struct can_frame frame;
  frame.can_id = 0xB7AC01 ;        //32bit
  frame.can_dlc=8;       //8bit
  BYTE data[8]={1,2,3,4,5,6,7,8}; //8bit

  memcpy(frame.data,data,sizeof(data));
  //data[8]={1,1,1,1,1,1,1,1}; //8bit

  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);

  open_port("can0");
  while (ros::ok())
  {
    read_port();
    send_port(&frame);

    std_msgs::String msg;
    msg.data = "hello world";

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
