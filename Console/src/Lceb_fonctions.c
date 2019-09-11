#include "Lceb.h"

///////////////////////////////////////////////////////////////////////////////
//Prend en entrée le pointeur sur structure chenille initialisé, pointeur sur la facture format counter déjà rempli et déjà filtrée.
void proposition_recursive(chenille* head,counter* facture,int max_quantity,float price)
{
  float* table_value = malloc(max_quantity*sizeof(float));
  int* table_quantity = malloc(max_quantity*sizeof(int));
  if (table_value == NULL || table_quantity == NULL)
      {
        perror("Pas d'espace pour stocker les valeurs de références\n");
      }
  else
      {
      unsigned int subrank = extract_values_and_quantities_from_chained_list(table_value,table_quantity,facture,max_quantity);
      int* proposition = malloc(subrank*sizeof(int));
      if (proposition == NULL)
        {
          perror("Pas d'espace pour stocker des propositions\n");
        }
      else
        {
          unsigned int depth = 0;
          int goal = max_quantity;
          int purpose = table_quantity[depth];

          proposition[depth] = purpose;
          recursive(table_value,head,table_quantity,proposition,subrank,depth,goal,price);
          free(proposition);
        }
      free(table_value);
      free(table_quantity);
      }
}

void recursive(float* table_value,chenille* head,int* table_quantity,int* proposition,int subrank,int depth,int goal,float price)
{
  int sum_qty = 0;
  int goal_save = goal;
  int* table;
  chenille* chenille_part;
  stack* stack_part;
  counter* counter_proposition;

  if ((depth == (subrank)) && (goal <= table_quantity[depth]))
    {
      proposition[depth] = goal;
      for (int rank = 0 ; rank < subrank ; rank++)
        sum_qty += proposition[rank];
     table = malloc(subrank*sizeof(int));
     if (table == NULL)
      {
        perror("Pas d'espace pour stocker la table des quantitées pour la proposition.\n");
      }
    else
      {
        my_memcpy((unsigned int*)table,(unsigned int*)proposition,subrank);

        counter_proposition = generate_stone_E(table_value,table,subrank);
        if (evaluation(counter_proposition) == price)
          {
            chenille_part = reach_chenille(sum_qty,head);
            if(chenille_part->quantity_entities == 0)
              {
                chenille_part->feet->proposition = counter_proposition;
                chenille_part->feet->next = NULL;
              }
            else
              {
                stack_part = malloc(sizeof(stack));
                stack_part->proposition = counter_proposition;
                stack_part->next = NULL;
                (reach_last_stack(chenille_part->feet))->next = stack_part;
              }
            if (chenille_part->quantity_entities < 0)
              printf("Quantity entities is negative : [%d] %ld\n",sum_qty,chenille_part->quantity_entities);
           chenille_part->quantity_entities++;
          }
        else
          {
            free(counter_proposition);
          }
        free(table);
      }
    }

  if ((depth == subrank) && (goal > table_quantity[depth]))
    {
         proposition[depth] = table_quantity[subrank-depth];
         for (int rank = 0 ; rank < subrank ; rank++)
           sum_qty += proposition[rank];
         table = malloc(subrank*sizeof(int));
         if (table == NULL)
          {
            perror("Pas d'espace pour stocker la table des quantitées pour la proposition.\n");
          }
        else
          {
            my_memcpy((unsigned int*)table,(unsigned int*)proposition,subrank);

            counter_proposition = generate_stone_E(table_value,table,subrank);
            if (evaluation(counter_proposition) == price)
              {
                chenille_part = reach_chenille(sum_qty,head);
                if(chenille_part->quantity_entities == 0)
                  {
                    chenille_part->feet->proposition = counter_proposition;
                    chenille_part->feet->next = NULL;
                  }
                else
                  {
                    stack_part = malloc(sizeof(stack));
                    stack_part->proposition = counter_proposition;
                    stack_part->next = NULL;
                    (reach_last_stack(chenille_part->feet))->next = stack_part;
                  }
                if (chenille_part->quantity_entities < 0)
                  printf("Quantity entities is negative : [%d] %ld\n",sum_qty,chenille_part->quantity_entities);
                chenille_part->quantity_entities++;
              }
            else
              {
                free(counter_proposition);
              }
              free(table);
            }
          }

  if (depth < subrank)
    {
         for (int purpose = table_quantity[depth] ; purpose >= 0 ; purpose--)
           {
             if (goal >= purpose)
             {
               proposition[depth] = purpose;
               goal -= purpose;
               recursive(table_value,head,table_quantity,proposition,subrank,depth+1,goal,price);
             }
             goal = goal_save;
           }
        }
}

void road_to_the_text_file (facture* facture,chenille* head,int max_quantity)
{
  FILE* Out;
  Out = fopen("Possibilities.json","w");
  chenille* chenille_part;
  stack* stack_part;
  counter* cell;
  unsigned long id;
  int subrank;
  int qty_value;
  int qty_element;

  fprintf(Out,"{\n");
  for (int rank = 1 ; rank <= max_quantity ; rank++ )
    {
      chenille_part = reach_chenille(rank,head);
      if (chenille_part->quantity_entities == 0)
        {
          fprintf(Out,"\t\t\t%c%d_element%c:{\t},\n\n",'"',rank,'"');
        }
      if (chenille_part->quantity_entities > 0)
        {
          fprintf(Out,"\t\t\t%c%d_element%c:{\n",'"',rank,'"');
          //Value:
          fprintf(Out,"\t\t\t\t%cvalue%c:[\n",'"','"');
          for (int depth = 0 ; depth < chenille_part->quantity_entities ; depth++)
            {
              fprintf(Out,"\t\t\t\t\t\t\t\t[");
              stack_part = reach_stack(depth,chenille_part->feet);
              subrank = 0;
              qty_element = 1;
              do{
                cell = reach_counter(subrank,stack_part->proposition);
                qty_value = 1;
                if (cell->value != 0.0 && cell->quantity != 0)
                  {
                    while( qty_value <= cell->quantity)
                      {
                        fprintf(Out,"%f",cell->value);
                        qty_value++;
                        if (qty_element != rank)
                          {
                            fprintf(Out,",");
                            qty_element++;
                          }
                      }
                  }
                subrank++;
                }while(cell != reach_last_counter(stack_part->proposition));
              fprintf(Out,"]");
              if(depth != (chenille_part->quantity_entities-1))
                fprintf(Out,",\n");
              else
                fprintf(Out,"\n");
            }
          fprintf(Out,"\t\t\t\t\t\t\t\t],\n");
          //ID:
          fprintf(Out,"\t\t\t\t\t%cID%c:[\n",'"','"');
          for (int depth = 0 ; depth < chenille_part->quantity_entities ; depth++)
            {
              fprintf(Out,"\t\t\t\t\t\t\t\t[");
              stack_part = reach_stack(depth,chenille_part->feet);
              subrank = 0;
              qty_element = 1;

              do{
                cell = reach_counter(subrank,stack_part->proposition);
                qty_value = 1;
                id = 0;
                if (cell->value != 0.0 && cell->quantity != 0)
                  {
                    while( qty_value <= cell->quantity)
                      {
                        id = get_id_with_value(cell->value,id,facture);
                        fprintf(Out,"%lu",id);
                        qty_value++;
                        if (qty_element != rank)
                          {
                            fprintf(Out,",");
                            qty_element++;
                          }
                      }
                  }
                subrank++;
                }while(cell != reach_last_counter(stack_part->proposition));
              fprintf(Out,"]");
              if(depth != (chenille_part->quantity_entities-1))
                fprintf(Out,",\n");
              else
                fprintf(Out,"\n");
            }
          fprintf(Out,"\t\t\t\t\t\t\t ],\n");
          fprintf(Out,"\t\t\t\t\t\t\t\t\t},\n\n");
        }
      if (chenille_part->quantity_entities < 0)
        printf("[c->json] Error , quantity_entities < 0\n");
    }
    fprintf(Out,"\n},%c",'\0');
    fclose(Out);
}

//renvoie la première occurence après l'ID donnée de la value passée en argument dans la structure facture (trié dans l'ordre croissant)
unsigned long get_id_with_value(float value,unsigned long id,facture* input)
{
  facture* in = input;
  if (in == NULL)
    return 666;
  while(in != NULL && !(in->value == value && in->id > id))
    {
        in = in->next;
    }
  return in->id;
}


void my_memcpy(unsigned int* dest,unsigned int* src,unsigned int size)
{
  for (int rank = 0 ; rank < size ; rank++)
    {
      dest[rank] = src[rank];
    }
}

// Entrée : pointeur sur structure facture déjà alloué,pointeur sur un float , le nom du fichier d'entrée , sortie : nombre de factures; Génère la structure facture entière et assigne à price la valeur contenu dans le json.
unsigned long parse_input_json(facture* head, char* filename, float* target)
{
  unsigned long nb_value = get_nb_values(filename);
  unsigned long nb_ID = get_nb_ID(filename);
  float* table_value;
  unsigned long* table_id;
  facture* cursor;
  facture* stone_T;

  if (nb_value != nb_ID)
    {
      return ERROR;
    }
  else if (nb_value == nb_ID && nb_value == 0)
    {
      return NOTHING;
    }
  else
    {
      table_value = malloc(nb_value*sizeof(float));
      table_id = malloc(nb_ID*sizeof(unsigned long));

      extract_values_from_json(filename,table_value,nb_value);
      extract_ID_from_json(filename,table_id,nb_ID);
      *target = get_price(filename);

      for (unsigned long rank = 0 ; rank < nb_value ; rank++)
          {
            //On génère une nouvelle brique élémentaire.
            if (rank == 0)
              {
                head->value = table_value[rank];
                head->id = table_id[rank];
                head ->next = NULL;
                cursor = head;
              }
            else
              {
                stone_T = malloc(sizeof(facture));
                if (stone_T == NULL)
                  {
                    perror("Parsing du json immpossible , element non généré\n");
                  }
                else
                  {
                    stone_T->value = table_value[rank];
                    stone_T->id = table_id[rank];
                    stone_T->next = NULL;
                    cursor->next = stone_T;
                    cursor = stone_T;
                  }
              }
          }

      free(table_value);
      free(table_id);
      return nb_value;
    }
}

unsigned long get_nb_values(char* file_name)
{
  unsigned long nb_elements = 0;
  char element;
  short indicator =0;
  FILE* In;
  In = fopen(file_name,"r");
  while (!(indicator) && element != '}')
  {
      do{
        element = getc(In);
      }while(element != '"' && element != '}');
      element = getc(In);
      if (element == 'V' || element == 'v')
      {
        element = getc(In);
        if (element == 'A' || element == 'a')
          {
            element = getc(In);
            if (element == 'L' || element == 'l')
              {
                element = getc(In);
                if (element == 'U' || element == 'u')
                  {
                    element = getc(In);
                    if (element == 'E' || element == 'e')
                      {
                        element = getc(In);
                        if (element == '"')
                          {
                            indicator = 1;
                          }
                      }
                  }
              }
          }
      }

    if(!(indicator))
      {
        do{
          element = getc(In);
          printf("%c\n",element);
        }while(element != ']' && element != '}');
      }
  }

  if (element != '}')
    {
      do{
        element = getc(In);
      }while(element != '[');

      do{
        element = getc(In);
        if(element != ']')
          {
            nb_elements++;
            while (element != ',' && element != ']')
              {
                element = getc(In);
              }
          }
      }while(element != ']');
    }

  fclose(In);
  return nb_elements;
}

unsigned long get_nb_ID(char* file_name)
{
  unsigned long nb_elements = 0;
  char element;
  short indicator = 0;
  FILE* In;
  In = fopen(file_name,"r");
  while (!(indicator) && element != '}')
  {
      do{
        element = getc(In);
      }while(element != '"' && element != '}');

      element = getc(In);
      if (element == 'I' || element == 'i')
        {
          element = getc(In);
          if (element == 'D' || element == 'd')
            {
              element = getc(In);
              if (element == '"')
                {
                  indicator = 1;
                }
            }
        }

    if(!(indicator))
      {
        do{
          element = getc(In);
        }while(element != ']' && element != '}');
      }
  }

  if (element != '}')
    {
      do{
        element = getc(In);
      }while(element != '[');

      do{
        element = getc(In);
        if(element != ']')
          {
            nb_elements++;
            while (element != ',' && element != ']')
              {
                element = getc(In);
              }
          }
      }while(element != ']');
    }

  fclose(In);
  return nb_elements;
}

float get_price(char* file_name)
{
  float price = 1.0;
  int nb_digits = 0;
  char element;
  FILE* In;
  In = fopen(file_name,"r");
  short indicator = 0;
  char buffer[50];
  while (!(indicator) && element != '}')
    {
      do{
        element = getc(In);
      }while(element != '"' && element != '}');

      element = getc(In);
      if (element == 'P' || element == 'p')
        {
          element = getc(In);
          if (element == 'R' || element == 'r')
            {
              element = getc(In);
              if (element == 'I' || element == 'i')
                {
                  element = getc(In);
                  if (element == 'C' || element == 'c')
                    {
                      element = getc(In);
                      if (element == 'E' || element == 'e')
                        {
                          element = getc(In);
                          if (element == '"')
                            {
                              element = getc(In);
                              if (element == ':')
                                {
                                  indicator = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

    if(!(indicator))
      {
        do{
          element = getc(In);
        }while(element != ']' && element != '}');
      }
    }

  if (element != '}')
    {
      while (element != '.' && element != '}')
        {
          element = getc(In);
          if(element != ' ')
            {
              buffer[nb_digits] = element;
              nb_digits++;
            }
        }
      for (int depth = 1 ; depth <= 6 ; depth++)
        {
          if(element != '\n')
            {
              element = getc(In);
              buffer[nb_digits+depth] = element;
            }
          if(element == '\n')
            {
              buffer[nb_digits+depth] = '0';
            }
        }
      }

  price = atof(buffer);
  fclose(In);
  return price;
}

void extract_values_from_json(char* filename,float* table_value,unsigned long nb_value)
{
  char element;
  unsigned long nb_elements = 0;
  unsigned long rank = 0;
  short precision = 0;
  short indicator = 0;
  char buffer[SIZE];
  FILE* In;
  In = fopen(filename,"r");
  while (!(indicator) && element != '}')
  {
      do{
        element = getc(In);
      }while(element != '"' && element != '}');
      element = getc(In);
      if (element == 'V' || element == 'v')
      {
        element = getc(In);
        if (element == 'A' || element == 'a')
          {
            element = getc(In);
            if (element == 'L' || element == 'l')
              {
                element = getc(In);
                if (element == 'U' || element == 'u')
                  {
                    element = getc(In);
                    if (element == 'E' || element == 'e')
                      {
                        element = getc(In);
                        if (element == '"')
                          {
                            indicator = 1;
                          }
                      }
                  }
              }
          }
      }

    if(!(indicator))
      {
        do{
          element = getc(In);
          printf("%c\n",element);
        }while(element != ']' && element != '}');
      }
  }

  if (element != '}')
    {
      do{
        element = getc(In);
      }while(element != '[');
      do{
        element = getc(In);
        precision = 0;
        nb_elements = 0;
        while(element != ',' && element != ']')
          {
            while(element != '.')
              {
                buffer[nb_elements] = element;
                nb_elements++;
                element = getc(In);
              }
            buffer[nb_elements] = element;
            nb_elements++;
            while (element != ',' && element != ']')
            {
              element = getc(In);
              if(element != ',' && element != ']')
                {
                  buffer[nb_elements] = element;
                  nb_elements++;
                  precision++;
                }
            }
          }
        if (precision < 6)
         {
           for(int depth = precision ; precision <= 6 ; depth++)
            {
              buffer[nb_elements] = 0;
              nb_elements++;
            }
          }
          table_value[rank] = atof(buffer);
          // for(int rank = 0 ; rank < SIZE ; rank++)
          //   {
          //     printf("%c|",buffer[rank]);
          //   }
          // printf("\n");
          // printf("%f\n",table_value[rank]);
          reset(buffer,SIZE);
          rank++;
      }while(element != ']');
    }
fclose(In);
}

void extract_ID_from_json(char* filename,unsigned long* table_id,unsigned long nb_ID)
{
  char element;
  unsigned long nb_elements = 0;
  unsigned long rank = 0;
  short indicator = 0;
  char buffer[SIZE];
  FILE* In;
  In = fopen(filename,"r");
  while (!(indicator) && element != '}')
    {
      do{
        element = getc(In);
      }while(element != '"' && element != '}');

      element = getc(In);
      if (element == 'I' || element == 'i')
        {
          element = getc(In);
          if (element == 'D' || element == 'd')
            {
              element = getc(In);
              if (element == '"')
                {
                  indicator = 1;
                }
            }
        }

    if(!(indicator))
      {
        do{
          element = getc(In);
        }while(element != ']' && element != '}');
      }
  }

  if (element != '}')
    {
      do{
        element = getc(In);
      }while(element != '[');
      do{
        element = getc(In);
        if(element == ',' || element == ']')
            {
              buffer[nb_elements] = '.';
              table_id[rank] = atoi(buffer);
              rank++;
              reset(buffer,SIZE);
              nb_elements = 0;
            }
        else
            {
              buffer[nb_elements] = element;
              nb_elements++;
            }
         }while(element != ']');
     }
fclose(In);
}

void reset(char* array,int length)
{
  for (int rank = 0 ; rank < length ; rank++)
    {
      array[rank] = '0';
    }
}

void merging(int low, int mid, int high,float* table_value)
{
   int l1, l2, i;
   float* buffer_value = malloc((high+1)*sizeof(float));
   if (buffer_value == NULL)
	   	 perror("Merging Imppossible.\n");
   else
   {
	   for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++)
	   {
		   if(table_value[l1] <= table_value[l2])
		   	   {
			   	   buffer_value[i] = table_value[l1++];
		   	   }
		   else
		   	   {
			   	   buffer_value[i] = table_value[l2++];
		   	   }
	   }

	   while(l1 <= mid)
	   	   {
		   	   buffer_value[i++] = table_value[l1++];
	   	   }

	   while(l2 <= high)
	   	   {
		   	   buffer_value[i++] = table_value[l2++];
	   	   }
	   for(i = low; i <= high; i++)
	   	   {
		   	   table_value[i] = buffer_value[i];
	   	   }
  free(buffer_value);
   }
}

void sort(int low, int high,float* table_value)
{
   int mid;

   if(low < high)
   {
      mid = (low + high) / 2;
      sort(low, mid,table_value);
      sort(mid+1, high,table_value);
      merging(low, mid, high,table_value);
   }
}
