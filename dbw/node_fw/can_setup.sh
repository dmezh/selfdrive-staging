#!/bin/bash

ip link set can0 type can bitrate $1
ip link set can0 up
