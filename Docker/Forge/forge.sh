#!/usr/bin/env bash
#Générer les .tar à jour
cd ../src/
./docker.sh

#Générer les images
cd ../Forge
let img = 0
docker build --tag=main_compilation ./Iso/Compilation-main
((img=img+1))
docker build --tag=test_compilation ./Iso/Compilation-test
((img=img+1))
docker build --tag=final ./Iso/Execution
((img=img+1))

echo "$img images générées,"
echo "Images en stock :"
docker image ls
echo "Containers actifs :"
docker container ps
