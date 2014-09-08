#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>

namespace gazebo {

	class RoverSimulator : public RoverPlugin {
	
		void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
		
			// Make sure the ROS node for Gazebo has already been initialized
			if (!ros::isInitialized()) {
				ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
				<< "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
				return;
			}
			
			// Save Global Pointers
			this->rover = _model;
			this->sdf = _sdf;
			this->world = rover->GetWorld();

			this->fl_wheel_joint = rover->GetJoint("fl_wheel_joint");
			this->ml_wheel_joint = rover->GetJoint("ml_wheel_joint");
			this->rl_wheel_joint = rover->GetJoint("rl_wheel_joint");
			this->fr_wheel_joint = rover->GetJoint("fr_wheel_joint");
			this->mr_wheel_joint = rover->GetJoint("mr_wheel_joint");
			this->rr_wheel_joint = rover->GetJoint("rr_wheel_joint");

			this->fl_steer_joint  = rover->GetJoint("fl_bogie_wheel_joint");
			this->rl_steer_joint  = rover->GetJoint("left_rocker_wheel_joint");
			this->fr_steer_joint  = rover->GetJoint("fr_bogie_wheel_joint");
			this->rr_steer_joint  = rover->GetJoint("right_rocker_wheel_joint");

			this->fl_wheel_wrench  = 0;
			this->ml_wheel_wrench  = 0;
			this->rl_wheel_wrench  = 0;
			this->fr_wheel_wrench  = 0;
			this->mr_wheel_wrench  = 0;
			this->rr_wheel_wrench  = 0;
			
			this->fl_steer_wrench  = 0;
			this->rl_steer_wrench  = 0;
			this->fr_steer_wrench  = 0;
			this->rr_steer_wrench  = 0;

			this->_spinnerThread = boost::thread(boost::bind(&RoverSimulator::LoadThread, this));
		}

		void LoadThread() {
		
			if(!this->rover) {
				ROS_FATAL_STREAM("Rover plugin requires a rover as its parent.");
				return;
			}
			
			ros::NodeHandle _nh("rover");
			subscriber = _nh.subscribe("rover_forces", 1000, &RoverSimulator::RoverStateCallback, this);
			this->_updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&RoverSimulator::OnUpdate, this, _1));
			ROS_INFO("Rover plugin for ROS successfully loaded :)");

			// Check for new messages
			//      ros::spin();
			while(ros::ok()) {
				ros::spinOnce();
			}
		}

		// Called by the world update start event
		void OnUpdate(const common::UpdateInfo & /*_info*/) {
			if ( subscriber.getNumPublishers() != 0 ) {
				this->fl_wheel_joint->SetForce(0,fl_wheel_wrench);
				this->ml_wheel_joint->SetForce(0,ml_wheel_wrench);
				this->rl_wheel_joint->SetForce(0,rl_wheel_wrench);
				this->rr_wheel_joint->SetForce(0,rr_wheel_wrench);
				this->mr_wheel_joint->SetForce(0,mr_wheel_wrench);
				this->fr_wheel_joint->SetForce(0,fr_wheel_wrench);
				
				this->fl_steer_joint->SetForce(0,fl_steer_wrench);
				this->rl_steer_joint->SetForce(0,rl_steer_wrench);
				this->rr_steer_joint->SetForce(0,rr_steer_wrench);
				this->fr_steer_joint->SetForce(0,fr_steer_wrench);
			}
			else {
				this->fl_wheel_joint->SetForce(0,0);
				this->ml_wheel_joint->SetForce(0,0);
				this->rl_wheel_joint->SetForce(0,0);
				this->rr_wheel_joint->SetForce(0,0);
				this->mr_wheel_joint->SetForce(0,0);
				this->fr_wheel_joint->SetForce(0,0);
				
				this->fl_steer_joint->SetForce(0,0);
				this->rl_steer_joint->SetForce(0,0);
				this->rr_steer_joint->SetForce(0,0);
				this->fr_steer_joint->SetForce(0,0);
			}
		}
		
		void RoverStateCallback(const geometry_msgs::Quaternion::ConstPtr& rx_msg) {
			this->fl_wheel_wrench  = rx_msg->fl_wheel_wrench;
			this->ml_wheel_wrench  = rx_msg->ml_wheel_wrench;
			this->rl_wheel_wrench  = rx_msg->rl_wheel_wrench;
			this->fr_wheel_wrench  = rx_msg->fr_wheel_wrench;
			this->mr_wheel_wrench  = rx_msg->mr_wheel_wrench;
			this->rr_wheel_wrench  = rx_msg->rr_wheel_wrench;
			
			this->fl_steer_wrench  = rx_msg->fl_steer_wrench;
			this->rl_steer_wrench  = rx_msg->rl_steer_wrench;
			this->fr_steer_wrench  = rx_msg->fr_steer_wrench;
			this->rr_steer_wrench  = rx_msg->rr_steer_wrench;
		}

		// Global pointer declarations
		private: physics::ModelPtr rover;
		private: physics::WorldPtr world;
		private: sdf::ElementPtr sdf;
		private: event::ConnectionPtr _updateConnection;
		private: boost::thread _spinnerThread;

		private: physics::JointPtr fl_wheel_joint;
		private: physics::JointPtr ml_wheel_joint;
		private: physics::JointPtr rl_wheel_joint;
		private: physics::JointPtr fr_wheel_joint;
		private: physics::JointPtr mr_wheel_joint;
		private: physics::JointPtr rr_wheel_joint;

		private: physics::JointPtr fl_steer_joint;
		private: physics::JointPtr rl_steer_joint;
		private: physics::JointPtr fr_steer_joint;
		private: physics::JointPtr rr_steer_joint;

		private: float fl_wheel_wrench;
		private: float ml_wheel_wrench;
		private: float rl_wheel_wrench;
		private: float rr_wheel_wrench;
		private: float mr_wheel_wrench;
		private: float fr_wheel_wrench;
		
		private: float fl_steer_wrench;
		private: float rl_steer_wrench;
		private: float rr_steer_wrench;
		private: float fr_steer_wrench;

		private: ros::Subscriber subscriber;
	};

	GZ_REGISTER_MODEL_PLUGIN(RoverSimulator);
}

