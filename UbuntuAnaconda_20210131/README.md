# README.md

# Tutorial: Docker Anaconda Python -- 4

## web adress 
- http://www.science.smith.edu/dftwiki/index.php/Tutorial:_Docker_Anaconda_Python_--_4

# Procedure

- mkdir notebooks
- docker build -t toward-data-science .

- docker run --name toward-data-science -p 8888:8888 --env="DISPLAY" \
      -v "$PWD/notebooks:/home/ubuntu/notebooks" -d toward-data-science

 - open browser at localhost:8888     

- Enter "root" as the password. Then create a simple notebook to test various libraries 