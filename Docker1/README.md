# Review of some concepts:

Some code is taken from https://www.youtube.com/watch?v=3c-iBn73dDE&t=5310s&ab_channel=TechWorldwithNana

To compile this code:


    1. run node server.js

--------------------------------------------------------

# How to build the network infrastructure between containers?

Docker network:

    General idea: we need to expose a physical port(HostPort) in order to allow the container to be accessible outside the docker network and defining into which network the container exist

1. let's create first a docker network:
    1. docker network create mongo_network
    2. then for each container we want to use in that network:
        1. docker run -p 27017:27017 -d --name mongodb mongo ----> **but** with other options (Environment variable to overwrite):
             1. -e MONGO_INITDB_ROOT_USERNAME=mongoadmin
             2. -e MONGO_INITDB_ROOT_PASSWORD=password
            3. and the new created network: --net mongo_network
--------------------------------------------------------
## Commands
    1. docker run -p 27017:27017 -d --name mongodb101 --net mongo_network -e MONGO_INITDB_ROOT_USERNAME=admin -e MONGO_INITDB_ROOT_PASSWORD=password mongo 
    2. docker run -p 8081:8081  -d -e ME_CONFIG_MONGODB_ADMINUSERNAME=admin -e ME_CONFIG_MONGODB_ADMINPASSWORD=password --net mongo_network --name mongo_express -e ME_CONFIG_MONGODB_SERVER=mongodb101 -e ME_CONFIG_MONGODB_URL=mongodb://mongoadmin:password@mongodb101:27017/   mongo-express

--------------------------------------------------------
# Docker-compose
Now whenever I have many services or even two but i need a more organized way to run the containers, i need to use docker-compose;
Differently from before there is no need to set up the network container since docker compose take care automatically of it (connecting the containeirs);
To know the name of the network just look at the terminal after the command;

## Commands:
1. To create the containers: 'docker-compose -f NAMEFILE.yaml up'  ("up" to run all the containers)
2. to stop all the conteiners and remove them(together with the network): 'docker-compose -f NAMEFILE.yaml down'
--------------------------------------------------------

# Dockerfile
it is used to create a docker image of an application;

Useful example:
1. FROM node
2. ENV MONGO_DB_USERNAME=admin MONGO_DB_PWD=password
3. RUN mkdir -p /home/app
4. COPY . /home/app
5. CMD ["node", "server.js"]

   
Explanations:
1. FROM is used to import the layers from another image (in this case one that has Node already installed)
2. ENV is used to set the environment variables
3. RUN is used to run any linux commands (but the effect is only inside the container)
4. COPY to copy from the local host
5. CMD to define the commands to run as the entry point after the container is run (first command, in this case is to run the command "node server.js")

# Commands:
1. docker build -t my-app:1.0 .
where
-t to give a tag
- the path of the Dockerfile

2. docker images
   to see the new image

--------------------------------------------------------

# What happens often in production?

1. commit on Git: app and the Docker file
2. Jenkins generate the docker image after intepreting the javascript
3. Jenkins push into the docker repository
4. others pull from there

# Some useful commands:
1. docker exec -it ID /bin/bash or docker exec -it ID /bin/sh
to enter the terminal inside the container denoted with ID

2. after entering it: i can use the command "env" to see the environment variables
3. the command exit to exit the terminal
4. docker rmi to remove an image after stopping the container!

-------------------------------------------------------------

# Docker Volumes
## General idea:
a container has a VIRTUAL FILE SYSTEM where the data are stored;
-> whenever i stop a container the data is gone! 
-> and if i restart i have a fresh start!

## Solution:
To solve this problem whenever we need a persistent state we need to use Docker volumes!

How?
1. Folder in the physical host file system is mounted into the virtual file system of Docker
	1. basically whenever the container writes to its virtual file system, the data gets replicated on the physical file system (this means MOUNTING A VOLUME)

2. when a container is restarted, it is populted with the data from the mounted folder;

## 3 volumes types:
1. docker run -v HostDir:ContainerDir (this type of definition is called Host Volumes)
2. docker run -v ContainerDir (these are called Anonymous Volumes) ->  These volumes are commonly used in scenarios where 
the data needs to be persistent throughout restarts of the same container, but it does not need to be shared across other ones or persist once the container is destroyed.
3. docker run -v name:ConteinerDir (called Named Volumes) -> an improvment of the Anonymous containers referencing the volume by name without knowing the exact path (these are the most used)


## How to use in a docker-compose file?

version: '3'
services:
  # my-app:
  # image: ${docker-registry}/my-app:1.0
  # ports:
  # - 3000:3000
  mongodb:
    image: mongo
    ports:
      - 27017:27017

    volumes: 
      -mongo-data:ContainerDir ## this is the path used by the mongo container to access the volume 
		(this is sth already defined in a image -> mongo-db has the virtual file system in /data/db)
   		(this is the mapping, it maps the volume created called mongo-data to the virtual file system located in containerDir  )
  mongo-express:
    image: mongo-express
    restart: always # fixes MongoNetworkError when mongodb is not ready when mongo-express starts
    ports:
      - 8080:8081
    environment:
      - ME_CONFIG_MONGODB_ADMINUSERNAME=admin
      - ME_CONFIG_MONGODB_ADMINPASSWORD=password
      - ME_CONFIG_MONGODB_SERVER=mongodb
volumes: # here all the volumes we want to define  in order to make them accessible to all the containers!  
  mongo-data:
    driver: local # it says to create the volume in the local system


 
## Where the docker volumes are stored?
it depends on the operating system!
1. Windows: C:\ProgramData\Docker\volumes
2. Linux: /var/lib/docker/volumes
3. MAC: /var/lib/docker/volumes

### Remark:
on Mac, you can't find it just running ls /var/lib/docker/volumes
Docker for MAC creates a linux virtual machine and stores all the Docker data there!
So i need to enter its terminal and then run the command!

Basically in volumes i find both the anonymous and the named volumes!
