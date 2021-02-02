# README.md

- author : Sylvie Dagoret-Campagne
- creation date : Feb 2nd 2021
- affiliation IJCLab/IN2P3/CNRS

Example for installing and running the software wic-sdk-1.2.1_Ubuntu16.06-x86_64_installer.run under ubuntu docker image

- Dockerfile : instruction to build the image
- wic-sdk-1.2.1_Ubuntu16.06-x86_64_installer.run : workswell run file to steer the camera
- docker_cmd : set of docker commands



## commands to handle the image

-docker_cmd/docker_build.sh  : build the image from the Dockerfile (to be run from the directory containing the Dockerfile
-docker_cmd/docker_images.sh : list  existing images 
-docker_cmt/docker_rmi.sh    : delete an image 


## commands to handle the container

-docker_cmd/docker_run.sh : docher to create the container

-docker_cmd/docker_containers.sh : commands to list the container
-docker_cmd/docker_run.sh        : command to create the container form the image
-docker_cmd/docker_exec.sh       : comand to enter in the container (if outside) 
-docker_cmd/docker_logs.sh       : command to view the logs inside the container (if outside)
-docker_cmd/docker_stop.sh       : command to stop the container from running
-docker_cmd/docker_rm.sh         : command to delete the container   



# Inside the container (after build image and run container)

sudo ./wic-sdk-1.2.1_Ubuntu16.06-x86_64_installer.run
