FROM ros:noetic-ros-base

SHELL ["/bin/bash", "-c"]

WORKDIR /app

RUN apt update && apt install -y git

RUN mkdir -p ros_ws/src &&\
    git clone --recursive https://github.com/stereolabs/zed-ros-wrapper.git
 ros_ws/src/zed

RUN cd ros_ws &&\
    source /opt/ros/noetic/setup.bash &&\
    rosdep install --from-paths src --ignore-src -r -y && \
    catkin_make -DCATKIN_ENABLE_TESTING=0 -DCMAKE_BUILD_TYPE=Release

COPY ./entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
