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
In this case:
    1. docker run -p 27017:27017 -d --name mongodb101 --net mongo_network -e MONGO_INITDB_ROOT_USERNAME=admin -e MONGO_INITDB_ROOT_PASSWORD=password mongo 
    2. docker run -p 8081:8081  -d -e ME_CONFIG_MONGODB_ADMINUSERNAME=admin -e ME_CONFIG_MONGODB_ADMINPASSWORD=password --net mongo_network --name mongo_express -e ME_CONFIG_MONGODB_SERVER=mongodb101 -e ME_CONFIG_MONGODB_URL=mongodb://mongoadmin:password@mongodb101:27017/   mongo-express

--------------------------------------------------------
# Docker-compose
Now whenever I have many services or even two but i need a more organized way to run the containers, i need to use docker-compose;

Basically is a file run by 'docker-compose -f NAMEFILE.yaml up' :
1. "up" to run all the containers

Differently from before there is no need to set up the network container since docker compose take care automatically of it (connecting the containeirs);
To know the name of the network just look at the terminal after the command;

to stop all the conteiners: 'docker-compose -f NAMEFILE.yaml down'

