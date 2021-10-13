#!/bin/bash
rosdep install --from-paths . --ignore-src --rosdistro noetic -y
catkin_make
