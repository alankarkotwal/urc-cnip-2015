#!/bin/sh

SCRIPT=$(readlink -f $0)
DIR=`dirname $SCRIPT`

echo "export GAZEBO_RESOURCE_PATH=$GAZEBO_RESOURCE_PATH:$DIR/gazebo" >> ~/.bashrc
echo "export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$DIR/gazebo/plugins" >> ~/.bashrc
echo "export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:$DIR/gazebo/models" >> ~/.bashrc
. ~/.bashrc
