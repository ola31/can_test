#include "can.h"
#include "motor_driver.h"

#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);

  CAN_initalize();
  Reset_ENC();

  send_RPM(100,100);
  
  struct Encoder_data enc_data;

  Encoder_REQ();
  loop_rate.sleep();

  while (ros::ok())
  {
    enc_data = read_Encoder();
        
    ROS_INFO("R_posi : %d   L_posi : %d",enc_data.R_posi,enc_data.L_posi);

    std_msgs::String msg;
    msg.data = "hello world";

    chatter_pub.publish(msg);

    ros::spinOnce();

    Encoder_REQ();
    loop_rate.sleep();
  }

  return 0;
}
