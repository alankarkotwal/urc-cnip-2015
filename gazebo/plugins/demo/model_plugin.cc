#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <iostream>
using namespace std;
namespace gazebo
{
  class ModelPluginDemo : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->rear_right_wheel = _parent->GetJoint("rear_right_wheel_joint");
      this->rear_left_wheel = _parent->GetJoint("rear_left_wheel_joint");
      this->front_right_wheel = _parent->GetJoint("front_right_wheel_joint");
      this->front_left_wheel = _parent->GetJoint("front_left_wheel_joint");

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPluginDemo::OnUpdate, this, _1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
//      double vel   = (this->model->GetWorldLinearVel()).GetLength();
      double x_vel = (this->model->GetWorldLinearVel()).Dot(math::Vector3(1,0,0));

      x_vel = x_vel * 450;
//      vel = (vel == 0)?0.1:vel;
      double rr = 450+x_vel;
      double rl = 450-x_vel;
      double fr = 200+x_vel;
      double fl = 200-x_vel;

      cout << rr << "\t" << rl << "\t" << fr << "\t" << fl << endl;

      this->rear_right_wheel->SetForce(0,rr);
      this->rear_left_wheel->SetForce(0,rl);
      this->front_right_wheel->SetForce(0,fr);
      this->front_left_wheel->SetForce(0,fl);
    }

    // Pointer to the model
    private: physics::ModelPtr model;
    private: physics::JointPtr rear_right_wheel;
    private: physics::JointPtr rear_left_wheel;
    private: physics::JointPtr front_right_wheel;
    private: physics::JointPtr front_left_wheel;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPluginDemo)
}
