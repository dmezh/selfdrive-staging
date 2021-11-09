# https://www.stereolabs.com/docs/docker/install-guide-linux/
# https://gitlab.com/nvidia/container-images/cuda/blob/master/dist/11.4.2/ubuntu2004/base/Dockerfile
FROM ros:noetic-ros-base

# # cuda stuff
# ENV NVARCH x86_64
# ENV NVIDIA_REQUIRE_CUDA "cuda>=11.4 brand=tesla,driver>=418,driver<419 brand=tesla,driver>=440,driver<441 brand=tesla,driver>=450,driver<451"
# ENV NV_CUDA_CUDART_VERSION 11.4.108-1
# ENV NV_CUDA_COMPAT_PACKAGE cuda-compat-11-4

# ENV NVARCH sbsa
# ENV NVIDIA_REQUIRE_CUDA "cuda>=11.4"
# ENV NV_CUDA_CUDART_VERSION 11.4.108-1

# RUN apt-get update && apt-get install apt-utils -y --no-install-recommends \
#     gnupg2 curl ca-certificates && \
#     curl -fsSL https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/${NVARCH}/7fa2af80.pub | apt-key add - && \
#     echo "deb https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/${NVARCH} /" > /etc/apt/sources.list.d/cuda.list && \
#     if [ ! -z ${NV_ML_REPO_ENABLED} ]; then echo "deb ${NV_ML_REPO_URL} /" > /etc/apt/sources.list.d/nvidia-ml.list; fi 

# ENV CUDA_VERSION 11.4.2

# # Required for nvidia-docker v1
# RUN echo "/usr/local/nvidia/lib" >> /etc/ld.so.conf.d/nvidia.conf \
#     && echo "/usr/local/nvidia/lib64" >> /etc/ld.so.conf.d/nvidia.conf

# ENV PATH /usr/local/nvidia/bin:/usr/local/cuda/bin:${PATH}
# ENV LD_LIBRARY_PATH /usr/local/nvidia/lib:/usr/local/nvidia/lib64

# # nvidia-container-runtime
# ENV NVIDIA_VISIBLE_DEVICES all
# ENV NVIDIA_DRIVER_CAPABILITIES compute,utility

# # end of cuda stuff

SHELL ["/bin/bash", "-c"]


RUN apt update && apt install -y wget

RUN source /etc/lsb-release && \
    UBUNTU_VERSION=ubuntu${DISTRIB_RELEASE/./} && \
    wget https://developer.download.nvidia.com/compute/cuda/repos/${UBUNTU_VERSION}/x86_64/cuda-${UBUNTU_VERSION}.pin && \
    mv cuda-${UBUNTU_VERSION}.pin /etc/apt/preferences.d/cuda-repository-pin-600 && \
    apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/${UBUNTU_VERSION}/x86_64/7fa2af80.pub && \
    add-apt-repository "deb http://developer.download.nvidia.com/compute/cuda/repos/${UBUNTU_VERSION}/x86_64/ /"

RUN apt update && apt install -y cuda-11-5

WORKDIR /app

RUN apt update && apt install -y git 

# install zed sdk
RUN curl -L https://download.stereolabs.com/zedsdk/3.6/cu114/ubuntu20 --output zed-ros.run && \
    chmod +x zed-ros.run && \
    ./zed-ros.run -- silent

RUN mkdir -p ros_ws/src &&\
    git clone --recursive https://github.com/stereolabs/zed-ros-wrapper.git ros_ws/src/zed

RUN cd ros_ws &&\
    source /opt/ros/noetic/setup.bash &&\
    rosdep install --from-paths src --ignore-src -r -y && \
    catkin_make -DCATKIN_ENABLE_TESTING=0 -DCMAKE_BUILD_TYPE=Release

COPY ./entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
