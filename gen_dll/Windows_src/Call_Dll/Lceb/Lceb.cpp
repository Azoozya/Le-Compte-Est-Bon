// Lceb.cpp : Définit les fonctions exportées pour l'application DLL.
//

#include "stdafx.h"


extern "C"
{
	DLL_EXPORT void Lceb(void)
	{
		//char* filename = long_to_char(serial);
		char* filename = (char*)malloc(11 * sizeof(char));
		filename[0] = 'I';
		filename[1] = 'n';
		filename[2] = 'p';
		filename[3] = 'u';
		filename[4] = 't';
		filename[5] = '.';
		filename[6] = 'j';
		filename[7] = 's';
		filename[8] = 'o';
		filename[9] = 'n';
		filename[10] = '\0';
		printf("[ 0-5 ]Extraction des informations contenues dans %s.\n", filename);
		//On extrait les infos : price , qty_elements , struct facture du json
		facture* input = (facture *)malloc(sizeof(facture));
		if (input == NULL)
		{
			perror("Impossible d'extraire les informations du fichier\n");
		}
		else
		{
			float price;
			unsigned long max_quantity = parse_input_json(input, filename, &price); //isOk
			if (max_quantity == IMPOSSIBLE)
			{
				printf("Pas autant de factures que d'ID:\n[%lu:Factures]\t[%lu:ID]\n", get_nb_values(filename), get_nb_ID(filename));
			}
			else if (max_quantity == NOTHING)
			{
				printf("Pas de facture ni d'ID dans le fichier.\n");
			}
			else
			{
				printf("[ 5-10 ]Génération des structures.\n");
				//On génère une structure counter(doublons non filtrées) qui contient les valeurs avec lesquelles on va travailler.
				counter* facture = generate_stone_T(input, max_quantity);                 //isOK
				//On génère la grosse structure chenille , on initialise next/previous,rank,quantity_entities(0),alloue la première cellule de feet
				chenille* head = generate_chenille_body(max_quantity);  //isOK
				//On génère et rempli la table des propositions (sous forme de tableaux de int)
				printf("[ 10-90 ]Calcul des propositions (cela peut prendre +/- de temps).\n");
				proposition_recursive(head, facture, max_quantity, price); //isOk

				//On écrit chacunes des propositions qui ont été stockées dans la liste chaînée stack/chenille->feet de chaque cellule
				printf("[ 90-95 ]Ecriture des résultats dans %s.json.\n", filename);
				road_to_the_text_file(input, head, max_quantity, filename); //isOk

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
		free(filename);
	}

	/* -------------------------------------------- */
				/* Lceb_chained_list.c */
		//Génère une structure counter depuis un tableau de valeurs et un tableau de quantitées et le nombre d'éléments à regarder.
	counter* generate_stone_E(float* table_value, int* table_quantity, unsigned int length)
	{
		//On une brique élémentaire qui est la tête de la liste
		counter* head_stone_E = NULL;
		counter* stone_E = NULL;
		int depth;

		depth = 0;
		for (unsigned int rank = 0; rank < length; rank++)
		{
			//On génère une nouvelle brique élémentaire.
			stone_E = (counter*)malloc(sizeof(counter));
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
				stone_E->next = NULL;
				depth++;
				//on récupère la dernière cellule de la liste chaînée et on la fait pointer sur la nouvelle celule.
				if (rank == 0)
					head_stone_E = stone_E;
				else
				{
					(reach_counter(rank, head_stone_E))->next = stone_E;
				}
			}

		}
		return head_stone_E;
	}

	//Génère une structure counter depuis une structure facture.
	counter* generate_stone_T(facture* in, unsigned long max_quantity)
	{
		counter* result_head = NULL;
		float* table_value;
		int* table_quantity;

		facture* input_retrieve = in;

		table_value = (float*)malloc(max_quantity * sizeof(float));
		table_quantity = (int*)malloc(max_quantity * sizeof(int));
		if (table_value == NULL && table_quantity == NULL)
		{
			perror("Generate_stone_T , échec\n");
		}
		else
		{
			for (unsigned int rank = 0; rank < max_quantity; rank++)
			{
				table_value[rank] = input_retrieve->value;
				table_quantity[rank] = 1;
				input_retrieve = input_retrieve->next;         //////////////////////////
			}
			result_head = generate_stone_E(table_value, table_quantity, max_quantity);

			free(table_value);
			free(table_quantity);
		}
		return result_head;
	}

	//Génère la grosse structure , chaque bout correspond à un nombre d'éléments et contient l'ensemble des propositions à n-éléments
	chenille* generate_chenille_body(int max_rank)
	{
		chenille* head = NULL;
		for (int rank = 0; rank <= max_rank; rank++)
		{
			if (rank == 0)
				head = generate_chenille_part(0, NULL);
			else
				(reach_chenille(rank - 1, head))->next = generate_chenille_part(rank, reach_chenille(rank - 1, head));
		}
		return head;
	}

	//Génère un bout de la structure , est appelée dans generate_body
	chenille* generate_chenille_part(int quantity, chenille* previous)
	{
		chenille* part;
		part = (chenille*)malloc(sizeof(chenille));
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

			part->feet = (stack*)malloc(sizeof(stack));
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

		table_value = (float*)malloc(max_rank * sizeof(float));
		table_quantity = (int*)malloc(max_rank * sizeof(int));

		if (table_value == NULL || table_quantity == NULL)
		{
			perror("Pas d'espace pour le(s) tableau(x) filter");
			result = NULL;
		}
		else
		{
			subrank = extract_values_and_quantities_from_chained_list(table_value, table_quantity, head, max_rank);
			//En l'état , on a 2 listes du genre : {50.0,23.95,33.0,0,0,0} et {3,2,1,x,x,x,x,} et subrank = 4;
			result = generate_stone_E(table_value, table_quantity, subrank);
			free(table_value);
			free(table_quantity);
			delete_counter(head);
		}
		return result;
	}

	//Free chaque cellule de la big_table donnée en argument , de n'importe où.
	void delete_chenille(chenille* part)
	{
		if (part->previous == NULL && part->next == NULL)
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
			if (down != NULL && up != NULL)
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
			if (down == NULL && up != NULL)
			{
				buffer = up->previous;
				if (up->quantity_entities == 0)
					free(up->feet);
				if (up->quantity_entities > 0)
					delete_stack(up->feet);
				free(up);
				up = buffer;
			}
			if (down != NULL && up == NULL)
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
		if (part == NULL)
		{
			free(part);
		}
		else
		{
			counter* buffer = NULL;
			while (part != NULL)
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
		if (part->next == NULL)
		{
			delete_counter(part->proposition);
			free(part);
		}
		else
		{
			stack* buffer = NULL;
			while (part != NULL)
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
		if (part->next == NULL)
		{
			free(part);
		}
		else
		{
			facture* buffer = NULL;
			while (part != NULL)
			{
				buffer = part->next;
				free(part);
				part = buffer;
			}
		}
	}

	//Reach
	//Pour accéder à une cellule de structure counter , (les propositions avec value/quantity associées).Ne fait que descendre
	counter* reach_counter(int rank, counter* head)
	{
		counter* pointeur = head;
		if (rank == 0)
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
		while (part->next != NULL)
		{
			part = part->next;
		}
		return part;
	}

	//Pour accéder à une cellule de structure chenille , (les cellules associées à N , qui contient l'ensemble des propositions).Monte et Descend.
	chenille* reach_chenille(int quantity, chenille* part)
	{
		chenille* pointeur = part;
		if (quantity == part->rank)
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
	stack* reach_stack(int rank, stack* head)
	{
		int quantity = 0;
		stack* pointeur = head;
		while (quantity != rank)
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
	int how_many_is_in(float* table, float value, unsigned int length)
	{
		int answer = 0;
		for (unsigned int rank = 0; rank < length; rank++)
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
	int extract_values_from_chained_list(float* table_value, counter* head, unsigned int max_rank)
	{
		unsigned int subrank = 0;
		for (unsigned int rank = 0; rank < max_rank; rank++)
		{
			table_value[rank] = (reach_counter(rank, head))->value;
		}

		for (unsigned int rank = 0; rank < max_rank; rank++)
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
	int extract_values_and_quantities_from_chained_list(float* table_value, int* table_quantity, counter* head, unsigned int max_rank)
	{
		unsigned int subrank = 0;
		for (unsigned int rank = 0; rank < max_rank; rank++)
		{
			table_value[rank] = (reach_counter(rank, head))->value;
			if (rank > 0)
			{
				sort(0, rank - 1, table_value);
			}
		}

		for (unsigned int rank = 0; rank < max_rank; rank++)
		{
			if (table_value[rank] != 0.0)
			{
				table_quantity[subrank] = how_many_is_in(table_value, table_value[rank], max_rank);
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
	int get_rank_with_value(float value, counter* head)
	{
		counter* pointeur = filter_doubles(head);
		int rank = 0;
		if (pointeur == NULL)
		{
			perror("Panique a bord c'est une liste vide");
			rank = -1;
		}
		else
		{

			while (pointeur != NULL && pointeur->value != value)
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
		for (unsigned int rank = 0; rank < max_rank; rank++)
		{
			result += reach_counter(rank, head)->sum;
		}
		return result;
	}

	/* -------------------------------------------- */
			/* Lceb_fonctions.c */

	//Prend en entrée le pointeur sur structure chenille initialisé, pointeur sur la facture format counter déjà rempli et déjà filtrée.
	void proposition_recursive(chenille* head, counter* facture, int max_quantity, float price)
	{
		float* table_value = (float*)malloc(max_quantity * sizeof(float));
		int* table_quantity = (int*)malloc(max_quantity * sizeof(int));
		if (table_value == NULL || table_quantity == NULL)
		{
			perror("Pas d'espace pour stocker les valeurs de références\n");
		}
		else
		{
			unsigned int subrank = extract_values_and_quantities_from_chained_list(table_value, table_quantity, facture, max_quantity);
			int* proposition = (int*)malloc(subrank * sizeof(int));
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
				recursive(table_value, head, table_quantity, proposition, subrank, depth, goal, price);
				free(proposition);
			}
			free(table_value);
			free(table_quantity);
		}
	}

	void recursive(float* table_value, chenille* head, int* table_quantity, int* proposition, int subrank, int depth, int goal, float price)
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
			for (int rank = 0; rank < subrank; rank++)
				sum_qty += proposition[rank];
			table = (int*)malloc(subrank * sizeof(int));
			if (table == NULL)
			{
				perror("Pas d'espace pour stocker la table des quantitées pour la proposition.\n");
			}
			else
			{
				my_memcpy((unsigned int*)table, (unsigned int*)proposition, subrank);

				counter_proposition = generate_stone_E(table_value, table, subrank);
				if (evaluation(counter_proposition) == price)
				{
					chenille_part = reach_chenille(sum_qty, head);
					if (chenille_part->quantity_entities == 0)
					{
						chenille_part->feet->proposition = counter_proposition;
						chenille_part->feet->next = NULL;
					}
					else
					{
						stack_part = (stack*)malloc(sizeof(stack));
						stack_part->proposition = counter_proposition;
						stack_part->next = NULL;
						(reach_last_stack(chenille_part->feet))->next = stack_part;
					}
					if (chenille_part->quantity_entities < 0)
						printf("Quantity entities is negative : [%d] %ld\n", sum_qty, chenille_part->quantity_entities);
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
			proposition[depth] = table_quantity[subrank - depth];
			for (int rank = 0; rank < subrank; rank++)
				sum_qty += proposition[rank];
			table = (int*)malloc(subrank * sizeof(int));
			if (table == NULL)
			{
				perror("Pas d'espace pour stocker la table des quantitées pour la proposition.\n");
			}
			else
			{
				my_memcpy((unsigned int*)table, (unsigned int*)proposition, subrank);

				counter_proposition = generate_stone_E(table_value, table, subrank);
				if (evaluation(counter_proposition) == price)
				{
					chenille_part = reach_chenille(sum_qty, head);
					if (chenille_part->quantity_entities == 0)
					{
						chenille_part->feet->proposition = counter_proposition;
						chenille_part->feet->next = NULL;
					}
					else
					{
						stack_part = (stack*)malloc(sizeof(stack));
						stack_part->proposition = counter_proposition;
						stack_part->next = NULL;
						(reach_last_stack(chenille_part->feet))->next = stack_part;
					}
					if (chenille_part->quantity_entities < 0)
						printf("Quantity entities is negative : [%d] %ld\n", sum_qty, chenille_part->quantity_entities);
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
			for (int purpose = table_quantity[depth]; purpose >= 0; purpose--)
			{
				if (goal >= purpose)
				{
					proposition[depth] = purpose;
					goal -= purpose;
					recursive(table_value, head, table_quantity, proposition, subrank, depth + 1, goal, price);
				}
				goal = goal_save;
			}
		}
	}

	void road_to_the_text_file(facture* facture, chenille* head, int max_quantity, char* filename)
	{
		std::ofstream Out;
		char* file = getOutputFileName(filename);
		Out.open(file);
		chenille* chenille_part;
		stack* stack_part;
		counter* cell;
		unsigned long id;
		int subrank;
		int qty_value;
		int qty_element;

		Out << "{\n";
		for (int rank = 1; rank <= max_quantity; rank++)
		{
			chenille_part = reach_chenille(rank, head);
			if (chenille_part->quantity_entities == 0)
			{
				Out << "\t\t\t" << '"' << rank << "_element" << '"' << ":{\t},\n\n";
			}
			if (chenille_part->quantity_entities > 0)
			{
				Out << "\t\t\t" << '"' << rank << "_element" << '"' << ":{\n";
				//Value:
				Out << "\t\t\t\t" << '"' << "value" << '"' << ":[\n";
				for (unsigned int depth = 0; depth < chenille_part->quantity_entities; depth++)
				{
					Out << "\t\t\t\t\t\t\t\t[";
					stack_part = reach_stack(depth, chenille_part->feet);
					subrank = 0;
					qty_element = 1;
					do {
						cell = reach_counter(subrank, stack_part->proposition);
						qty_value = 1;
						if (cell->value != 0.0 && cell->quantity != 0)
						{
							while (qty_value <= cell->quantity)
							{
								Out << cell->value;
								qty_value++;
								if (qty_element != rank)
								{
									Out << ",";
									qty_element++;
								}
							}
						}
						subrank++;
					} while (cell != reach_last_counter(stack_part->proposition));
					Out << "]";
					if (depth != (chenille_part->quantity_entities - 1))
						Out << ",\n";
					else
						Out << "\n";
				}
				Out << "\t\t\t\t\t\t\t\t],\n";
				//ID:
				Out << "\t\t\t\t\t" << '"' << "ID" << '"' << ":[\n";
				for (unsigned int depth = 0; depth < chenille_part->quantity_entities; depth++)
				{
					Out << "\t\t\t\t\t\t\t\t[";
					stack_part = reach_stack(depth, chenille_part->feet);
					subrank = 0;
					qty_element = 1;

					do {
						cell = reach_counter(subrank, stack_part->proposition);
						qty_value = 1;
						id = 0;
						if (cell->value != 0.0 && cell->quantity != 0)
						{
							while (qty_value <= cell->quantity)
							{
								id = get_id_with_value(cell->value, id, facture);
								Out << id;
								qty_value++;
								if (qty_element != rank)
								{
									Out << ",";
									qty_element++;
								}
							}
						}
						subrank++;
					} while (cell != reach_last_counter(stack_part->proposition));
					Out << "]";
					if (depth != (chenille_part->quantity_entities - 1))
						Out << ",\n";
					else
						Out << "\n";
				}
				Out << "\t\t\t\t\t\t\t ],\n";
				Out << "\t\t\t\t\t\t\t\t\t},\n\n";
			}
			if (chenille_part->quantity_entities < 0)
				printf("[c->json] Error , quantity_entities < 0\n");
		}
		Out << "\n}," << '\0';
		Out.close();
	}

	//renvoie la première occurence après l'ID donnée de la value passée en argument dans la structure facture (trié dans l'ordre croissant)
	unsigned long get_id_with_value(float value, unsigned long id, facture* input)
	{
		facture* in = input;
		if (in == NULL)
			return 666;
		while (in != NULL && !(in->value == value && in->id > id))
		{
			in = in->next;
		}
		return in->id;
	}

	void my_memcpy(unsigned int* dest, unsigned int* src, unsigned int size)
	{
		for (unsigned int rank = 0; rank < size; rank++)
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
		facture* cursor = NULL;
		facture* stone_T = NULL;

		if (nb_value != nb_ID)
		{
			return IMPOSSIBLE;
		}
		else if (nb_value == nb_ID && nb_value == 0)
		{
			return NOTHING;
		}
		else
		{
			table_value = (float*)malloc(nb_value * sizeof(float));
			table_id = (unsigned long*)malloc(nb_ID * sizeof(unsigned long));

			extract_values_from_json(filename, table_value, nb_value);
			extract_ID_from_json(filename, table_id, nb_ID);
			*target = get_price(filename);

			for (unsigned long rank = 0; rank < nb_value; rank++)
			{
				//On génère une nouvelle brique élémentaire.
				if (rank == 0)
				{
					head->value = table_value[rank];
					head->id = table_id[rank];
					head->next = NULL;
					cursor = head;
				}
				else
				{
					stone_T = (facture*)malloc(sizeof(facture));
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

	unsigned long get_nb_values(char* filename)
	{
		unsigned long nb_elements = 0;
		char element = '\0';
		short indicator = 0;
		std::ifstream In;
		In.open(filename);
		while (!(indicator) && element != '}')
		{
			do {
				element = In.get();
			} while (element != '"' && element != '}');
			element = In.get();

			if (element == 'V' || element == 'v')
			{
				element = In.get();
				if (element == 'A' || element == 'a')
				{
					element = In.get();
					if (element == 'L' || element == 'l')
					{
						element = In.get();
						if (element == 'U' || element == 'u')
						{
							element = In.get();
							if (element == 'E' || element == 'e')
							{
								element = In.get();
								if (element == '"')
								{
									indicator = 1;
								}
							}
						}
					}
				}
			}

			if (!(indicator))
			{
				do {
					element = In.get();
					printf("%c\n", element);
				} while (element != ']' && element != '}');
			}
		}

		if (element != '}')
		{
			do {
				element = In.get();
			} while (element != '[');

			do {
				element = In.get();
				if (element != ']')
				{
					nb_elements++;
					while (element != ',' && element != ']')
					{
						element = In.get();
					}
				}
			} while (element != ']');
		}

		In.close();
		return nb_elements;
	}

	unsigned long get_nb_ID(char* filename)
	{
		unsigned long nb_elements = 0;
		char element = '\0';
		short indicator = 0;
		std::ifstream In;
		In.open(filename);
		while (!(indicator) && element != '}')
		{
			do {
				element = In.get();
			} while (element != '"' && element != '}');

			element = In.get();
			if (element == 'I' || element == 'i')
			{
				element = In.get();
				if (element == 'D' || element == 'd')
				{
					element = In.get();
					if (element == '"')
					{
						indicator = 1;
					}
				}
			}

			if (!(indicator))
			{
				do {
					element = In.get();
				} while (element != ']' && element != '}');
			}
		}

		if (element != '}')
		{
			do {
				element = In.get();
			} while (element != '[');

			do {
				element = In.get();
				if (element != ']')
				{
					nb_elements++;
					while (element != ',' && element != ']')
					{
						element = In.get();
					}
				}
			} while (element != ']');
		}

		In.close();
		return nb_elements;
	}

	float get_price(char* filename)
	{
		float price = 1.0;
		int nb_digits = 0;
		char element = '\0';
		std::ifstream In;
		In.open(filename);
		short indicator = 0;
		char buffer[50];
		while (!(indicator) && element != '}')
		{
			do {
				element = In.get();
			} while (element != '"' && element != '}');

			element = In.get();
			if (element == 'P' || element == 'p')
			{
				element = In.get();
				if (element == 'R' || element == 'r')
				{
					element = In.get();
					if (element == 'I' || element == 'i')
					{
						element = In.get();
						if (element == 'C' || element == 'c')
						{
							element = In.get();
							if (element == 'E' || element == 'e')
							{
								element = In.get();
								if (element == '"')
								{
									element = In.get();
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

			if (!(indicator))
			{
				do {
					element = In.get();
				} while (element != ']' && element != '}');
			}
		}

		if (element != '}')
		{
			while (element != '.' && element != '}')
			{
				element = In.get();
				if (element != ' ')
				{
					buffer[nb_digits] = element;
					nb_digits++;
				}
			}
			for (int depth = 1; depth <= 6; depth++)
			{
				if (element != '\n')
				{
					element = In.get();
					buffer[nb_digits + depth] = element;
				}
				if (element == '\n')
				{
					buffer[nb_digits + depth] = '0';
				}
			}
		}

		price = atof(buffer);
		In.close();
		return price;
	}

	void extract_values_from_json(char* filename, float* table_value, unsigned long nb_value)
	{
		char element = '\0';
		unsigned long nb_elements = 0;
		unsigned long rank = 0;
		short precision = 0;
		short indicator = 0;
		char buffer[SIZE];
		std::ifstream In;
		In.open(filename);
		while (!(indicator) && element != '}')
		{
			do {
				element = In.get();
			} while (element != '"' && element != '}');
			element = In.get();
			if (element == 'V' || element == 'v')
			{
				element = In.get();
				if (element == 'A' || element == 'a')
				{
					element = In.get();
					if (element == 'L' || element == 'l')
					{
						element = In.get();
						if (element == 'U' || element == 'u')
						{
							element = In.get();
							if (element == 'E' || element == 'e')
							{
								element = In.get();
								if (element == '"')
								{
									indicator = 1;
								}
							}
						}
					}
				}
			}

			if (!(indicator))
			{
				do {
					element = In.get();
					printf("%c\n", element);
				} while (element != ']' && element != '}');
			}
		}

		if (element != '}')
		{
			do {
				element = In.get();
			} while (element != '[');
			do {
				element = In.get();
				precision = 0;
				nb_elements = 0;
				while (element != ',' && element != ']')
				{
					while (element != '.')
					{
						buffer[nb_elements] = element;
						nb_elements++;
						element = In.get();
					}
					buffer[nb_elements] = element;
					nb_elements++;
					while (element != ',' && element != ']')
					{
						element = In.get();
						if (element != ',' && element != ']')
						{
							buffer[nb_elements] = element;
							nb_elements++;
							precision++;
						}
					}
				}
				if (precision < 6)
				{
					for (int depth = precision; precision <= 6; depth++)
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
				reset(buffer, SIZE);
				rank++;
			} while (element != ']');
		}
		In.close();
	}

	void extract_ID_from_json(char* filename, unsigned long* table_id, unsigned long nb_ID)
	{
		char element = '\0';
		unsigned long nb_elements = 0;
		unsigned long rank = 0;
		short indicator = 0;
		char buffer[SIZE];
		std::ifstream In;
		In.open(filename);
		while (!(indicator) && element != '}')
		{
			do {
				element = In.get();
			} while (element != '"' && element != '}');

			element = In.get();
			if (element == 'I' || element == 'i')
			{
				element = In.get();
				if (element == 'D' || element == 'd')
				{
					element = In.get();
					if (element == '"')
					{
						indicator = 1;
					}
				}
			}

			if (!(indicator))
			{
				do {
					element = In.get();
				} while (element != ']' && element != '}');
			}
		}

		if (element != '}')
		{
			do {
				element = In.get();
			} while (element != '[');
			do {
				element = In.get();
				if (element == ',' || element == ']')
				{
					buffer[nb_elements] = '.';
					table_id[rank] = atoi(buffer);
					rank++;
					reset(buffer, SIZE);
					nb_elements = 0;
				}
				else
				{
					buffer[nb_elements] = element;
					nb_elements++;
				}
			} while (element != ']');
		}
		In.close();
	}

	void reset(char* array, int length)
	{
		for (int rank = 0; rank < length; rank++)
		{
			array[rank] = '0';
		}
	}

	void merging(int low, int mid, int high, float* table_value)
	{
		int l1, l2, i;
		float* buffer_value = (float*)malloc((high + 1) * sizeof(float));
		if (buffer_value == NULL)
			perror("Merging Imppossible.\n");
		else
		{
			for (l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++)
			{
				if (table_value[l1] <= table_value[l2])
				{
					buffer_value[i] = table_value[l1++];
				}
				else
				{
					buffer_value[i] = table_value[l2++];
				}
			}

			while (l1 <= mid)
			{
				buffer_value[i++] = table_value[l1++];
			}

			while (l2 <= high)
			{
				buffer_value[i++] = table_value[l2++];
			}
			for (i = low; i <= high; i++)
			{
				table_value[i] = buffer_value[i];
			}
			free(buffer_value);
		}
	}

	void sort(int low, int high, float* table_value)
	{
		int mid;

		if (low < high)
		{
			mid = (low + high) / 2;
			sort(low, mid, table_value);
			sort(mid + 1, high, table_value);
			merging(low, mid, high, table_value);
		}
	}


	//  Pour la conversion nombre -> char*
	char* long_to_char(long serial)
	{
		char* to_return = NULL;
		long max_power = 1;
		//Détermine le nombre de chiffre(s)
		if (serial < 0)
		{
			serial = serial * (-1);
		}
		if (serial < 10 && serial >= 0)
		{

			to_return = (char*)malloc(sizeof(char));
			to_return[0] = digit_convert(serial);
		}
		else
		{
			max_power = 2;
			while (serial >= pow(10, max_power))
			{
				max_power++;
			}
			//On récupère chacun des chiffres et on les met dans un tableau
			int* table_value = (int*)malloc(max_power * sizeof(int));
			for (int depth = 0; depth < max_power; depth++)
			{
				table_value[depth] = 0;
			}
			for (long power = (max_power - 1); power >= 0; power--)
			{
				table_value[power] = serial / pow(10, power);
				serial -= table_value[power] * pow(10, power);
			}
			to_return = reverse_and_convert(table_value, max_power);
		}

		printf("[long_to_char] : %s \n", to_return);

		return to_return;
	}

	//Prend un tableau d'entier pour les convertir en char , [1,2,3] -> "123"
	char* reverse_and_convert(int* table_value, long length)
	{
		char* buffer = (char*)malloc(length * sizeof(char));
		for (int i = 0; i < length; i++)
		{
			buffer[length - i - 1] = digit_convert(table_value[i]);
		}
		return buffer;
	}
	//Fais la conrrespondance chiffre char , 1 -> '1'
	char digit_convert(int digit)
	{
		switch (digit) {
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
			break;
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		default:
			return '\0';
			break;
		}
	}
	//  Pour la conversion nombre -> char*
	char* getOutputFileName(char* inputFileName)
	{
		char* extension = (char*)malloc(5 * sizeof(char));
		extension[0] = '.';
		extension[1] = 'j';
		extension[2] = 's';
		extension[3] = 'o';
		extension[4] = 'n';
		int lenght = strlen(inputFileName);
		char* output = (char*)malloc((lenght + 5) * sizeof(char));
		errno_t err = strcat_s(output,lenght, inputFileName);
		err = strcat_s(output, lenght, extension);
		free(extension);
		return output;
	}
}