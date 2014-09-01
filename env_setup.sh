#!/bin/bash

SCRIPT=$(readlink -f $0)
DIR=`dirname $SCRIPT`/catkin_ws

sudo chmod 666 /usr/share/gazebo-4.0/setup.sh
sudo chmod 666 /usr/share/gazebo/setup.sh
echo "export GAZEBO_RESOURCE_PATH=$GAZEBO_RESOURCE_PATH:$DIR/gazebo" >> /usr/share/gazebo-4.0/setup.sh
echo "export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$DIR/gazebo/plugins" >> /usr/share/gazebo-4.0/setup.sh
echo "export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:$DIR/gazebo/models" >> /usr/share/gazebo-4.0/setup.sh
echo "export GAZEBO_RESOURCE_PATH=$GAZEBO_RESOURCE_PATH:$DIR/gazebo" >> /usr/share/gazebo/setup.sh
echo "export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$DIR/gazebo/plugins" >> /usr/share/gazebo/setup.sh
echo "export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:$DIR/gazebo/models" >> /usr/share/gazebo/setup.sh
sudo chmod 666 /usr/share/gazebo-4.0/setup.sh
sudo chmod 666 /usr/share/gazebo/setup.sh
echo "alias @msi='cd $DIR;source $DIR/devel/setup.sh'" >> ~/.bash_aliases

echo "Environment setup complete :)."
echo "Alias @msi created to change ROS workspace to MSI. Restart the terminal or source ~/.bashrc to enable it."
