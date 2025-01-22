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

1. commit on Git app and the Docker file
2. Jenkins generate the docker image after intepreting the javascript
3. Jenkins push into the docker repository
4. other pull from there

