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
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPluginDemo::OnUpdate, this, _1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
      math::Vector3 diff;
      math::Vector3 init(0, 1, 0);
      diff = this->model->GetWorldLinearVel();
      cout << "_Velocity is\t" << diff << endl;
      diff = diff*math::Vector3(0, 0, 1);
      cout << "ZVelocity is\t" << diff << endl;
      diff+= math::Vector3(0, 1, 0);
      cout << "SVelocity is\t" << diff << endl;
//      cout << "Diff*(1,1,0)" << diff << endl;
//      diff = 10*(diff-init);
//      cout << "Diff*(1,1,0)-Init" << diff << endl << endl;
      this->model->SetLinearVel(diff);
//      this->model->SetLinearAccel(math::Vector3(0, 2, -9.8));
//      this->model->SetLinearVel(math::Vector3(0, 1, 0));
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPluginDemo)
}
