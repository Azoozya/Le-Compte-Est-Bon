#!/usr/bin/env bash
## Utilisé dans Docker/Forge/forge.sh , génère les .tar avec les codes sources nécessaires aux conteneurs
tar cvf ../Forge/Iso/Compilation-test/test.tar header/Lceb.h jsons/Input.json main/Lceb_chained_list.c main/Lceb_fonctions.c test/*
tar cvf ../Forge/Iso/Compilation-main/main.tar header/Lceb.h jsons/Input.json main/*
