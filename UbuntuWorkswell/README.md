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




# ce que je sais sur la camera IR

bonsoir,
comme je suis pas la demain, je vous passe les infos par ecrit, mais ce n'est pas beaucoup ... 
Le "software" est telechargeable a partir de : 
https://software.workswell.eu/wic_sdk/Windows/
On recupere un installer (Script qui telecharge et compile des choses) comme celui attache (ca a pu changer ...).

ce script va deposer des choses, sous linux, dans la zone /opt : 
*) librairies pleora ebus sdk 
en faisant ca le script essaie de compiler un module chargeable et ca echoue - mais le module n'est pas utilise. on peut oublier ca.
*) librairies de workswell pour commander la camera + 2 codes de test pour gerer la camera 
la il n'y a pas de sources, juste des shared libs et des headers c++.

pour que ca compile et que ca marche il faut setter des variables d'environnement. Ils (worksell)  donnent un script
pour ca qui ne marche pas. j'ai utilise les commandes de l'attachement 'setvars2wic'.


ensuite pour citer la "doc" (pas geniale...) vous pouvez compiler les 2 exemples donnes qui sont 
dans /opt/workswell/wic_sdk/sample/src

*) Il y a un exemple simple en C++ : WIC_SDK_Sample.cpp 
pour le compiler :

g++ -g -D _UNIX_ -D _LINUX_-std=c++11 -I /opt/pleora/ebus_sdk/Ubuntu-x86_64/include/ -I ../../include/  WIC_SDK_Sample.cpp -L/opt/workswell/wic_sdk/lib/ -lWIC_SDK -lWT-lib  -L/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib/ -lEbTransportLayerLib -lPvBase -lPvDevice -lPvBuffer -lPvGenICam -lPvTransmitter -lPvVirtualDevice -lPvAppUtils -lPvPersistence -lPvSerial -lPvStream -lm -lpthread -ljpeg -o test 

je vous donne ca pour vous eviter d'avoir a retrouver les bonnes options de compilation  

*) il y a aussi un exemple avec un GUI dans le repertoire WIK_SDK_Sample_GUI

Il s'agit d'un code developpe sous Qt - je ne connais pas trop bien. Il est decrit par le fichier avec l'extension .pro

pour fabriquer l'executable il faut generer un makefile avec qmake : qmake WIK_SDK_Sample_GUI.pro dans ce repertoire 
ca devrait genere un makefile qui peut etre compile ... mais ne compile pas. 
En effet, le code est fait pour une version recente de Qt qui n'est plus distribue pour linux . 
j'ai du modifier le fichier c++ attache pour le formal d'image utilise (pour revenir en grayscale 8 bit), qui est supporte dans la version de qt dispo sous linux, 
au lieu de 16 bits. 
apres ca a compile, avec make. 


Enfin pour runner (moi ca crashait) il faut : 
definir une IP (filaire) fixe sur le PC ou laptop 
connecter la camera directement sur le PC et voir avec quelle adresse IP  elle s'annonce et choisir pour le PC une IP proche (ex X.Y.Z.10 pour la camera, X.Y.Z.20 pour le PC - ne changer que le dernier champ). 

On peut aussi fabrique un reseau local (prive) avec un serveur dhcp, la camera va recuperer son IP avec ca. Meme reglge de priximite avec l'IP fixe du PC ; 
je n'ai pas essaye de faire tourner le seurveur dhcp sur la meme machine que celle qui pilote la camera. 

pour pouvoir dialoguer avec la camera la licences (que je vous attache, aussi) doit etre placee dans /opt/workswell/wic_sdk/sample/src/ (la ou se trouve la source des programmes de test)...

voila tout ce que je sais. bonne chance ! 
Olivier 



# History

ls
    2  cd software/
    3  
    4  cat thermalvideocamera.cpp 
    5  sudo ./wic-sdk-1.2.1_Ubuntu16.06-x86_64_installer.run
    6  cd /opt/workswell/wic_sdk/doc
    7  ls
    8  cd
    9  cd notebooks/
   10  ln -s /opt/workswell/wic_sdk/doc
   11  cd ..
   12  ls
   13  cd software/
   14  ls
   15  
   18  source setvars4wic 
   19  
   24  cd /opt/workswell/
   26  cd wic_sdk/
   28  cd sample/
   29  
   39  g++ -g -D UNIX -D LINUX-std=c++11 -I /opt/pleora/ebus_sdk/Ubuntu-x86_64/include/ -I ../../include/ WIC_SDK_Sample.cpp -L/opt/workswell/wic_sdk/lib/ -lWIC_SDK -lWT-lib -L/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib/ -lEbTransportLayerLib -lPvBase -lPvDevice -lPvBuffer -lPvGenICam -lPvTransmitter -lPvVirtualDevice -lPvAppUtils -lPvPersistence -lPvSerial -lPvStream -lm -lpthread -ljpeg -o test
   40  ls
   41  sudo apt-get update -y
   42  sudo apt-get install -y libjpeg-dev
   47  sudo apt-get install vim
   48  ls
   49  
   54  g++ -g -D UNIX -D LINUX-std=c++11 -I /opt/pleora/ebus_sdk/Ubuntu-x86_64/include/ -I ../../include/ WIC_SDK_Sample.cpp -L/opt/workswell/wic_sdk/lib/ -lWIC_SDK -lWT-lib -L/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib/ -lEbTransportLayerLib -lPvBase -lPvDevice -lPvBuffer -lPvGenICam -lPvTransmitter -lPvVirtualDevice -lPvAppUtils -lPvPersistence -lPvSerial -lPvStream -lm -lpthread -ljpeg -o test
   55  
   58  cd WIC_SDK_Sample_GUI/
   59  ls
   60  qmake WIK_SDK_Sample_GUI.pro
   61  
   63  
   65  sudo apt-get install qt-sdk
   66  
   68  apt-get install libmesa-dev
   69  
   71  sudo apt-get install mesa-utils
   72  
   73  sudo apt install mesa-common-dev
   74  make
   75  
   77  ls
   78  cp thermalvideocamera.cpp thermalvideocamera.cpp_old
   79  cp $HOME/software/thermalvideocamera.cpp .
   80  make
   81  


