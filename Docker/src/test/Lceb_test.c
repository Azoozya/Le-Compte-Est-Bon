#include "Lceb.h"

void test_random(void)
{
  int* randoms;
  randoms = malloc(RANGE*sizeof(int));

  if (randoms == NULL)
    perror("Seg Fault en Vue");
  else
  {
    srand(time(NULL));
    for (int rank = 0 ; rank < RANGE ; ++rank)
        {
          randoms[rank] = rand();
          printf("[Random In] Valeur Random N°%d = %d\n",rank,randoms[rank]);
        }
    for (int rank = 0 ; rank < 50 ; ++rank)
        {
          printf("[Random Out] Valeur Random N°%d = %d\n",rank,randoms[rank]);
        }
  }
  free(randoms);
}

void test_get_depth(void)
{
  counter* table = generate_stone_E_for_test();
  printf("RANGE = %d et la liste contient %d éléments\n",RANGE,get_depth_of_the_chained_list(table));
  delete_counter(table);
}

void test_filter(void)
{
  counter* table = generate_stone_E_for_test();
  table = filter_doubles(table);
  for (unsigned int rank = 0 ; rank < get_depth_of_the_chained_list(table) ; ++rank)
    {
      printf("Value = %f , Quantity = %d , Cell n°%u\n",(reach_counter(rank,table))->value,(reach_counter(rank,table))->quantity,rank);
    }
  delete_counter(table);
}

void test_generate_stone_E_for_test(void)
{
  counter* table = generate_stone_E_for_test();
  for (int rank = 0 ; rank < get_depth_of_the_chained_list(table) ; rank++)
  {
    printf("[Test Generation for Test] Value = %f , Quantity = %d , Cell n°%d\n",(reach_counter(rank,table))->value,(reach_counter(rank,table))->quantity,rank);
  }
  delete_counter(table);
}

void test_how_many(void)
{
  float test[]={5.0,2.0,3.0,2.0,3.0};
  printf("Il y a %d 5.0 , %d 3.0 , %d 2.0\n",how_many_is_in(test,5.0,5),how_many_is_in(test,3.0,5),how_many_is_in(test,2.0,5));
  printf("%f\t%f\t%f\t%f\t%F\n",test[0],test[1],test[2],test[3],test[4]);
}

void test_get_rank_with_values(void)
{
  counter* table = generate_stone_E_for_test();
  table = filter_doubles(table);
  for (int rank = 0 ; rank < get_depth_of_the_chained_list(table) ; rank++)
  {
    printf("[Test Get Rank] Value = %f donne rank = %d alors que rank = %d\n",(reach_counter(rank,table))->value,get_rank_with_value((reach_counter(rank,table))->value,table),rank);
  }
  delete_counter(table);
}

void test_sort(void)
{
  float value[5] = {3.0,2.0,6.0,9.0,1.0};
  sort(0,4,value);
  for (int rank = 0 ; rank < 5 ; rank++)
    printf("[Sort]Cell %d : Value %f\n",rank,value[rank]);
}

void test_evaluation(void)
{
  counter* table = generate_stone_E_for_test();
  printf("[Evaluation]Génération : %f , Post-Filter : %f\n",evaluation(table),evaluation(filter_doubles(table)));
  delete_counter(table);
}

void test_reach_stack(void)
{
  stack head,corps,tail;
  head.next = &corps;
  corps.next = &tail;
  tail.next = NULL;
  printf("[Reach Stack]\nHead : %p %p ;\n Corps : %p %p ;\n Tail : %p %p ;\n",&head,reach_stack(0,&head),&corps,reach_stack(1,&head),&tail,reach_stack(2,&head));
  printf("Last Stack : %p %p ;\n",&tail,reach_last_stack(&head));
}


// void test_recursive_proposition(void)
// {
//   facture* input = generate_facture_for_test();
//   counter* stone = generate_stone_T(input,RANGE);
//   int max_quantity = get_depth_of_the_chained_list(stone);
//
//   chenille* head = generate_chenille_body(max_quantity);
//   big_table* target = proposition_recursive(head,stone,max_quantity);
//   big_table* arrow = target->next;
//   int subrank = get_depth_of_the_chained_list(filter_doubles(stone));
//   int depth = 1;
//   do{
//         for (int rank = 0 ; rank < subrank ; rank++)
//           {
//             //printf("Big_Table [%d][%d] : %p %p\n",depth,rank,arrow,arrow->prop);
//             printf("Big_Table[%d][%d] = %d\n",depth,rank,arrow->prop[rank]);
//           }
//         depth++;
//         arrow = arrow->next;
//     }while(arrow != reach_last_table(target));
//   delete_big_table(target);
//   delete_chenille(head);
//   delete_facture(input);
// }


////////////////////////////////////////////////////////////////////////////////

//génère une structure contenant des valeurs de factures factices et des quantitées
counter* generate_stone_E_for_test(void)
{
    //On une brique élémentaire qui est la tête de la liste
    counter* head_stone_E;
    counter* stone_E;
    head_stone_E = malloc(sizeof(counter));

    if (head_stone_E == NULL)
      {
        perror("Mémoire non alloué");
        return NULL;
      }
    else
    {
      for (int rank = 0 ; rank < RANGE ; rank++)
        {
          //On génère une nouvelle brique élémentaire.
          stone_E = malloc(sizeof(counter));
          stone_E->value = ((float)((rank*40)%100))+20.0;
          stone_E->quantity = 1;
          stone_E->sum = (stone_E->value)*(float)(stone_E->quantity);
          stone_E->rank = rank;
          stone_E->next= NULL;

          //on récupère la dernière cellule de la liste chaînée et on la fait pointer sur la nouvelle celule.
          if (rank == 0)
            head_stone_E = stone_E;
          else
            {
              (reach_counter(rank,head_stone_E))->next = stone_E;
            }
          //printf("[Generation for Test]Au rank %d on a une valeur de %f avec une quantité de %u\n",rank,stone_E->value,stone_E->quantity);
        }
      return head_stone_E;
    }
}

facture* generate_facture_for_test(void)
{
  facture* stone_T;
  facture* head_stone_T;
  facture* cursor;
  for (unsigned long rank = 0 ; rank < RANGE ; rank++)
    {
      //On génère une nouvelle brique élémentaire.
      stone_T = malloc(sizeof(facture));
      stone_T->value = (float)((rank*80)%300)+20.0;
      stone_T->id = 2*rank+1;
      printf("[id]Value = %f , id = %ld\n",stone_T->value,stone_T->id);
      stone_T->next = NULL;

      //on récupère la dernière cellule de la liste chaînée et on la fait pointer sur la nouvelle celule.
      if (rank == 0)
        {
          head_stone_T = stone_T;
          cursor = stone_T;
        }
      else
        {
          cursor->next = stone_T;
          cursor = cursor->next;
        }
    }
  return head_stone_T;
}

void generate_json_input(void)
{
  FILE* Out;
  Out = fopen("Input.json","w");
  float value;
  unsigned long id;
  float price = TARGET;

  fprintf(Out,"{\n\t\t\t%cvalue%c:[",'"','"');
  for (int rank = 0 ; rank < RANGE ; rank++)
    {
      // Pour changer la valeur à inscrire
      value = (float)((rank*40)%120)+20.0;
      //
      fprintf(Out,"%f",value);
      if (rank != RANGE-1)
        fprintf(Out,",");
    }
  fprintf(Out,"],\n\t\t\t%cID%c:[",'"','"');
  for (int rank = 0 ; rank < RANGE ; rank++)
    {
      //Pour changer la valeur à inscrire
      id = rank+1;
      //
      fprintf(Out,"%ld",id);
      if (rank != RANGE-1)
        fprintf(Out,",");
    }

  fprintf(Out,"],\n\t\t\t%cprice%c: %f\n},%c",'"','"',price,'\0');
}

void test_generate_stone_T(void)
{
  facture* head_stone_T;
  facture* stone_T;
  facture* cursor;
  counter* head;
  unsigned int depth;

    for (int rank = 0 ; rank < RANGE ; rank++)
      {
        //On génère une nouvelle brique élémentaire.
        stone_T = malloc(sizeof(facture));
        stone_T->value = ((float)((rank*20)%100))+20.0;
        stone_T->id = rank+(rank/100.0);
        stone_T->next = NULL;

        //on récupère la dernière cellule de la liste chaînée et on la fait pointer sur la nouvelle celule.
        if (rank == 0)
          {
            head_stone_T = stone_T;
            cursor = stone_T;
          }
        else
          {
            cursor->next = stone_T;
            cursor = cursor->next;
          }
        //printf("[Generation for Test]Au rank %d on a une valeur de %f avec une quantité de %u\n",rank,stone_E->value,stone_E->quantity);
      }
      head = generate_stone_T(head_stone_T,RANGE);
      depth = 0;
      do
        {
          printf("[Stone_T]Rank = %d , Value = %f , Qty = %d\n",depth,(reach_counter(depth,head))->value,(reach_counter(depth,head))->quantity);
          depth++;
        } while((reach_counter(depth,head))->next != NULL);

}
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char* argv[])
{
  test_parse_input_json();
  return 1;
}

void test_get_nb_values(void)
{
  printf("Valeur souhaitée : %d , Valeur reçu : %lu\n",SIZE,get_nb_values("Input.json"));
}

void test_get_nb_ID(void)
{
  printf("Valeur souhaitée : %d , Valeur reçu : %lu\n",SIZE,get_nb_ID("Input.json"));
}

void test_get_price(void)
{
  printf("Valeur souhaitée : %f , Valeur reçu : %f\n",TARGET,get_price("Input.json"));
}

void test_extract_values_from_json(void)
{
  float* table_value = malloc(sizeof(float)*get_nb_values("Input.json"));
  extract_values_from_json("Input.json",table_value,get_nb_values("Input.json"));
  for (int rank = 0 ; rank < SIZE ; rank++)
    {
      printf("Table_value [%d] : %f\n",rank,table_value[rank]);
    }
}

void test_extract_ID_from_json(void)
{
  unsigned long* table_ID = malloc(sizeof(long)*get_nb_ID("Input.json"));
  extract_ID_from_json("Input.json",table_ID,get_nb_ID("Input.json"));
  for (int rank = 0 ; rank < SIZE ; rank++)
    {
      printf("Table_value [%d] : %lu\n",rank,table_ID[rank]);
    }
}

// void test_road_to_text_file(void)
// {
//   facture* input = malloc(sizeof(facture));
//   float price;
//   unsigned long max_quantity = parse_input_json(input,"Input.json",&price);
//   counter* facture = generate_stone_T(input,max_quantity);                 //isOK
//   chenille* head = generate_chenille_body(max_quantity);  //isOK
//
//   proposition_recursive(head,facture,max_quantity,price); //isOk
//
//   road_to_the_text_file(input,head,max_quantity); //isOk
//
//   delete_chenille(head);
//   delete_facture(input);
// }

void test_parse_input_json(void)
{
  char* path = "Input.json";
  facture* loutre = malloc(sizeof(facture));
  float price;
  unsigned long chameau = parse_input_json(loutre,path,&price);
  printf("%s contient %lu factures et un réglement de %f.\n",path,chameau,price);

  
  delete_facture(loutre);
}
