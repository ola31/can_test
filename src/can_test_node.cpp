//#include "ros/ros.h"
//#include "std_msgs/String.h"
#include "can_test_node.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {

     open_port("can0");
     read_port();
     return 0;

    std_msgs::String msg;
    msg.data = "hello world";

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
