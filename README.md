MSI - URC 2015
==============
Instructions:
* Install gazebo4 with ROS and DEM support on Ubuntu 14.04 (13.10 may work, setup.bash script included).
* Run env_setup.sh to setup environment.
* Restart the terminal Type the following

```
@msi
catkin_make
```

* Test the installation with `roslaunch rozebo_demo dem_plugin.launch` (`roslaunch rozebo_demo dem_only.launch` for DEM only, `roslaunch rozebo_demo plugin_only.launch` for plugin only)
* "Hello World!..." should be echoed in the terminal and husky should move on playing the simulation
