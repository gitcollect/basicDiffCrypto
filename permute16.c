/*  Func:		permute16													  */
/*  Summary:	permutes 16 bits corresponding to the a permutation implemen- */
/*				-ted as an array; p[i] corresponds to the number of left	  */
/*              shift needed, where negative corresponds to a right shift.    */

#include "basicSPN.h"
unsigned short po2[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
							2048, 4096, 8192,16384, 32768};

// permute16 applies to permutation defined by perm to in; if flag==-1 we
// apply the inverse permutation
unsigned short permute16(unsigned short in, struct perm p, int flag)
{

	unsigned short out = 0;
	int i;
	char * perm;

	if(flag == -1) {
		perm = p.isigma;
	}
	else {
		perm = p.sigma;
	}

	for(i = 0; i <= 15; i++) {
		if(perm[i] >= 0) {
			out |= ((in & po2[i]) << perm[i]);
		}
		else {
			out |= ((in & po2[i]) >> (-1)*perm[i]);
		}

	}

	return out;

}

void inv_permutation(struct perm * p)
{

	int i,j;

	for(i = 0; i <=15 ; i++){
		for(j = 0; j<=15; j++) {
			if(p->sigma[j] + j == i) {
				p->isigma[i] = j-i;
				break;
			}
		}
	}

}

#ifdef PERM_TEST 

#include <stdio.h>

int main(int argc, char ** argv)
{
	// check that all shifts are working.
	int i;
	struct perm p;
	char t[16]= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -15};

	for(i=0; i<=15; i++) {
		p.sigma[i] = t[i];
	}

	inv_permutation(&p);

	for(i = 0; i <= 15; i++) {
		printf("po2: %2d  perm:%u\n", i, permute16(po2[i], p,0));
	}
	printf("\n");
	for(i = 0; i<=15; i++) {
		printf("po2: %2d  perm:%u\n", i, permute16(po2[i], p,-1));
	}

	unsigned short in = 0x2f2a;
	printf("%x\n", permute16(in, p, 0));

	return 0;
}

#endif
