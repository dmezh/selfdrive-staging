FROM ros:noetic-ros-base

ARG DEBIAN_FRONTEND=noninteractive

SHELL ["/bin/bash", "-c"]

WORKDIR /app

# ------------------------
# --- GAZEBO SERVER 11 ---
# ------------------------
# setup timezone
RUN apt-get update && \
    apt-get install -q -y --no-install-recommends tzdata apt-utils

# install packages
RUN apt-get update && apt-get install -q -y --no-install-recommends \
    dirmngr \
    gnupg2 \
    lsb-release

# setup keys
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys D2486D2DD83DB69272AFE98867170598AF249743

# setup sources.list
RUN . /etc/os-release \
    && echo "deb http://packages.osrfoundation.org/gazebo/$ID-stable `lsb_release -sc` main" > /etc/apt/sources.list.d/gazebo-latest.list

# install gazebo packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    gazebo11=11.9.0-1*

# setup environment
EXPOSE 11345

RUN apt update && apt install -y git

RUN mkdir -p ros_ws/src &&\
    git clone https://github.com/ros-simulation/gazebo_ros_pkgs.git ros_ws/src/gazebo-packages && \
    git clone -b noetic-devel https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git ros_ws/src/turtle

RUN cd ros_ws && \
    source /opt/ros/noetic/setup.bash && \
    rosdep fix-permissions && \
    rosdep update && \
    rosdep install --from-paths . --ignore-src -y && \
    catkin_make -DCATKIN_ENABLE_TESTING=0 -DCMAKE_BUILD_TYPE=Release && \
    export TURTLEBOT3_MODEL=burger

COPY ./entrypoint.sh /

ENV TURTLEBOT3_MODEL='waffle_pi'

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
