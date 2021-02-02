#!/bin/bash
docker  run -it --name txpipec -p 8888:8888 --env "DISPLAY" -v "$PWD/notebooks:/home/ubuntu/notebooks" -d txpipe
