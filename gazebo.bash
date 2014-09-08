#!/bin/bash

if [[ $EUID -eq 0 ]]; then
  echo -e "\e[01;31m[ERR]: This script must NOT be run as root (or sudo). It will ask for sudo permissions when required.\e[0m" 1>&2
  exit 1
fi

clear
printf '%*s\n' "${COLUMNS:-$(tput cols)}" '' | tr ' ' '#'
printf '%*s\n' "${COLUMNS:-$(tput cols)}" '' | tr ' ' '#'
echo -e "\nThis is an installation script for Gazebo4 with ROS and DEM Support. Packages can also be installed individually"
echo -e "ROS Indigo will be installed with Ubuntu 14.04 (Trusty) while ROS Hydro will be installed with Ubuntu 13.10 (Saucy)"
echo -e "\nAuthor: Kamal Galrani\n"
echo -e "\e[01;31mDo NOT press Ctrl+C while this script is running or you could end up in some unprecedented situation. Pressing Alt+F4 will be a better option.\e[0m\n"
printf '%*s\n' "${COLUMNS:-$(tput cols)}" '' | tr ' ' '#'
printf '%*s\n' "${COLUMNS:-$(tput cols)}" '' | tr ' ' '#'
env | grep ROS &>/dev/null
if [ $? -eq 0 ]; then
  echo -e "\e[01;31m[ERR]: Unsource the '/opt/ros/*/setup.bash' file to continue...\e[0m" 1>&2
  exit 2
fi
echo -e "This script requires sudo privileges. Use your Ubuntu login password."
sudo echo -e "\e[01;36m[INFO]: Authentication successful :)\e[0m"
if [ $? -ne 0 ]; then
  echo -e "\e[01;31m[ERR]: Authentication failed :(\e[0m" 1>&2
  echo exit 3
fi

codename=`lsb_release -cs`
release=`lsb_release -rs`

case "$codename" in
'trusty') echo -e "\e[01;36m[INFO]: Running on Ubuntu 14.04 (Trusty Tahr)\e[0m";;
'saucy') echo -e "\e[01;36m[INFO]: Running on Ubuntu 13.10 (Saucy Salamander)\e[0m";;
*) echo -e "\e[01;31m[ERR]: This script supports only Ubuntu 14.04 (Trusty Tahr) and Ubuntu 13.10 (Saucy Salamander) (not tested)\e[0m";exit 4;
esac

ros_indigo(){
  case "$codename" in
    'trusty') sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu trusty main" > /etc/apt/sources.list.d/ros-latest.list';;
    'saucy') sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu saucy main" > /etc/apt/sources.list.d/ros-latest.list';;
    *) echo -e "\e[01;31m[ERR]: Unable to determine operating system! Aborting...\e[0m";exit 5;
  esac
  wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -
  sudo apt-get update
  sudo apt-get install libopencv*
  sudo apt-get install ros-indigo-desktop-full
  sudo rosdep init
  rosdep update
  sudo apt-get install python-rosinstall
  echo "source /opt/ros/indigo/setup.sh" >> ~/.bashrc
}

ros_hydro(){
  echo -e "\e[01;33m[WARN]: This part is not tested for lack of testbench. Ping me if there is some problem. Press enter to continue...\e[0m"
  read input
  sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu raring main" > /etc/apt/sources.list.d/ros-latest.list'
  wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -
  sudo apt-get update
  sudo apt-get install libopencv*
  sudo apt-get install ros-hydro-desktop-full
  sudo rosdep init
  rosdep update
  sudo apt-get install python-rosinstall
  echo "source /opt/ros/hydro/setup.sh" >> ~/.bashrc
}

gazebo_plain(){
  sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-latest.list'
  wget http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
  sudo apt-get update
  sudo apt-get install libopencv*
  sudo apt-get install gazebo4 libgazebo4-dev
  echo "source /usr/share/gazebo/setup.sh" >> ~/.bashrc
}

gazebo_ros(){
  sudo apt-get remove ros-fuerte-simulator-gazebo ros-groovy-simulator-gazebo
  case "$codename" in
    'trusty') ros_indigo; gazebo_plain; sudo apt-get install ros-indigo-gazebo4-msgs ros-indigo-gazebo4-plugins ros-indigo-gazebo4-ros ros-indigo-gazebo4-ros-control ros-indigo-gazebo4-ros-pkgs;;
    'saucy') ros_hydro; gazebo_plain; sudo apt-get install ros-hydro-gazebo4-msgs ros-hydro-gazebo4-plugins ros-hydro-gazebo4-ros ros-hydro-gazebo4-ros-control ros-hydro-gazebo4-ros-pkgs;;
    *) echo -e "\e[01;31m[ERR]: Unable to determine operating system! Aborting...\e[0m";exit 5;
  esac
  echo "source /usr/share/gazebo/setup.sh" >> ~/.bashrc
}

gazebo_dem(){
  sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-latest.list'
  wget http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
  sudo apt-get remove 'gazebo-*' 'sdformat-*' ros-hydro-simulator-gazebo ros-indigo-simulator-gazebo
  sudo apt-get install libopencv*
  sudo apt-get install build-essential libtinyxml-dev libboost-all-dev cmake mercurial pkg-config libprotoc-dev libprotobuf-dev protobuf-compiler libqt4-dev libtar-dev libcurl4-openssl-dev libcegui-mk2-dev libopenal-dev libtbb-dev libswscale-dev libavformat-dev libavcodec-dev libogre-1.8-dev libgts-dev libltdl3-dev playerc++ libxml2-dev libfreeimage-dev freeglut3-dev xsltproc ruby-ronn robot-player-dev* gdal-bin libgdal-dev libgdal1h python-gdal doxygen
  if [ $? -ne 0 ]; then
    echo "\e[01;31m[ERR]: Failed installing prerequisites for making gazebo4 :(\nAborting...\e[0m"
    exit 6
  fi
  cd /usr/local/src
  sudo chmod 777 /usr/local/src
  hg clone https://bitbucket.org/osrf/sdformat
  cd sdformat
  hg up sdf_2.0
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX=/usr ../
  make -j4
  sudo make install
  cd /usr/local/src
  hg clone https://bitbucket.org/osrf/gazebo
  cd gazebo
  hg up gazebo_4.0
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_TESTS_COMPILATION:BOOL=False ../
  echo -e "\e[01;33m[WARN]: Is the above cmake output for gazebo4 correct? (y/n)\e[0m"
  read input
  if [ $input != 'y' ]; then
    echo "Aborted. Gazebo4 cmake error!"
    exit 7
  fi
  make -j4
  sudo chmod 755 /usr/local/src
  sudo make install
  echo "source /usr/share/gazebo/setup.sh" >> ~/.bashrc
}

all(){
  sudo apt-get remove ros-fuerte-simulator-gazebo ros-groovy-simulator-gazebo
  gazebo_ros
  gazebo_dem
  cd /usr/local/src/gazebo/build
  sudo make install
}

menu_disp(){
echo -e "\n\nChoose from the following options:\n\n\
1) Set up Gazebo4 with ROS and DEM support\n\
2) Set up Gazebo4 with DEM only\n\
3) Set up Gazebo4 with ROS only\n\
4) Set up Gazebo4 without ROS or DEM\n\
5) Set up ROS Indigo (No support for Gazebo4 on Saucy)\n\
6) Set up ROS Hydro (No support for Gazebo4 on Trusty)\n\
0) Quit\n
\nType your option and hit enter:"
menu_read
}

menu_read(){
  read input
  case "$input" in
    1) all;;
    2) gazebo_dem;;
    3) gazebo_ros;;
    4) gazebo_plain;;
    5) ros_indigo;;
    6) ros_hydro;;
    0) exit 0;;
    *) echo -e "\e[01;31m[ERR]: Invalid response\nTry again:\e[0m";menu_read;;
  esac
  echo -e "\e[01;32m[INFO]: All done :)\e[0m"
}

menu_disp
