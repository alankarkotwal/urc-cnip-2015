#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <stdlib.h>
#include <rover_test/RoverCommands.h>
#define INDEX 0

float t_vel_2 = 4;

ros::Publisher publisher;
rover_test::RoverCommands tx_msg;

void RoverStateCallback(const gazebo_msgs::ModelStates::ConstPtr& rx_msg) {
	if (rx_msg->name[INDEX] != "rover") { // TODO: Search for rover in general
		ROS_ERROR("Rover not found at specified index in '/gazebo/model_states'");
		exit (EXIT_FAILURE);
	}
	ROS_INFO("Position    ( %f, %f, %f )", rx_msg->pose[INDEX].position.x, rx_msg->pose[INDEX].position.y, rx_msg->pose[INDEX].position.z);
	ROS_INFO("Orientation ( %f, %f, %f, %f )", rx_msg->pose[INDEX].orientation.x, rx_msg->pose[INDEX].orientation.y, rx_msg->pose[INDEX].orientation.z, rx_msg->pose[INDEX].orientation.w);
	ROS_INFO("Linear      ( %f, %f, %f )", rx_msg->twist[INDEX].linear.x, rx_msg->twist[INDEX].linear.y, rx_msg->twist[INDEX].linear.z);
	ROS_INFO("Angular     ( %f, %f, %f )", rx_msg->twist[INDEX].angular.x, rx_msg->twist[INDEX].angular.y, rx_msg->twist[INDEX].angular.z);

	float x_vel = rx_msg->twist[INDEX].linear.x;
	float y_vel = rx_msg->twist[INDEX].linear.y;
	float z_vel = rx_msg->twist[INDEX].linear.z;
	float x_pos = rx_msg->pose[INDEX].position.x;
	float y_pos = rx_msg->pose[INDEX].position.y;
	float vel_2 = x_vel*x_vel + y_vel*y_vel + z_vel*z_vel;
	
	// Set rover forces here and publish them to rover_forces
	tx_msg.fl_wheel_wrench = 0;
	tx_msg.ml_wheel_wrench = 0;
	tx_msg.rl_wheel_wrench = 0;
	tx_msg.rr_wheel_wrench = 0;
	tx_msg.mr_wheel_wrench = 0;
	tx_msg.fr_wheel_wrench = 0;
	tx_msg.fl_steer_wrench = 0;
	tx_msg.rl_steer_wrench = 0;
	tx_msg.rr_steer_wrench = 0;
	tx_msg.fr_steer_wrench = 0;

	ROS_INFO("Velocity^2 = %f \t X Vel %f", vel_2, x_vel);
	publisher.publish(tx_msg);
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "rover_ros_node");
	ros::NodeHandle _nh("rover");

	publisher = _nh.advertise<rover_test::RoverCommands>("rover_forces", 1000);
	ros::Subscriber subscriber = _nh.subscribe("/gazebo/model_states", 1000, RoverStateCallback);

	ros::spin();
	return 0;
}
