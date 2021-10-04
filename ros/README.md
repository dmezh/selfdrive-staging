# ROS Environment

### ROS environment install
http://wiki.ros.org/noetic/Installation/Ubuntu
- make sure you source ROS environment
```bash
source /opt/ros/noetic/setup.zsh # or setup.bash
```
verify it worked by running 
```
printenv | grep ROS
```

### ROS environment install pt2
http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment
- make sure you source catkin workspace
```bash
# inside catkin_ws
source devel/setup.zsh # or setup.bash
```
verify it worked by running 
```
echo $ROS_PACKAGE_PATH
# you should get something like {your-catkin-path}/catkin_ws/src:/opt/ros/kinetic/share
```

### Gazebo install
http://gazebosim.org/tutorials?tut=install_ubuntu&cat=install


### Gazebo + ROS integration
http://gazebosim.org/tutorials?tut=ros_installing


### TurtleBot3
https://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/


### Reference Repos
https://github.com/robustify/igvc_self_drive_sim
https://github.com/westpoint-robotics/AY21_IGVC/tree/master/catkin_ws