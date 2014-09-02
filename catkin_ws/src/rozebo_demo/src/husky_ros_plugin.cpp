#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
namespace gazebo
{
  class HuskySimulator : public ModelPlugin
  {
    public:
    HuskySimulator() : ModelPlugin()
    {
    }

    void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Make sure the ROS node for Gazebo has already been initialized
      if (!ros::isInitialized())
      {
        ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
          << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
        return;
      }
      // Save Global Pointers
      this->husky = _model;
      this->sdf   = _sdf;
      this->world = husky->GetWorld();
      this->front_left_joint  = husky->GetJoint("front_left_joint");
      this->front_right_joint = husky->GetJoint("front_right_joint");
      this->back_left_joint   = husky->GetJoint("back_left_joint");
      this->back_right_joint  = husky->GetJoint("back_right_joint");

      this->front_left_wrench  = 0;
      this->front_right_wrench = 0;
      this->back_left_wrench   = 0;
      this->back_right_wrench  = 0;

      this->_spinnerThread = boost::thread(boost::bind(&HuskySimulator::LoadThread, this));
    }

    void LoadThread()
    {
      if(!this->husky){
        ROS_FATAL_STREAM("Husky plugin requires a husky as its parent.");
        return;
      }
      ros::NodeHandle _nh("husky");
      subscriber = _nh.subscribe("diff_wrench", 1000, &HuskySimulator::HuskyStateCallback, this);
      this->_updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&HuskySimulator::OnUpdate, this, _1));
      ROS_INFO("Husky plugin for ROS successfully loaded :)");

      // Check for new messages
//      ros::spin();
      while(ros::ok()){
        ros::spinOnce();
      }
    }

    // Called by the world update start event
    void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      if ( subscriber.getNumPublishers() != 0 ) {
        this->front_left_joint->SetForce(0,front_left_wrench);
        this->front_right_joint->SetForce(0,front_right_wrench);
        this->back_left_joint->SetForce(0,back_left_wrench);
        this->back_right_joint->SetForce(0,back_right_wrench);
      }
      else {
        this->front_left_joint->SetForce(0,0);
        this->front_right_joint->SetForce(0,0);
        this->back_left_joint->SetForce(0,0);
        this->back_right_joint->SetForce(0,0);
      }
    }
    void HuskyStateCallback(const geometry_msgs::Quaternion::ConstPtr& rx_msg)
    {
      front_left_wrench  = rx_msg->x;
      front_right_wrench = rx_msg->y;
      back_left_wrench   = rx_msg->z;
      back_right_wrench  = rx_msg->w;
    }

    // Global pointer declarations
    private: physics::ModelPtr husky;
    private: physics::WorldPtr world;
    private: sdf::ElementPtr sdf;
    private: event::ConnectionPtr _updateConnection;
    private: boost::thread _spinnerThread;

    private: physics::JointPtr front_left_joint;
    private: physics::JointPtr front_right_joint;
    private: physics::JointPtr back_left_joint;
    private: physics::JointPtr back_right_joint;
    private: float front_left_wrench;
    private: float front_right_wrench;
    private: float back_left_wrench;
    private: float back_right_wrench;

    private: ros::Subscriber subscriber;
};
GZ_REGISTER_MODEL_PLUGIN(HuskySimulator);
}

