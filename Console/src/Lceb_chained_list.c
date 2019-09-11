#include "Lceb.h"

//Génère une structure counter depuis un tableau de valeurs et un tableau de quantitées et le nombre d'éléments à regarder.
counter* generate_stone_E(float* table_value,int* table_quantity,unsigned int length)
{
    //On une brique élémentaire qui est la tête de la liste
    counter* head_stone_E;
    counter* stone_E;
    int depth;

      depth = 0;
      for (int rank = 0 ; rank < length ; rank++)
        {
            //On génère une nouvelle brique élémentaire.
            stone_E = malloc(sizeof(counter));
            if (stone_E == NULL)
              {
                  perror("Generate_stone_E , échec\n");
              }
            else
              {
                stone_E->value = table_value[rank];
                stone_E->quantity = table_quantity[rank];
                stone_E->sum = (stone_E->value)*(float)(stone_E->quantity);
                stone_E->rank = depth;
                stone_E->next= NULL;
                depth++;
                //on récupère la dernière cellule de la liste chaînée et on la fait pointer sur la nouvelle celule.
                if (rank == 0)
                  head_stone_E = stone_E;
                else
                  {
                    (reach_counter(rank,head_stone_E))->next = stone_E;
                  }
              }

       }
  return head_stone_E;
}

//Génère une structure counter depuis une structure facture.
counter* generate_stone_T(facture* in,unsigned long max_quantity)
{
  counter* result_head;
  float* table_value;
  int* table_quantity;

  facture* input_retrieve = in;

  table_value = malloc(max_quantity*sizeof(float));
  table_quantity = malloc(max_quantity*sizeof(int));
  if(table_value == NULL && table_quantity == NULL)
    {
      perror("Generate_stone_T , échec\n");
    }
  else
    {
      for (int rank = 0 ; rank < max_quantity ; rank++)
        {
          table_value[rank] = input_retrieve->value;
          table_quantity[rank] = 1;
          input_retrieve = input_retrieve->next;         //////////////////////////
        }
      result_head = generate_stone_E(table_value,table_quantity,max_quantity);

      free(table_value);
      free(table_quantity);
    }
  return result_head;
}

//Génère la grosse structure , chaque bout correspond à un nombre d'éléments et contient l'ensemble des propositions à n-éléments
chenille* generate_chenille_body(int max_rank)
{
  chenille* head;
  for(int rank = 0 ; rank <= max_rank ; rank++)
    {
      if (rank == 0)
        head = generate_chenille_part(0,NULL);
      else
        (reach_chenille(rank-1,head))->next = generate_chenille_part(rank,reach_chenille(rank-1,head));
    }
  return head;
}

//Génère un bout de la structure , est appelée dans generate_body
chenille* generate_chenille_part(int quantity,chenille* previous)
{
  chenille* part;
  part = malloc(sizeof(chenille));
  if (part == NULL)
    {
      perror("Generate_chenille_part , échec\n");
    }
  else
    {
      part->rank = quantity;
      part->next = NULL;
      part->previous = previous;
      part->quantity_entities = 0;

      part->feet = malloc(sizeof(stack));
      part->feet->next = NULL;
      part->feet->proposition = NULL;
    }
  return part;
}

//Génère une structure qui contient la version sans doublon d'une liste chaînée
counter* filter_doubles(counter* head)
{
    counter* result;
    unsigned int max_rank = get_depth_of_the_chained_list(head);
    unsigned int subrank = 0;
    //float buffer_value;
    float* table_value;
    int* table_quantity;

    table_value = malloc(max_rank*sizeof(float));
    table_quantity = malloc(max_rank*sizeof(int));

    if (table_value == NULL || table_quantity == NULL)
      {
        perror("Pas d'espace pour le(s) tableau(x) filter");
        result = NULL;
      }
    else
      {
          subrank =  extract_values_and_quantities_from_chained_list(table_value,table_quantity,head,max_rank);
          //En l'état , on a 2 listes du genre : {50.0,23.95,33.0,0,0,0} et {3,2,1,x,x,x,x,} et subrank = 4;
          result = generate_stone_E(table_value,table_quantity,subrank);
          free(table_value);
          free(table_quantity);
          delete_counter(head);
      }
      return result;
}

//Free chaque cellule de la big_table donnée en argument , de n'importe où.
void delete_chenille(chenille* part)
{
  if( part->previous == NULL && part->next == NULL)
    {
      if (part->quantity_entities == 0)
        free(part->feet);
      if (part->quantity_entities > 0)
        delete_stack(part->feet);
      free(part);
    }
  chenille* down = part->next;
  chenille* up = part->previous;
  chenille* buffer = NULL;
  while (down != NULL || up != NULL)
    {
      if( down != NULL && up != NULL)
        {
          buffer = down->next;
          if (down->quantity_entities == 0)
            free(down->feet);
          if (down->quantity_entities > 0)
            delete_stack(down->feet);
          free(down);
          down = buffer;

          buffer = up->previous;
          if (up->quantity_entities == 0)
            free(up->feet);
          if (up->quantity_entities > 0)
            delete_stack(up->feet);
          free(up);
          up = buffer;
      }
    if( down == NULL && up != NULL)
      {
          buffer = up->previous;
          if (up->quantity_entities == 0)
            free(up->feet);
          if (up->quantity_entities > 0)
            delete_stack(up->feet);
          free(up);
          up = buffer;
      }
    if( down != NULL && up == NULL)
      {
          buffer = down->next;
          if (down->quantity_entities == 0)
            free(down->feet);
          if (down->quantity_entities > 0)
            delete_stack(down->feet);
          free(down);
          down = buffer;
      }
    }
}

void delete_counter(counter* head)
{
  counter* part = head;
  if( part == NULL)
    {
      free(part);
    }
  else
  {
    counter* buffer = NULL;
    while (part != NULL )
    {
        buffer = part->next;
        free(part);
        part = buffer;
    }
  }
}

void delete_stack(stack* head)
{
	stack* part = head;
	if( part->next == NULL)
		{
			delete_counter(part->proposition);
			free(part);
		}
	else
    {
      stack* buffer = NULL;
	    while (part != NULL )
		    {
				      buffer = part->next;
				      delete_counter(part->proposition);
				      free(part);
				      part = buffer;
		     }
    }
}

void delete_facture(facture* head)
{
  facture* part = head;
	if( part->next == NULL)
		{
			free(part);
		}
	else
    {
      facture* buffer = NULL;
	    while (part != NULL )
		    {
				      buffer = part->next;
				      free(part);
				      part = buffer;
		     }
    }
}

//Reach
//Pour accéder à une cellule de structure counter , (les propositions avec value/quantity associées).Ne fait que descendre
counter* reach_counter(int rank,counter* head)
{
  counter* pointeur = head;
  if (rank == 0 )
    return pointeur;
  else
    {
      while ((pointeur->rank < rank) && (pointeur->next != NULL))
        {
          pointeur = pointeur->next;
        }
      return pointeur;
    }
}

//Pour accéder directement à la dernière cellule d'une structure counter.
counter* reach_last_counter(counter* stone)
{
  counter* part = stone;
  while(part->next != NULL)
    {
      part = part->next;
    }
  return part;
}

//Pour accéder à une cellule de structure chenille , (les cellules associées à N , qui contient l'ensemble des propositions).Monte et Descend.
chenille* reach_chenille(int quantity,chenille* part)
{
  chenille* pointeur = part;
  if (quantity == part->rank )
    return pointeur;
  else
    {
      if (quantity < part->rank)
      {
        while ((pointeur->rank > quantity) && (pointeur->previous != NULL))
        {
          pointeur = pointeur->previous;
        }
      }
      else
      {
        while ((pointeur->rank < quantity) && (pointeur->next != NULL))
          {
            pointeur = pointeur->next;
          }
        }
      return pointeur;
    }
}

//Ne fait que descendre
stack* reach_stack(int rank,stack* head)
{
  int quantity = 0;
  stack* pointeur = head;
  while(quantity != rank )
    {
      quantity++;
      pointeur = pointeur->next;
    }
  return pointeur;
}

//Ne fait que descendre
stack* reach_last_stack(stack* head)
{
  stack* pointeur = head;
  if (pointeur != NULL)
    {
      while (pointeur->next != NULL)
        {
          pointeur = pointeur->next;
        }
    }
  return pointeur;
}

//Get
//Renvoie le nombre d'occurence d'un nombre dans une liste et met à 0 les cas autres que le 1er
int how_many_is_in(float* table,float value,unsigned int length)
{
  int answer = 0;
  for (unsigned int rank = 0 ; rank < length ; rank++)
    {
      if (table[rank] == value)
        {
          answer += 1;
          if (answer >= 2)
            {
              table[rank] = 0.0;
            }
        }
    }
  return answer;
}

//Renvoie le nombre de values différentes et rempli la liste passé en argument.
int extract_values_from_chained_list(float* table_value,counter* head,unsigned int max_rank)
{
    unsigned int subrank = 0;
     for (unsigned int rank = 0 ; rank < max_rank ; rank++)
       {
         table_value[rank] = (reach_counter(rank,head))->value;
       }

     for (unsigned int rank = 0 ; rank < max_rank ; rank++)
       {
         if (table_value[rank] != 0.0)
           {
             table_value[subrank] = table_value[rank];
             subrank++;
           }
       }
  return subrank;
}

//Rempli les tableaux (vides) données en paramètres avec les valeurs des différents cellules de la liste.Retourne le nombre de value différentes.
int extract_values_and_quantities_from_chained_list(float* table_value,int* table_quantity,counter* head,unsigned int max_rank)
{
     unsigned int subrank = 0;
     for (unsigned int rank = 0 ; rank < max_rank ; rank++)
       {
         table_value[rank] = (reach_counter(rank,head))->value;
         if (rank > 0)
            {sort(0,rank-1,table_value);}
       }

     for (unsigned int rank = 0 ; rank < max_rank ; rank++)
       {
         if (table_value[rank] != 0.0)
           {
             table_quantity[subrank] = how_many_is_in(table_value,table_value[rank],max_rank);
             table_value[subrank] = table_value[rank];
             subrank++;
           }
       }
  return subrank;
}

//Détermine le nombre d'éléments présents dans une liste chaînée
unsigned int get_depth_of_the_chained_list(counter* head)
{
  unsigned int rank;
  if (head == NULL)
  {
      rank = 0;
  }
  else
  {
    counter* buffer = head;
    rank = 1;
    while (buffer->next != NULL)
      {
        rank++;
        buffer = buffer->next;
      }
   }
  return rank;
}

//On récupère le rang d'une cellule en f(value) , rend -1 si la liste est vide, pas géré si elle n'est pas dans la liste
int get_rank_with_value(float value,counter* head)
{
  counter* pointeur = filter_doubles(head);
  int rank = 0;
  if( pointeur == NULL)
      {
        perror("Panique a bord c'est une liste vide");
        rank = -1;
      }
  else
    {

      while(pointeur != NULL && pointeur->value != value)
      {
        rank++;
        pointeur = pointeur->next;

      }
    }
  return rank;
}

//Rend la somme des value*quantities contenues dans la structure counter passée en argument
float evaluation(counter* head)
{
  float result = 0.0;
  unsigned int max_rank = get_depth_of_the_chained_list(head);
  for (unsigned int rank = 0 ; rank < max_rank ; rank++ )
    {
      result += reach_counter(rank,head)->sum;
    }
  return result;
}
