#!/bin/bash
export IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
/usr/X11/bin/xhost +
/usr/X11/bin/xhost + "$IP"
docker  run -it --name delightrail2022c -p 8891:8891 -e DISPLAY=host.docker.internal:0  -v /tmp/.X11-unix:/tmp/.X11-unix -v "${PWD}/notebooks:/home/ubuntu/notebooks" -v "${PWD}/Delight:/home/ubuntu/softdev/Delight" -v "${PWD}/RAIL:/home/ubuntu/softdev/RAIL" -d delightrail2022
