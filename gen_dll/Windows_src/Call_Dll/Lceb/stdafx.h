// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement changés
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define DLL_EXPORT __declspec(dllexport)
#define SIZE 50
#define min(a,b) (a<=b?a:b)
#define max(a,b) (((a)>(b))?(a):(b))
//Liste chaînée pour enregistrer le nombre de facture en f(Value)
extern "C"
{
typedef struct counter counter;
struct counter
{
	float value;
	float sum;
	int quantity;
	int rank;
	counter* next;
};

typedef struct facture facture;
struct facture
{
	float value;
	unsigned long id;
	facture* next;
};

typedef struct interval interval;
struct interval
{
	float min;
	float max;
};

typedef struct stack stack;
struct stack
{
	stack* next;
	counter* proposition;
};

typedef struct chenille chenille;
struct chenille
{
	unsigned long quantity_entities;
	int rank;
	stack* feet;
	chenille* next;
	chenille* previous;
};

// référencer ici les en-têtes supplémentaires nécessaires à votre programme


void merging(int low, int mid, int high,float* table_value);
void sort(int low, int high,float* table_value);

//Lceb_chained_list.c ----------------------------------------------------------

//Generation
counter* generate_stone_E(float* table_value,int* table_quantity,unsigned int length);
counter* generate_stone_T(facture* in,unsigned long max_quantity);
chenille* generate_chenille_body(int max_quantity);
chenille* generate_chenille_part(int quantity,chenille* previous);
counter* filter_doubles(counter* head);
void delete_chenille(chenille* part);
void delete_counter(counter* head);
void delete_stack(stack* head);
void delete_facture(facture* head);

//Mouvement
counter* reach_counter(int rank,counter* head);
counter* reach_last_counter(counter* stone);
chenille* reach_chenille(int quantity,chenille* head);
stack* reach_stack(int rank,stack* head);
stack* reach_last_stack(stack* head);

//Informations gathering
int how_many_is_in(float* table,float value, unsigned int length);
int extract_values_from_chained_list(float* table_value,counter* head,unsigned int max_rank);
int extract_values_and_quantities_from_chained_list(float* table_value,int* table_quantity,counter* head,unsigned int max_rank);
unsigned int get_depth_of_the_chained_list(counter* head);
int get_rank_with_value(float value,counter* head);

float evaluation(counter* head);

unsigned long get_nb_values(char* file_name);
unsigned long get_nb_ID(char* file_name);
float get_price(char* file_name);

//Lceb_main.c ------------------------------------------------------------------

void road_to_the_text_file(facture* facture,chenille* head,int max_quantity);
unsigned long parse_input_json(facture* head,char* fiile_name,float* target);
void extract_values_from_json(char* filename,float* table_value,unsigned long nb_value);
void extract_ID_from_json(char* filename,unsigned long* table_id,unsigned long nb_ID);

void proposition_recursive(chenille* head,counter* stone,int max_quantity,float price);
void recursive(float* table_value,chenille* head,int* table_quantity,int* proposition,int subrank,int depth,int goal,float price);

unsigned long get_id_with_value(float value,unsigned long id,facture* input);
void my_memcpy(unsigned int* dest,unsigned int* src,unsigned int size);
void reset(char* array,int length);
