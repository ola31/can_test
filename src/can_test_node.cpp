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
  send_RPM(200,200);

  CAN_REQ(PID_MONITOR);
  loop_rate.sleep();
  while (ros::ok())
  {
    CAN_read();
    /*int l_posi,r_posi;

    read_Encoder(&l_posi, &r_posi);
    ROS_INFO("Left Tick : %d, Right_Tick : %d",l_posi,r_posi);
    std_msgs::String msg;
    msg.data = "hello world";

    chatter_pub.publish(msg);

    ros::spinOnce();
    */
    CAN_REQ(PID_MONITOR);

    loop_rate.sleep();
  }

  return 0;
}
