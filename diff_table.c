#include "basicSPN.h"


struct table diff_table(struct Sbox sbox)
{

	struct table tb;
	int i, j = 0;

	for(i=0; i<=15; i++) {
		for(j = 0; j <=15; j++) {
			tb.table[i][j] = 0;
		}
	}

	union nibble a, astar, b, bstar, bprime;
	a.byte = 0;
	astar.byte = 0;
	// a' corresponds to i;
	for(i = 0; i<=15 ; i++) {
		// b' corresponds to j
		for(j = 0; j<=15; j++) {
			a.byte = j;
			astar.byte = j ^ i;
			b = apply_sbox4(a, sbox,1);
			bstar = apply_sbox4(astar, sbox,1);
			bprime.lsn = bstar.lsn ^ b.lsn;
			tb.table[i][bprime.lsn] += 1;
		}
	}

	return tb;
}

void print_diff_table(struct table tb)
{
	int i,j = 0;
	// print header
	printf("a |");
	for(i = 0; i<= 15; i++){

		printf(" %2X", i);

	}
	printf("\n");
	for(i = 0; i<=25; i++)
		printf("--");

	printf("\n");

	for(i = 0; i<=15; i++) {
		printf("%X |", i);
		for(j = 0; j<=15; j++){
			if(tb.table[i][j] == 0) {
				printf(" %2c", '-');
			} else {
				printf(" %2hu", tb.table[i][j]);
			}
		}
		printf("\n");
	}

}

#ifdef TABLE_TEST

int main(char argc, char **argv)
{

	struct table tb;
	int i, j = 0;
//	struct Sbox sbox = { .pi = {0x4, 0x1, 0xe, 0x8, 0xd, 0x6, 0x2, 0xb, 0xf, 0xc, 0x9, 0x7, 0x3, 0xa, 0x5, 0x0}};

	struct Sbox sbox = {.pi = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}, .inv_pi = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5}};

	for(i =0; i<=15 ; i++)
		for(j = 0; j <=15; j++)
			tb.table[i][j] = (i*j) % 100;


	tb = diff_table(sbox);

	print_diff_table(tb);

	return 0;

}

#endif
