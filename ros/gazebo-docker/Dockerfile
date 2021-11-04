FROM igvc/rosbase:alpha

ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux

RUN mkdir -p /catkin_ws/src
RUN cd /catkin_ws && /ros_entrypoint.sh catkin_make

COPY igvc_self_drive_sim /catkin_ws/igvc_self_drive_sim

RUN apt-get install -y ros-noetic-gazebo-ros-pkgs ros-noetic-gazebo-ros-control
RUN cd /catkin_ws && rosdep install --from-paths src --ignore-src -r

# Pass display attempt
# Replace 1000 with your user / group id
RUN export uid=1000 gid=1000 && \
    mkdir -p /home/developer && \
    echo "developer:x:${uid}:${gid}:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd && \
    echo "developer:x:${uid}:" >> /etc/group && \
    echo "developer ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/developer && \
    chmod 0440 /etc/sudoers.d/developer && \
    chown ${uid}:${gid} -R /home/developer

USER developer
ENV DEBIAN_FRONTEND teletype
ENV HOME /home/developer
#only one CMD line allowed
#CMD /usr/bin/gazebo

CMD ["bash"]
