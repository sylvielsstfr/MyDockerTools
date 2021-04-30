# We will use Ubuntu for our image
# guide taken from http://www.science.smith.edu/dftwiki/index.php/Tutorial:_Docker_Anaconda_Python_--_4
FROM ubuntu:latest

# MÉTADONNÉES DE L'IMAGE
LABEL version="1.0" maintainer="DAGORET-CAMPAGNE sylvie <dagoret.lal.in2p3.fr>"

# Updating Ubuntu packages
RUN apt-get update && yes|apt-get upgrade
RUN apt-get clean 
RUN apt-get install -y emacs
RUN apt-get install -y xterm
RUN apt-get install -y x11-apps

#Adding wget and bzip2
RUN apt-get install -y wget bzip2

#adding git
RUN apt install -y git

#adding gcc
RUN apt install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y manpages-dev
RUN apt-get install -y pkg-config

#adding gfortran
RUN apt-get install -y gfortran


# add more
RUN apt-get install -y swig


RUN apt-get install -y libfftw3-dev libfftw3-doc
RUN apt-get install -y libgsl-dev

RUN apt-get install -y libblas-dev checkinstall
#RUN apt-get install -y libblas-doc checkinstall
RUN apt-get install -y liblapacke-dev checkinstall
#RUN apt-get install -y liblapacke-doc checkinstall

RUN apt-get install -y libsharp-dev


# Add sudo
RUN apt-get -y install sudo

# Add user ubuntu with no password, add to sudo group
RUN adduser --disabled-password --gecos '' ubuntu
RUN adduser ubuntu sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER ubuntu
WORKDIR /home/ubuntu/
RUN chmod a+rwx /home/ubuntu/
RUN echo `pwd`

# Anaconda installing
RUN wget https://repo.continuum.io/archive/Anaconda3-2020.11-Linux-x86_64.sh
RUN bash Anaconda3-2020.11-Linux-x86_64.sh -b
RUN rm Anaconda3-2020.11-Linux-x86_64.sh

# Set path to conda
#ENV PATH /root/anaconda3/bin:$PATH
ENV PATH /home/ubuntu/anaconda3/bin:$PATH

# Updating Anaconda packages
RUN conda update conda
RUN conda update anaconda
RUN conda update --all
RUN conda install --yes cython numpy scipy pytest pylint pycodestyle coveralls matplotlib astropy mpi4py sphinx coloredlogs
RUN conda install --yes -c conda-forge healpy 
RUN conda install --yes h5py pyyaml pyarrow
RUN conda install --yes -c conda-forge emcee 
RUN conda install --yes -c conda-forge corner
RUN conda install --yes -c conda-forge fitsio 
RUN pip install tables

# install TXPipe in the container
ADD TXPipe TXPipe
USER root
WORKDIR /home/ubuntu/TXPipe
RUN chmod a+rwx /home/ubuntu/TXPipe
#RUN pip install --no-binary=mpi4py,healpy -r requirements.txt
RUN pip install -r requirements.txt



USER ubuntu
WORKDIR /home/ubuntu

# softxare developmment zone
RUN mkdir /home/ubuntu/softdev ; exit 0
RUN mkdir /home/ubuntu/softdev/TXPipe ; exit 0
ADD TXPipe /home/ubuntu/TXPipe

#Configuring access to Jupyter
RUN mkdir /home/ubuntu/notebooks ; exit 0
RUN jupyter notebook --generate-config --allow-root
RUN echo "c.NotebookApp.password = u'sha1:6a3f528eec40:6e896b6e4828f525a6e20e5411cd1c8075d68619'" >> /home/ubuntu/.jupyter/jupyter_notebook_config.py

# Jupyter listens port: 8893
EXPOSE 8893

# Run Jupytewr notebook as Docker main process
CMD ["jupyter", "notebook", "--allow-root", "--notebook-dir=/home/ubuntu/notebooks", "--ip='*'", "--port=8893", "--no-browser"]



