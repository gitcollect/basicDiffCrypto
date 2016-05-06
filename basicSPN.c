#include "basicSPN.h"

unsigned short basicSPN(unsigned short message, unsigned short key[5], struct Sbox sbox, struct perm p, int flag)
{

	int i = 0;

	union uint16 ciphertext;
	ciphertext.value = message;
	unsigned short k[5] = {0, 0, 0, 0 , 0 };

	for(i=0;i<=4; i++) {
		if(flag == -1){
			if((i != 0) & (i != 4)) {
				k[4-i] = permute16(key[i], p,-1);
			}
			else {
				k[4-i] = key[i];
			}
		}
		else {
			k[i] = key[i];
		}
	}
	for(i = 0; i <=2; i++) {
		ciphertext.value ^= k[i];
		ciphertext =  apply_sbox(ciphertext, sbox,flag);
		ciphertext.value  = permute16(ciphertext.value, p,flag);
	}

	ciphertext.value ^= k[i];
	ciphertext =  apply_sbox(ciphertext, sbox, flag);
	i++;
	ciphertext.value ^= k[i];
	return ciphertext.value;

}

#ifdef SPN_TEST

int main()
{
	int i=0;
	char t[16] = {0,3,6, 9, -3, 0, 3, 6, -6, -3, 0, 3, -9, -6, -3, 0};
	//char t[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-15};
	struct perm p;

	for(i=0; i<=15; i++) {
		p.sigma[i] = t[i];
	}

	inv_permutation(&p);

	//for(i=0; i<=15; i++) {
	//	printf("%d ", p.sigma[i]);
	//	printf("%d ", p.isigma[i]);
	//}
	//printf("\n");
	struct Sbox sbox = { .pi = {0x4, 0x1, 0xe, 0x8, 0xd, 0x6, 0x2, 0xb, 0xf, 0xc, 0x9, 0x7, 0x3, 0xa, 0x5, 0x0}, .inv_pi = {0xf, 0x1, 0x6, 0xc, 0x0, 0xe, 0x5, 0xb, 0x3, 0xa, 0xd, 0x7, 0x9, 0x4, 0x2, 0x8}};

	unsigned short key[5] = {0b1110011101100111, 0b0111011001111001,0b0110011110010000, 0b0111100100000011, 0xdead};
// 0b1001000000111101
	unsigned short msg = 0b0100111010100001;
	printf("msg %x\n", msg);
	unsigned short ctxt = basicSPN(msg, key, sbox, p,0);
	printf("ct %x\n", ctxt);
	printf("msg %x\n", basicSPN(ctxt, key, sbox, p,-1));
	return 0;

}

#endif
