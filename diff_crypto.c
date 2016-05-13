#include "basicSPN.h"

/************************* SPN PARAMETERS **********************/
unsigned short key[5] = { 0xbeef, 0xb33f, 0xd00d, 0x1337, 0xdead};
//static
struct perm p = {.sigma = {0,3,6, 9, -3, 0, 3, 6, -6, -3, 0, 3, -9, -6, -3, 0}};

#ifdef CLASS

struct Sbox sbox = { .pi = {0x4, 0x1, 0xe, 0x8, 0xd, 0x6, 0x2, 0xb, 0xf, 0xc, 0x9, 0x7, 0x3, 0xa, 0x5, 0x0}, .inv_pi = {0xf, 0x1, 0x6, 0xc, 0x0, 0xe, 0x5, 0xb, 0x3, 0xa, 0xd, 0x7, 0x9, 0x4, 0x2, 0x8}};
unsigned short id =  0xf00;
unsigned short od =  0xcc00;

#endif
#ifdef BOOK


struct Sbox sbox = {.pi = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}, .inv_pi = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5}};
unsigned short id = 0x0b00;
unsigned short od = 0x0606;

#endif
/***************************************************************/


unsigned short spn_oracle(unsigned short msg,int flag)
{

	return basicSPN(msg, key, sbox, p, flag);

}

void diff_c(unsigned short (*oracle)(unsigned short msg, int flag), unsigned short input_diff, unsigned short output_diff)
{

	unsigned short i,j;
	unsigned short limit  = ~0;

	union uint16 y_star, Y, o_diff, i_diff;
	o_diff.value = output_diff;
	i_diff.value = input_diff;

	unsigned short r,s;

	union uint16 sub_keys;
	static unsigned short g_key_count[16][16][16][16];

	int x,y,w,z;
	int tx, ty, tw, tz;
	// Here we try every possible input difference.
	// This might not desirable in block ciphers with larger block size.

	for(i = 0; i < limit; i++) {
		// picks the correct input pair.
		j = input_diff ^ i;

		for(x = 0; x <= 15; x++) {
			tx = (o_diff.n4 > 0) ? x : 0;
			for(y = 0; y <= 15; y++) {

				ty = (o_diff.n3 > 0) ? y : 0;
				for(w = 0; w <= 15; w++) {

					tw = (o_diff.n2 > 0) ? w : 0;
					for(z = 0; z <= 15; z++) {

						tz = (o_diff.n1 > 0) ? z : 0;
						sub_keys.n4 = tx;
						sub_keys.n3 = ty;
						sub_keys.n2 = tw;
						sub_keys.n1 = tz;


						y_star.value = oracle(i,1) ^ sub_keys.value;
						Y.value = oracle(j,1) ^ sub_keys.value;

						y_star = apply_sbox(y_star, sbox, -1);
						Y = apply_sbox(Y, sbox, -1);

						Y.value = (Y.value ^ y_star.value); // Now y.value is what we called y' in the notes.

						if((Y.n1 == o_diff.n1) && (Y.n2 == o_diff.n2) && (Y.n3 == o_diff.n3) && (Y.n4 == o_diff.n4)) {
							g_key_count[x][y][w][z] += 1;
						}

						if(!(o_diff.n1))
							break;
					}
					if(!(o_diff.n2))
						break;
				}
				if(!(o_diff.n3))
					break;
			}
			if(!(o_diff.n4))
				break;
		}
	}


	int max = g_key_count[0][0][0][0];
	unsigned short mx = 0;
	unsigned short my = 0;
	unsigned short mw = 0;
	unsigned short mz = 0;

	for(x = 0; x < 16; x++) {
		for(y = 0; y < 16; y++) {
			for(w = 0; w < 16; w++) {
				for(z = 0; z < 16; z++) {

					if(g_key_count[mx][my][mw][mz] < g_key_count[x][y][w][z]) {
						max = g_key_count[x][y][w][z];
						mx = x;
						my = y;
						mw = w;
						mz = z;
					}
				}
			}
		}
	}


	printf("partial key:[%x,%x,%x,%x]  -- count = %d\n", mx,my,mw,mz,max);

}

#ifdef DIFF_TEST
int main(char argc, char **argv)
{

	diff_c(spn_oracle, id, od);

	return 0;

}
#endif
