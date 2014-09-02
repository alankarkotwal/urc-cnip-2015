MSI - URC 2015
==============
Instructions:
* Install gazebo4 with ROS and DEM support on Ubuntu 14.04 (13.10 may work, setup.bash script included).
* Run env_setup.sh to setup environment.
* Restart the terminal Type the following

```
@msi
catkin_make
roslaunch rozebo_demo dem_plugin.launch
#
# Replace 'dem_plugin.launch' with 'dem_only.launch'
# or 'plugin_only.launch' for individual tests
#

```
* Now play the simulation.
