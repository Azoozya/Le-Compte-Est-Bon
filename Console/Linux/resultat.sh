#!/bin/bash
make purge
make
make exec
read -r -p 'Debug ? Valgrind(2) ou GDB(1) ou Sans(0) , Test (666)? : ' mode
if [ $mode = 666 ];then
  path=Input.json
  ./final $path
fi

if [ $mode -lt 666 ];then
  read -r -p 'Fichier en entrée ? : ' path
  if [ $mode =  2 ]; then
    valgrind --tool=memcheck --leak-check=full --leak-resolution=high --track-origins=yes --show-reachable=yes ./final $path
  fi

  if [ $mode =  1 ]; then
     gdb --args ./final $path
  fi

  if [ $mode = 0 ];then
    ./final $path
  fi
fi

#cat Possibilities.json
