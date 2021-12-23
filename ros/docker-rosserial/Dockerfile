FROM ros:noetic-ros-base

SHELL ["/bin/bash", "-c"]

WORKDIR /app

# install dep and such
RUN apt update && apt install -y git ros-noetic-rosserial ros-noetic-rosserial-arduino

COPY ./entrypoint.sh /
COPY ./alias.sh /

RUN echo "source /alias.sh" >> ~/.bashrc

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
