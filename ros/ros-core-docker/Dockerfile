FROM ros:noetic-ros-core
LABEL maintainer "Cooper IGVC <igvc@cooper.edu>"

SHELL ["/bin/bash","-c"]
ENTRYPOINT ["/ros_entrypoint.sh"]

# rosdep
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get install -y python3-rosdep 
RUN apt-get install -y --no-install-recommends apt-utils
RUN rosdep init

#received warning don't run rosdep as sudo, fix
RUN sudo rosdep fix-permissions
RUN rosdep update

# other stuff
RUN apt-get install -y build-essential

# may not be needed
RUN apt-get	install -y ros-noetic-ros-core 

	#RUN rm -rf /var/lib/apt/lists/

ENV DEBIAN_FRONTEND teletype

CMD ["bash"]
