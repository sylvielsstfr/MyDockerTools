#!/bin/bash
export IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
/usr/X11/bin/xhost +
/usr/X11/bin/xhost + "$IP"
docker  run -it --name ircamc -p 8892:8892 -e DISPLAY=host.docker.internal:0  -v /tmp/.X11-unix:/tmp/.X11-unix -v "${PWD}/notebooks:/home/ubuntu/notebooks" ircam