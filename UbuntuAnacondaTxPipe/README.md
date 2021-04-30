# README.md

- author : Sylvie Dagoret-Campagne
- creation date : April 30 2021
- affiliation IJCLab/IN2P3/CNRS

Example for installing and running TXPpie ubuntu docker image

- Dockerfile : instruction to build the image
- docker_cmd : set of docker commands


## commands to handle the image

-docker_cmd/docker_build.sh  : build the image from the Dockerfile (to be run from the directory containing the Dockerfile
-docker_cmd/docker_images.sh : list  existing images 
-docker_cmt/docker_rmi.sh    : delete an image 


## commands to handle the container

-docker_cmd/docker_run.sh : docker to create the container

-docker_cmd/docker_containers.sh : commands to list the container
-docker_cmd/docker_run.sh        : command to create the container form the image
-docker_cmd/docker_exec.sh       : command to enter in the container (if outside) 
-docker_cmd/docker_logs.sh       : command to view the logs inside the container (if outside)
-docker_cmd/docker_stop.sh       : command to stop the container from running
-docker_cmd/docker_rm.sh         : command to delete the container   



## Before running a container

### notebooks
notebooks directory is mounted inside the container.
> mkdir notebooks

### TXPipe

TXPIPE tutorial : https://docs.google.com/presentation/d/1haMu1eLBzfYjAlcZ-En7PdBxA4ebnWPwp8D2mCdvZV8/edit#slide=id.p

git clone --recurse-submodules https://github.com/LSSTDESC/TXPipe




## After running the container

open the Browser from outside the container

> localhost:8893

provide root as password


If build is not working do docker general cleaning
docker system prune -a 
