#!/bin/bash
docker  run -it --name delightrailc -p 8888:8888 --env "DISPLAY" -v "${PWD}/notebooks:/home/ubuntu/notebooks" -v "${PWD}/Delight:/home/ubuntu/softdev/Delight" -v "${PWD}/RAIL:/home/ubuntu/softdev/RAIL" -d delightrail
