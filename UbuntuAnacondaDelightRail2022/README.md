# README.md

- author : Sylvie Dagoret-Campagne
- creation date : Oct 20 2021
- update : October 26th 2021 : use October 2021 RAIL version (issue/101/delight)
- affiliation IJCLab/IN2P3/CNRS

Example for installing and running Delight + RAIL ubuntu docker image

- Dockerfile : instruction to build the image
- docker_cmd : set of docker commands


## commands to handle the image

-docker_cmd/docker_build.sh  : build the image from the Dockerfile (to be run from the directory containing the Dockerfile
-docker_cmd/docker_images.sh : list  existing images 
-docker_cmt/docker_rmi.sh    : delete an image 
-docker_cmt/docker_prune.sh  : command to delete all images (docker system prune -a )


## commands to handle the container

-docker_cmd/docker_run.sh : docker to create the container

-docker_cmd/docker_containers.sh : commands to list the container
-docker_cmd/docker_run.sh        : command to create the container form the image
-docker_cmd/docker_exec.sh       : command to enter in the container (if outside virtual environnement) 
-docker_cmd/docker_logs.sh       : command to view the logs inside the container (if outside virtual environnement)
-docker_cmd/docker_stop.sh       : command to stop the container from running
-docker_cmd/docker_rm.sh         : command to delete the container   


## sequential order of docker commands to launch to build the image and then run the container and work

### 1) Create the image
from the top directory which contain the Dockerfile and the docker_cmd directory:
- build the image "delightrail2022" according the Dockerfile:
> sh docker_cmd/docker_build.sh 

In principle Delight and RAIL are built when building the image "delightrail2022" under root priviledges. 
If it fails, correct the Dockerfile and run again. If stuck, erase the image by :
> docker_cmt/docker_rmi.sh  
or more bruptly
> docker_cmt/docker_prune.sh 

check the images has been created
>sh docker_cmd/docker_images.sh 

### 2) Create the container

- create a container "delightrail2022c" from the image delightrail2022" and run it :
> sh docker_cmd/docker_run.sh

access control disabled, clients can connect from any host
192.168.0.43 being added to access control list
1c07c25a3f4445344a41d59e3b5f43630b97f0eaa609240e8692219ee54a001f

Check the container has been created and is running from the host
> sh docker_cmd/docker_containers.sh 
Check the last commands run in the container:
> sh docker_cmd/docker_logs.sh

- enter inside the environnement of the container "delightrail2022c"
> sh docker_cmd/docker_exec.sh


### 3) Work inside the container

Now we are inside the container !
To run a command as administrator (user "root"), use "sudo <command>".
See "man sudo_root" for details.

ubuntu@1c07c25a3f44:~$ ls -l
total 28
drwxrwxrwx 1 root   root   4096 Oct 26 11:36 Delight
drwxrwxrwx 1 root   root   4096 Oct 26 11:40 RAIL
drwxr-xr-x 1 ubuntu ubuntu 4096 Oct 20 19:42 anaconda3
drwxr-xr-x 2 root   root     64 Oct 20 14:02 notebooks
drwxr-xr-x 1 ubuntu ubuntu 4096 Oct 26 11:40 softdev
ubuntu@1c07c25a3f44:~$ cd softdev
ubuntu@1c07c25a3f44:~/softdev$ ls -l
total 0
drwxr-xr-x 24 root root 768 Oct 26 10:17 Delight
drwxr-xr-x 15 root root 480 Oct 26 10:20 RAIL

It is only possible to work under softdev which is under ubuntu owner
> cd softdev/Delight   (Not mandatory if you don't edit Delight)
ubuntu@1c07c25a3f44:~/softdev/Delight$ python setup.py build_ext
ubuntu@1c07c25a3f44:~/softdev/Delight$ python setup.py install --user  (don't know which I cannot write in anaconda3)

> cd softdev/RAIL
ubuntu@1c07c25a3f44:~/softdev/RAIL$ python setup.py install --user
ubuntu@1c07c25a3f44:~/softdev/RAIL$ cd examples/estimation/

- check randomPZ is working:
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation$ python main.py configs/randomPZ.yaml 

- Prepare DelightPZ : 

- create temporary dirs tmp and tmpsim with Delight FILTERS and SED
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation$ mkdir tmp
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation$ mkdir tmpsim
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation$ cd tmp
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmp$ cp -r $HOME/softdev/Delight/data/* delight_indata/.
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmpsim$ cd ../tmp/delight_indata/
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmp/delight_indata$ ls
BROWN_SEDs  CWW_SEDs  FILTERS
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/e
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmp$ cd ../tmpsim
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmpsim$ mkdir delight_indata
ubuntu@1c07c25a3f44:~/softdev/RAIL/examples/estimation/tmpsim$ cp -r $HOME/softdev/Delight/data/* delight_indata/.


- Fast check with delight internal simulation in Delight (could becone a test)
ubuntu@b738a78de52a:~/softdev/RAIL/examples/estimation$ python main.py configs/delightsimPZ.yaml 

- Run RAIL/delightPZ using DC2 data (long time)
python main.py configs/delightPZ.yaml 

- backup the results under the local notebook directory
We must save Delight results in notebook directory for later analysis

ubuntu@b738a78de52a:~/softdev/RAIL/examples/estimation$ cp -r tmp $HOME/notebooks/.
ubuntu@b738a78de52a:~/softdev/RAIL/examples/estimation$ cp -r tmpsim $HOME/notebooks/.
ubuntu@b738a78de52a:~/softdev/RAIL/examples/estimation$ cd $HOME/notebooks
ubuntu@b738a78de52a:~/notebooks$ ls
tmp  tmpsim
ubuntu@b738a78de52a:~/notebooks$ 


- exit from the environnement
ubuntu@b738a78de52a:~/notebooks$ exit   

- check the container is still running
> sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE             COMMAND                  CREATED          STATUS          PORTS                                       NAMES
b738a78de52a   delightrail2022   "jupyter notebook --…"   49 minutes ago   Up 49 minutes   0.0.0.0:8891->8891/tcp, :::8891->8891/tcp   delightrail2022c

As long as the containner is still running nothing is lost.
- to reenter again in the container

> sh docker_cmd/docker_exec.sh 

- to quit again without stopping the container
ubuntu@b738a78de52a:~/softdev/RAIL/examples/estimation$ exit
exit
/Users/dagoret/MacOSX/GitHub/LSST/MyDockerTools/UbuntuAnacondaDelightRail2022>sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE             COMMAND                  CREATED             STATUS             PORTS                                       NAMES
b738a78de52a   delightrail2022   "jupyter notebook --…"   About an hour ago   Up About an hour   0.0.0.0:8891->8891/tcp, :::8891->8891/tcp   delightrail2022c

- to stop the container (nothing lost)
> sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE             COMMAND                  CREATED             STATUS             PORTS                                       NAMES
b738a78de52a   delightrail2022   "jupyter notebook --…"   About an hour ago   Up About an hour   0.0.0.0:8891->8891/tcp, :::8891->8891/tcp   delightrail2022c
> sh docker_cmd/docker_stop.sh
delightrail2022c
/Users/dagoret/MacOSX/GitHub/LSST/MyDockerTools/UbuntuAnacondaDelightRail2022>sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE             COMMAND                  CREATED             STATUS                      PORTS     NAMES
b738a78de52a   delightrail2022   "jupyter notebook --…"   About an hour ago   Exited (0) 20 seconds ago             delightrail2022c

- to start again the container (nothing lost)
> sh docker_cmd/docker_start.sh 
delightrail2022c
>sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE             COMMAND                  CREATED             STATUS         PORTS                                       NAMES
b738a78de52a   delightrail2022   "jupyter notebook --…"   About an hour ago   Up 4 seconds   0.0.0.0:8891->8891/tcp, :::8891->8891/tcp   delightrail2022c

- to remove definitely the container (data lost)
> sh docker_cmd/docker_stop.sh 
delightrail2022c
> sh docker_cmd/docker_rm.sh 
delightrail2022c
> sh docker_cmd/docker_containers.sh 
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

- next time to relaunch the container
> sh docker_cmd/docker_run.sh

- to clean the image (no container)
>sh docker_cmd/docker_rmi.sh
Untagged: delightrail2022:latest
Deleted: sha256:46a2395850eb21b247602a25397e09f664462d48dc03a47f4bd4c2a1179f7641


## Before building the image and running the container get the packages (from the HOST):

- create some directories and clone the packages from the top directory (same level as Dockerfile and docker_cmd dir)

### notebooks
notebooks directory is mounted inside the container.
> mkdir notebooks
the interest of this is to allow exchange of files between the host and the virtual environnement

if we want to make analysis of Delight output:
> cd notebooks
git clone https://github.com/sylvielsstfr/PhotoZAna

### set delight
git clone git@github.com:sylvielsstfr/Delight.git
cd Delight
git checkout desc_rail


### rail
git clone git@github.com:LSSTDESC/RAIL.git
cd RAIL
>git checkout issue/101/delight


## After running the container, after having copied results (tmp/ and tmpsim/ in directory notebooks/),access via jupyter to some notebooks (file files to edit)

If the container is still running (not stopped or removed)
open the Browser from outside the container (from the host) and provide the web adress :

> localhost:8891

provide root as password




