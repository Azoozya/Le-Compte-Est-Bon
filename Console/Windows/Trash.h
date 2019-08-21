#ifndef FACTORIELLE
#define FACTORIELLE

typedef struct big_table big_table;
struct big_table
{
  int* prop;
  int qty;
  int rank;
  big_table* previous;
  big_table* next;
};


//Prototypes
void test_factorielle(void);
void test_theorem_low(void);
void test_factorielle_for_part(void);
void test_multiplication_factorielle(void);
void test_division_factorielle(void);
void test_my_memcpy(void);
void test_p_part_n(void);
void test_get_floor_and_roof(void);
void test_generate_proposition(void);
void test_reach_table(void);

void test_read_prime_decomposition(void);
void my_memcpy(unsigned int* dest,unsigned int* src,unsigned int size);

unsigned long factorielle(unsigned int number);

unsigned int* factorielle_for_part(unsigned int number);
unsigned int* multiplication_factorielle(unsigned int* table , unsigned int max_rank);
void division_factorielle(unsigned int* numerator,unsigned int* denominator);
unsigned long theorem_part(int quantity,int max_rank);
unsigned long theorem_low(counter* head_filtered);
float get_min_F(float* table , unsigned int max_rank);
float get_max_F(float* table , unsigned int max_rank);
interval* get_floor_and_roof(int quantity , counter* head );
counter* generate_proposition(counter* facture_filtered,int quantity);
void delete_big_table(big_table* target);
big_table* reach_table(int rank,big_table* target);
big_table* reach_last_table(big_table* target);

#endif //_HEADER_FILE_H_
