FROM ubuntu:latest

# Updating Ubuntu packages
RUN apt-get update && yes|apt-get upgrade
RUN apt-get install -y emacs
RUN apt-get install -y xterm

#Adding wget and bzip2
RUN apt-get install -y wget bzip2

#adding git
RUN apt install -y git-all

#adding gcc
RUN apt install -y build-essential
RUN apt-get install -y manpages-dev


# Add sudo
RUN apt-get -y install sudo

# Add user ubuntu with no password, add to sudo group
RUN adduser --disabled-password --gecos '' ubuntu
RUN adduser ubuntu sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER ubuntu
WORKDIR /home/ubuntu/
RUN chmod a+rwx /home/ubuntu/
#RUN echo `pwd`



RUN mkdir /home/ubuntu/software ; exit 0

ADD wic-sdk-1.2.1_Ubuntu16.06-x86_64_installer.run /home/ubuntu/software/.


# Run Jupytewr notebook as Docker main process
CMD ["bash"]



