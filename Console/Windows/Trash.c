unsigned int prime_decomposition[][11] = {{1,2,3,5,7,11,13,17,19,23,29},{1,0,0,0,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,0,0,0,0},{1,0,1,0,0,0,0,0,0,0,0},{1,2,0,0,0,0,0,0,0,0,0},{1,0,0,1,0,0,0,0,0,0,0},{1,1,1,0,0,0,0,0,0,0,0},{1,0,0,0,1,0,0,0,0,0,0},{1,3,0,0,0,0,0,0,0,0,0},{1,0,2,0,0,0,0,0,0,0,0},{1,1,0,1,0,0,0,0,0,0,0},{1,0,0,0,0,1,0,0,0,0,0},{1,2,1,0,0,0,0,0,0,0,0},{1,0,0,0,0,0,1,0,0,0,0},{1,1,0,0,1,0,0,0,0,0,0},{1,0,1,1,0,0,0,0,0,0,0},{1,4,0,0,0,0,0,0,0,0,0},{1,1,0,0,0,0,0,1,0,0,0},{1,1,2,0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,1,0,0},{1,2,0,1,0,0,0,0,0,0,0},{1,0,1,0,1,0,0,0,0,0,0},{1,1,0,0,0,1,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,1,0},{1,3,1,0,0,0,0,0,0,0,0},{1,0,0,2,0,0,0,0,0,0,0},{1,0,0,0,0,0,2,0,0,0,0},{1,0,3,0,0,0,0,0,0,0,0},{1,2,0,0,1,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,0,0,0,0,0,0,0}};


//Fonctions de test
void test_theorem_low(void)
{
  counter* table = generate_stone_E_for_test();
  filter_doubles(table);
  unsigned long chameau = theorem_low(table);
  printf("La valeur en test donne %lu combinaisons et d'après la fonction on a %lu (donc un delta de %lu )\n",((factorielle(RANGE))/),chameau ,((factorielle(20))/MOD_5)-chameau);
}

void test_factorielle_for_part(void)
{
  unsigned int* test;
  for (int rank = 0 ; rank <= RANGE ; rank++)
    {
      printf("Loutre n°%d\n",rank);
      test = factorielle_for_part(rank);
      printf("\n\n");
    }
  free(test);
}

void test_factorielle(void)
{
  for(unsigned int rank = 0 ; rank <= 20 ; ++rank)
    {
      printf("Factiorielle %u = %lu\n",rank,factorielle(rank));
    }
}

void test_multiplication_factorielle(void)
{
  unsigned int* test;
  unsigned int* list = malloc((PRIME+1)*sizeof(int));
  list = factorielle_for_part(PRIME);
  test = multiplication_factorielle(list,PRIME);
  for(int rank = 0 ; rank < PRIME ; rank++)
        {
          printf("[Test Multiplication]Test[%d] = %d\n\n",rank,test[rank]);
        }
  free(list);
  free(test);
}

void test_division_factorielle(void)
{
  unsigned int* numerator;
  unsigned int* denominator;
  for (int rank = RANGE ; rank > 0 ; rank--)
    {
          printf("Rank : %d , Depth : %d\n",rank,RANGE);
          numerator = factorielle_for_part(RANGE);
          denominator= factorielle_for_part(rank);
          division_factorielle(numerator,denominator);
          for (int depth = 0 ; depth <= RANGE ; depth++)
            {
              printf("[Division Numerateur]Rank = %d , Value = %d\n",depth,numerator[depth]);
            }
          printf("\n\n");
    }
    free(numerator);
    free(denominator);
}

void test_my_memcpy(void)
{
  unsigned int chameau[] = {0,1,2,3,4,5};
  unsigned int dromadaire[6];
  my_memcpy(dromadaire,chameau,6);
  for (int rank = 0; rank < 6 ; rank++)
    printf("[MEMCPY]Rank = %d , Value = %d\n",rank,dromadaire[rank]);
}

void test_p_part_n(void)
{
  for (int rank = 1 ; rank <= RANGE ; rank++)
    {
      for (int depth = 0

         ; depth <= rank; depth++)
        {
          printf("[P parmi N]%d parmi %d : %lu possibilitées\n",depth,rank,p_part_of_n(depth,rank));
        }
      printf("\n\n\n");
    }
}

void test_read_prime_decomposition(void)
{
  for (int rank = 0 ; rank <= RANGE ; rank++)
  {
    for (int depth = 0 ; depth < PRIME ; depth++)
    {
      printf("[Prime] Prime[%d][%d] = %d\n",rank,depth,prime_decomposition[rank][depth]);
    }
  }
}


void test_get_floor_and_roof(void)
{
  counter* head = generate_stone_E_for_test();
  head = filter_doubles(head);
  for (int rank = 0 ; rank < 10 ; rank++)
    {
      printf("[Roof And Floor]Avec %d éléments , min = %f et max = %f\n",rank,(get_floor_and_roof(rank,head))->min,(get_floor_and_roof(rank,head))->max);
    }
  delete_counter(head);
}

void test_generate_proposition(void)
{
  counter* facture_filtered;
  facture_filtered = filter_doubles(generate_stone_E_for_test());
  unsigned int subrank = get_depth_of_the_chained_list(facture_filtered);
  counter* proposition ;
  for (int rank = 1 ; rank < RANGE ; rank ++)
    {
      proposition = generate_proposition(facture_filtered,rank);
      for (int depth = 0 ; depth < subrank ; depth++)
        {
          printf("[Test Proposition]Value = %f , Quantity = %d\n",(reach_counter(depth,proposition))->value,(reach_counter(depth,proposition))->quantity);
        }
    }
  delete_counter(facture_filtered);
  delete_counter(proposition);
}


void test_reach_table(void)
{
  big_table head,corps,tail,extra_tail;
  head.previous = NULL;
  head.next = &corps;
  head.rank = 0;
  corps.previous = &head;
  corps.next = &tail;
  corps.rank = 1;
  tail.previous = &corps;
  tail.next = &extra_tail;
  tail.rank = 2;
  extra_tail.previous = &tail;
  extra_tail.next = NULL;
  extra_tail.rank = 3;
  printf("[Test Reach Table] :\nHead : %p %p\nCorps : %p %p\n",&head,reach_table(0,&head),&corps,reach_table(1,&head));
  printf("Tail : %p %p\nExtra Tail : %p %p\n",&tail,reach_table(2,&extra_tail),&extra_tail,reach_table(3,&tail));
}

//Fonctions

void my_memcpy(unsigned int* dest,unsigned int* src,unsigned int size)
{
  for (int rank = 0 ; rank < size ; rank++)
    {
      dest[rank] = src[rank];
    }
}

//Calcul le factorielle de n , n <= 20
unsigned long factorielle(unsigned int number)
{
  if (number < 0)
    return 0;
  else if (number == 0)
    return 1;
  else
    return number*factorielle(number-1);
}

unsigned int* factorielle_for_part(unsigned int number)
{
    unsigned int* table;
    if ( number > 0)
    {
        table = malloc((number+1)*sizeof(int));
        if ( table == NULL)
          {
            perror("[Factorielle Part] Malloc Failed");
            return NULL;
          }
        else
          {
            for (int rank = 0 ; rank <= number ; rank++)
              {
                if (rank == number)
                  {
                    table[rank] = 0;
                  }
                if (rank < number)
                  {
                    table[rank] = rank+1;
                  }
                //printf("[Factorielle]Rank = %d , Value = %d\n",rank,table[rank]);
              }
            return table;
          }
      }
    if (number == 0)
      {
        table = malloc(2*sizeof(int));
        if ( table == NULL)
          {
            perror("[Factorielle Part] Malloc Failed");
            return NULL;
          }
        else
          {
            if (number == 0)
              {
                table[0] = 1;
                table[1] = 0;
                printf("[Fatorielle]Rank = 0 , Value = %d ; Rank = 1 , Value = %d\n",table[0],table[1]);
              }
          }
      }
}

unsigned int* multiplication_factorielle(unsigned int* table , unsigned int max_rank)
{
  unsigned int* result;
  result = malloc(PRIME*sizeof(unsigned int));
  for (int rank = 0 ; rank < max_rank ; rank++)
  {
    for(int depth = 0 ; depth < PRIME ; depth++)
      {
          result[depth] += prime_decomposition[table[rank]] [depth];
          //printf("[Multiplication Fact]%dème Ni,Rank = %d , Value = %d\n",rank,depth,result[depth]);
      }
  }
  return result;
}


void division_factorielle(unsigned int* numerator,unsigned int* denominator)
{
  for(int rank = 0 ; rank < PRIME ; rank++)
    {
      if(numerator[rank] >= denominator[rank])
        {
          numerator[rank] -= denominator[rank];
          denominator[rank] = 0;
        }
      if(numerator[rank] < denominator[rank])
        {
          denominator[rank] -= numerator[rank];
          numerator[rank] = 0;
        }
    }
}

//Détermine le nombre de proposition max en se servant d'une chaînée qui n'a pas de doublon
unsigned long theorem_low(counter* head_filtered)
{
  unsigned int* num_table;
  unsigned int num_value = 0;
  unsigned int* denum_table;
  unsigned int denum_value = 0;
  unsigned long result;
  unsigned int* quantities_table;
  int subrank = get_depth_of_the_chained_list(head_filtered);

  quantities_table = malloc(subrank*sizeof(int));
  num_table = malloc(RANGE*sizeof(int));

  for (int rank = 0 ; rank < subrank ; rank++)
    {
      quantities_table[rank] = (reach_cell(rank,head_filtered))->quantity;
      num_value += quantities_table[rank];
    }

  my_memcpy(num_table,prime_decomposition[num_value],RANGE);

  denum_table = multiplication_factorielle(quantities_table,subrank);
  division_factorielle(num_table,denum_table);

  num_value = 0;
  denum_value = 0;
  for (int rank = 0 ; rank < RANGE ; rank++)
    {
      num_value += pow(prime_decomposition[0][rank],num_table[rank]);
      denum_value += pow(prime_decomposition[0][rank],denum_table[rank]);
    }
  result = num_value/denum_value;

  free(denum_table);
  free(num_table);
  free(quantities_table);
  return result;
}

unsigned long theorem_part(int quantity,int max_rank)
{
  unsigned long numerator = factorielle(max_rank);
  unsigned long denominator = (factorielle(max_rank-quantity))*(factorielle(quantity));
  return (numerator/denominator);
}


//obtenir la valeur min d'un tableau
float get_min_F(float* table,unsigned int max_rank)
{
  float min = table[0];
  for (unsigned int rank = 1; rank < max_rank; ++rank)
  {
    if ((min >= table[rank]) && (table[rank] > 0.0))
      min = table[rank];
  }
  return min;
}

//Obtenir la valeur max d'un tableau
float get_max_F(float* table , unsigned int max_rank)
{
  float max = table[0];
  for (unsigned int rank = 1; rank < max_rank; ++rank)
  {
    if (max <= table[rank])
      max = table[rank];
  }
  return max;
}

//Donne l'intervalle [|min;max|] en f(nombre d'éléments)
interval* get_floor_and_roof(int quantity , counter* head )
{
  float buffer_value_min;
  int buffer_rank_min;
  int quantity_min = quantity;
  int buffer_quantity_min;

  float buffer_value_max;
  int buffer_rank_max;
  int buffer_quantity_max;
  int quantity_max = quantity;

  int max_rank = get_depth_of_the_chained_list(head);
  float* table_value = malloc(max_rank*sizeof(float));
  int* table_quantity = malloc(max_rank*sizeof(int));

  interval* roof_and_floor;
  roof_and_floor = malloc(sizeof(interval));
  if (roof_and_floor == NULL)
    {
      perror("Bip Bip , pas d'espace pour l'intervalle");
      return NULL;
    }
  else
    {
      roof_and_floor->min = 0.0;
      roof_and_floor->max = 0.0;
      max_rank = extract_values_and_quantities_from_chained_list(table_value,table_quantity,head,max_rank);
      buffer_rank_min = 0;
      buffer_rank_max = max_rank;
      while (quantity_max > 0 || quantity_min > 0)
        {
                      buffer_quantity_max = table_quantity[buffer_rank_max];
                      buffer_quantity_min = table_quantity[buffer_rank_min];
                      buffer_value_max = table_value[buffer_rank_max];
                      buffer_value_min = table_value[buffer_rank_min];

                      if ((quantity_max <= buffer_quantity_max) && (quantity_max > 0))
                        {
                          roof_and_floor->max += quantity_max*buffer_value_max;
                          quantity_max = 0;
                        }
                      if (quantity_max > buffer_quantity_max)
                        {
                          roof_and_floor->max += buffer_quantity_max*buffer_value_max;
                          buffer_rank_max--;
                          quantity_max -= buffer_quantity_max;
                        }

                      if ((quantity_min <= buffer_quantity_min) && (quantity_min > 0))
                        {
                          roof_and_floor->min += quantity_min*buffer_value_min;
                          quantity_min = 0;
                        }
                      if (quantity_min > buffer_quantity_min)
                        {
                          roof_and_floor->min += buffer_quantity_min*buffer_value_min;
                          buffer_rank_min++;
                          quantity_min -= buffer_quantity_min;
                        }
          }
    }

  free(table_value);
  free(table_quantity);
  return roof_and_floor;
}

//Prend une liste chaînée filtrée , rend une liste chaînée filtrée
counter* generate_proposition(counter* facture_filtered,int quantity)
{
  float* table_value;
  counter* proposition;
  int* table_quantity;

  unsigned int subrank = get_depth_of_the_chained_list(facture_filtered);
  table_value = malloc(quantity*sizeof(float));
  table_quantity = malloc(quantity*sizeof(int));

  subrank = extract_values_from_chained_list(table_value,facture_filtered,subrank);
  for (int rank = 0 ; rank < quantity ; rank++)
    {
      table_value[rank] = (reach_counter((rand()%subrank),facture_filtered))->value;
      table_quantity[rank] = 1;
    }
  proposition = filter_doubles(generate_stone_E(table_value,table_quantity,quantity));
  return proposition;
}

//Free chaque cellule de la big_table donnée en argument , de n'importe où.
void delete_big_table(big_table* target)
{
  big_table* part = target;
  if( part->previous == NULL && part->next == NULL)
    {
      free(part->prop);
      free(part);
    }
  else
    {
      big_table* down = part->next;
      big_table* up = part->previous;
      big_table* buffer = NULL;
      while (down != NULL || up != NULL)
        {
          if( down != NULL && up != NULL)
            {
              buffer = down->next;
              free(down->prop);
              free(down);
              down = buffer;

              buffer = up->previous;
              free(up->prop);
              free(up);
              up = buffer;
            }
          if( down == NULL && up != NULL)
            {
              buffer = up->previous;
              free(up->prop);
              free(up);
              up = buffer;
            }
          if( down != NULL && up == NULL)
            {
              buffer = down->next;
              free(down->prop);
              free(down);
              down = buffer;
            }
          }
      }
}

//Pour accéder à un élément de la big_table(Tableau qui contient les proposition de quantitées).Monte et Descend.
big_table* reach_table(int rank,big_table* target)
{
  big_table* part = target;
  while(part->rank != rank && part != NULL)
    {
      if(part->rank > rank && part->previous != NULL)
        {
          part = part->previous;
        }
      if(part->rank < rank && part->next != NULL)
        {
          part = part->next;
        }
      else
        {
          return part;
        }
      }
return part;
}

//Pour accéder à la dernière cellule de la big_table, (lors de l'appel recursif pour ajouter un nouvel élément).Ne fait que descendre
big_table* reach_last_table(big_table* target)
{
  big_table* part = target;
  while(part->next != NULL)
    {
      part = part->next;
    }
  return part;
}
