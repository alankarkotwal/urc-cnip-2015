#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Quaternion.h>
#include <stdlib.h>
#define INDEX 1

float t_vel_2 = 4;

ros::Publisher            publisher;
geometry_msgs::Quaternion tx_msg;

void HuskyStateCallback(const gazebo_msgs::ModelStates::ConstPtr& rx_msg)
{
  if (rx_msg->name[INDEX] != "husky") {
    ROS_ERROR("Husky not found at specified index in '/gazebo/model_states'");
    exit (EXIT_FAILURE);
  }
//  ROS_INFO("Position    ( %f, %f, %f )", rx_msg->pose[INDEX].position.x, rx_msg->pose[INDEX].position.y, rx_msg->pose[INDEX].position.z);
//  ROS_INFO("Orientation ( %f, %f, %f, %f )", rx_msg->pose[INDEX].orientation.x, rx_msg->pose[INDEX].orientation.y, rx_msg->pose[INDEX].orientation.z, rx_msg->pose[INDEX].orientation.w);
//  ROS_INFO("Linear      ( %f, %f, %f )", rx_msg->twist[INDEX].linear.x, rx_msg->twist[INDEX].linear.y, rx_msg->twist[INDEX].linear.z);
//  ROS_INFO("Angular     ( %f, %f, %f )", rx_msg->twist[INDEX].angular.x, rx_msg->twist[INDEX].angular.y, rx_msg->twist[INDEX].angular.z);

  float x_vel = rx_msg->twist[INDEX].linear.x;
  float y_vel = rx_msg->twist[INDEX].linear.y;
  float z_vel = rx_msg->twist[INDEX].linear.z;
//  float x_pos = rx_msg->pose[INDEX].position.x;
//  float y_pos = rx_msg->pose[INDEX].position.y;
  float vel_2 = x_vel*x_vel + y_vel*y_vel + z_vel*z_vel;
  float scale = ( vel_2 != 0 ) ? t_vel_2/vel_2 : 0.1;
  scale = ( scale > 1 ) ? 1 : scale;
  x_vel = x_vel*1.5;
  tx_msg.x = (5 - x_vel)*scale;	//Front Left Wheel
  tx_msg.y = (5 + x_vel)*scale;	//Front Right Wheel
  tx_msg.z = (5 - x_vel)*scale;	//Back Left Wheel
  tx_msg.w = (5 + x_vel)*scale;	//Back Right Wheel

  ROS_INFO("Velocity^2 = %f \t Scale = %f \t X Vel %f", vel_2, scale, x_vel);
  publisher.publish(tx_msg);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "husky_ros_node");
  ros::NodeHandle _nh("husky");

  publisher = _nh.advertise<geometry_msgs::Quaternion>("diff_wrench", 1000);
  ros::Subscriber subscriber = _nh.subscribe("/gazebo/model_states", 1000, HuskyStateCallback);

  ros::spin();
  return 0;
}
