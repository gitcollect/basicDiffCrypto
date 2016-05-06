#include <stdio.h>
#include <stdlib.h>

/******************** STRUCTURES USED **********************/
   union uint16{
	unsigned short value;
	struct {
		unsigned int n1:4; //least significant
		unsigned int n2:4;
		unsigned int n3:4;
		unsigned int n4:4; // most significant
	};
};

union nibble{
	unsigned char byte;
	struct {
		unsigned int lsn:4; //most significant nibble
		unsigned int msn:4; //least significant nibble
	};
};

struct Sbox {
	union nibble pi[16];
	union nibble inv_pi[16];
};

struct perm {
	char sigma[16];
	char isigma[16];
};

struct table {
	unsigned short table[16][16];
};

/************* function declarations ******************/

unsigned short basicSPN(unsigned short message, unsigned short key[5], struct Sbox sbox, struct perm p, int flag);

union nibble apply_sbox4(union nibble in, struct Sbox sbox,int flag);
unsigned short permute16(unsigned short in, struct perm p, int flags);
void  inv_permutation(struct perm * p );
union uint16 apply_sbox(union uint16 in, struct Sbox sbox,int flag);
struct table diff_table(struct Sbox sbox);
void print_diff_table(struct table tb);
