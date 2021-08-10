//#include "can_test/can.h"
#include "can_test/can_test_node.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "geometry_msgs/Twist.h"

static int operating_mode=2;           //start mode = cmd_vel mode
/*
void modeCallback(const std_msgs::Int8::ConstPtr& msg){

  if(operating_mode != msg->data){
    md.Torque_OFF();
    //ROS_INFO("modecallback1");
  }
  operating_mode = msg->data;

  //ROS_INFO("modecallback2");

  if(operating_mode == 1){}
    //ROS_INFO("Operating Mode : RPM control mode");
  else if(operating_mode == 2){}
    //ROS_INFO("Operating Mode : CMD_VEL control mode");
  else if(operating_mode == 0){
    ROS_INFO("TQ_OFF!!");
    md.Torque_OFF();
  }
 else if(operating_mode == 3){
    //ROS_INFO("Dymanic cmd_vel mode");
 }
  else
    ROS_WARN("Invalid control mode number");
}

void cmd_velCallback(const geometry_msgs::Twist::ConstPtr& msg){

  float linear_x = msg->linear.x;
  float angular_z = msg->angular.z;
  float vel_arr[2] = {linear_x,angular_z};

  if(operating_mode == 2 || operating_mode==3){
     md.contol_vel(vel_arr);
    //ROS_INFO("Linear_x : %f angular_z : %f",linear_x,angular_z);
  }
}

void rpmCallback(const can_test::rpm::ConstPtr& msg){

  if(operating_mode == 1){
    //ROS_INFO("rpmcallback1");
    int r_rpm = (msg->r_rpm)*-1;
    int l_rpm = msg->l_rpm;
    md.send_RPM(r_rpm,l_rpm);
  }
  //ROS_INFO("rpmcallback2");

}
*/
int main(int argc, char **argv)
{
  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  //ros::Publisher present_rpm_pub = nh.advertise<can_test::rpm>("/present_rpm", 1000);
  //ros::Subscriber mode_sub = nh.subscribe("/mode", 1000, modeCallback);
  //ros::Subscriber cmd_vel_sub = nh.subscribe("/cmd_vel", 1000, cmd_velCallback);
  //ros::Subscriber rpm_sub = nh.subscribe("/rpm", 1000, rpmCallback);

  ros::Rate loop_rate(10);

  can.CAN_initialize(_250k,"CAN0");

  struct CAN_data c_data;
  BYTE can_array[8]={'H','e','l','l','o','C','A','N'};

  while (ros::ok())
  {
    c_data = can.CAN_read();

    //ROS_INFO("data[0] : %d",c_data.data[0]);

    //present_rpm_pub.publish(msg);

    ros::spinOnce();

    //can.CAN_write(can_array);
    loop_rate.sleep();
  }

  return 0;
}
