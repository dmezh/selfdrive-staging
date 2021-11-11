# This is from: https://github.com/introlab/rtabmap_ros/blob/master/docker/noetic/Dockerfile
# FROM ros:noetic-perception

FROM ros:noetic-ros-base
# install rtabmap packages
ARG CACHE_DATE=2016-01-01
RUN apt-get update && apt-get install -y \
    ros-noetic-rtabmap \
    ros-noetic-rtabmap-ros \
    && rm -rf /var/lib/apt/lists/

COPY ./entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
