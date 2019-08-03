#include "Lceb.h"

int main(int argc,char* argv[])
{
  if (argc != 2)
	goto usage;
  else
{
  printf("[ 0-5 ]Extraction des informations contenues dans %s.\n",argv[1]);
  //On extrait les infos : price , qty_elements , struct facture du json
  facture* input = malloc(sizeof(facture));
  if (input == NULL)
    {
      perror("Impossible d'extraire les informations du fichier\n");
    }
  else
    {
      float price;
      unsigned long max_quantity = parse_input_json(input,argv[1],&price); //isOk
      if(max_quantity == ERROR)
        {
          printf("Pas autant de factures que d'ID:\n[%ld:Factures]\t[%ld:ID]\n",get_nb_values(argv[1]),get_nb_ID(argv[1]));
        }
      else if (max_quantity == NOTHING)
        {
          printf("Pas de facture ni d'ID dans le fichier.\n");
        }
      else
        {
            printf("[ 5-10 ]Génération des structures.\n");
            //On génère une structure counter(doublons non filtrées) qui contient les valeurs avec lesquelles on va travailler.
            counter* facture = generate_stone_T(input,max_quantity);                 //isOK
            //On génère la grosse structure chenille , on initialise next/previous,rank,quantity_entities(0),alloue la première cellule de feet
            chenille* head = generate_chenille_body(max_quantity);  //isOK
            //On génère et rempli la table des propositions (sous forme de tableaux de int)
            printf("[ 10-90 ]Calcul des propositions (cela peut prendre +/- de temps).\n");
            proposition_recursive(head,facture,max_quantity,price); //isOk

            //On écrit chacunes des propositions qui ont été stockées dans la liste chaînée stack/chenille->feet de chaque cellule
            printf("[ 90-95 ]Ecriture des résultats dans Possibilities.json.\n");
            road_to_the_text_file(input,head,max_quantity); //isOk

            /*Une fois le .json crée et rempli on free toutes les allocations:
              -Chenille : chaque cellule ->chenille* ->chenille* ->stack*
                ->Stack : chaque cellule ->counter* ->stack*
                  ->Counter : chaque cellule ->counter*
                  -Facture : chaque cellule ->facture*
                  */
            printf("[ 95-100 ]Suppression des structures et libération de la mémoire.\n");
            delete_chenille(head);
        }
    }
delete_facture(input);
return 0;
  }

usage:
  printf("Pas le bon nombre d'arguments : %c./Linux_Le_Compte_Est_Bon.exe json_input%c\n",'"','"');
  return 1;
}
