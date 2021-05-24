#include "can.h"
#include "motor_driver.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "geometry_msgs/Twist.h"
#include "can_test/rpm.h"

static int operating_mode=2;           //start mode = cmd_vel mode

void modeCallback(const std_msgs::Int16::ConstPtr& msg){

  operating_mode = msg->data;
  if(operating_mode == 1)
    ROS_INFO("Operating Mode : RPM control mode");
  else if(operating_mode == 2)
    ROS_INFO("Operating Mode : CMD_VEL control mode");
  else
    ROS_WARN("Invalid control mode number");
}

void cmd_velCallback(const geometry_msgs::Twist::ConstPtr& msg){

  float linear_x = msg->linear.x;
  float angular_z = msg->angular.z;
  float vel_arr[2] = {linear_x,angular_z};

  if(operating_mode == 2){
     contol_vel(vel_arr);
    ROS_INFO("Linear_x : %f angular_z : %f",linear_x,angular_z);
  }
}

void rpmCallback(const can_test::rpm::ConstPtr& msg){
    int r_rpm = msg->r_rpm;
    int l_rpm = msg->l_rpm;
    send_RPM(r_rpm,l_rpm);

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "can_test_node");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("/chatter", 1000);
  ros::Subscriber mode_sub = nh.subscribe("/mode", 1000, modeCallback);
  ros::Subscriber cmd_vel_sub = nh.subscribe("/cmd_vel", 1000, cmd_velCallback);
  ros::Subscriber rpm_sub = nh.subscribe("/rpm", 1000, rpmCallback);

  ros::Rate loop_rate(10);

  CAN_initialize();
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
