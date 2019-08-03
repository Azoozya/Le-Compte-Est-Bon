#!/usr/bin/env bash

#connexion à main
cd /main.tar/main
make

#connexion à test
cd /test.tar/test
make

#retour à la maison
cd /read
./exec_out
mv Possibilities.json /write/Prossibilities.json
