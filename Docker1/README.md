To compile this code:


    1. run node server.js

--------------------------------------------------------

# How to build the network infrastructure between containers?

Docker network:

    General idea: we need to expose a physical port(HostPort) in order to allow the container to be accessible outside the docker network and defining into which network the container exist

1. let's create first a docker network:
    1. docker network create mongo_network
    2. then for each container we want to use in that network:
        1. docker run -p 27017:27017 -d --name mongodb mongo as baseline right?
        2. but with other options (Environment variable to overwrite):
         1. -e MONGO_INITDB_ROOT_USERNAME=mongoadmin
         2. -e MONGO_INITDB_ROOT_PASSWORD=password
        3. and the new created network: --net mongo_network
--------------------------------------------------------
In this case:

    1. docker run -p 27017:27017 -d --name mongodb101 --net mongo_network -e MONGO_INITDB_ROOT_USERNAME=admin -e MONGO_INITDB_ROOT_PASSWORD=password mongo 
    2. docker run -p 8081:8081  -d -e ME_CONFIG_MONGODB_ADMINUSERNAME=admin -e ME_CONFIG_MONGODB_ADMINPASSWORD=password --net mongo_network --name mongo_express -e ME_CONFIG_MONGODB_SERVER=mongodb101 -e ME_CONFIG_MONGODB_URL=mongodb://mongoadmin:password@mongodb101:27017/   mongo-express



