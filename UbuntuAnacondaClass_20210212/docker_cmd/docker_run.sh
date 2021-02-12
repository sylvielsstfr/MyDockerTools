#!/bin/bash
docker  run -it --name classc -p 8888:8888 --env "DISPLAY" -v "$PWD/notebooks:/home/ubuntu/notebooks" -d class
