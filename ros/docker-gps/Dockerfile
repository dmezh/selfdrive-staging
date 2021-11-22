FROM ros:noetic-ros-base

SHELL ["/bin/bash", "-c"]

WORKDIR /app

RUN apt update && apt install -y git ros-noetic-rosdoc-lite

# RUN apt install -y ros-noetic-novatel-oem7-driver

RUN mkdir -p ros_ws/src &&\
    git clone -b ros1 https://github.com/novatel/novatel_oem7_driver.git ros_ws/src/gps_ros

RUN cd ros_ws &&\
    rosdep install --from-paths src --ignore-src -r -y &&\
    source /opt/ros/noetic/setup.bash &&\
    # ls -la &&\
    # source devel/setup.bash &&\
    source src/gps_ros/build.sh -f
    # catkin_make -DCATKIN_ENABLE_TESTING=0 -DCMAKE_BUILD_TYPE=Release

COPY ./entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
