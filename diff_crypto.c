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
#ifdef GENERAL

struct Sbox sbox = {.pi = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}, .inv_pi = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5}};
unsigned short id = 0x0b00;
unsigned short od = 0x0606;

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

	static int key_count[16][16];

	for(i = 0; i< 16 ; i++) {
		for(j = 0; j <16; j++) {
			key_count[i][j] = 0;
		}
	}


	union uint16 y_star, Y, o_diff, i_diff;
	o_diff.value = output_diff;
	i_diff.value = input_diff;
	// candidate key
	union uint16 key;
	key.value = 0;

	unsigned short r,s;


#ifdef GENERAL

	union uint16 sub_keys;
	static unsigned short g_key_count[16][16][16][16];

	int x,y,w,z;
	int tx, ty, tw, tz;
	// Here we try every possible input difference.
	// This might not desirable in block ciphers with larger block size.



	int nibbles[4] = {o_diff.n4, o_diff.n3, o_diff.n2, o_diff.n1};
	//                            x          y          w           z
	int finished = 0;

	for(i = 0; i < limit; i++) {
		// picks the correct input pair.
		j = input_diff ^ i;

		for(x = 0; x <= 15; x++) {
			tx = (nibbles[0] > 0 ) ? x : 0;
			for(y = 0; y <= 15; y++) {

				ty = (nibbles[1] > 0) ? y : 0;
				for(w = 0; w <= 15; w++) {

					tw = (nibbles[2]> 0) ? w : 0;
					for(z = 0; z <= 15; z++) {

						tz = (nibbles[3]>0) ? z : 0;
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

						if(!nibbles[3])
							break;
					}
					if(!nibbles[2])
						break;
				}
				if(!nibbles[1])
					break;
			}
			if(!nibbles[0])
				break;
		}
	}


#endif

	for(i = 0; i < limit ;  i++) {
		// picks out a correct input pair.
		j = input_diff ^ i;
		for(r = 0; r <=15 ; r++) {
			for(s = 0; s <=15; s++) {


#ifdef CLASS
				// n4 is the leftmost nibble in the SPN picture from class.
				key.n4 = r;
				key.n3 = s;

				y_star.value = oracle(i,1) ^ key.value;
				y.value = oracle(j,1) ^ key.value;

				y_star = apply_sbox(y_star, sbox, -1);
				y = apply_sbox(y, sbox, -1);

				y.value = (y.value ^ y_star.value); // Now y.value is what we called y' in the notes.

				if((y.n2 != 0) || (y.n1 != 0)) {
					continue;
				}

				if((y.n4 == o_diff.n4)&&(y.n3 == o_diff.n3)) {
					key_count[r][s] += 1;
				}
#endif
#ifdef BOOK
				union uint16 y;
				key.n3 = r ;
				key.n1 = s ;

				y_star.value = oracle(i, 1) ^ key.value;
				y.value = oracle(j,1) ^ key.value;

				y_star = apply_sbox(y_star, sbox, -1);
				y = apply_sbox(y, sbox, -1);

				y.value = ( y_star.value ^ y.value); // Now y.value is what we called y' in the notes.

				if((y.n2 != 0) || (y.n4 != 0)) {
					continue;
				}

				if((y.n1 == o_diff.n1)&&(y.n3 == o_diff.n3)) {
					key_count[r][s] += 1;
				}

#endif
			}
		}
	}
#ifndef GENERAL
	int max = key_count[0][0];
	unsigned short mr = 0;
	unsigned short ms = 0;

	for(i = 0; i < 16; i++ ) {
		for(j =0; j < 16; j++) {
			//if(key_count[i][j] > 0) {
			//	printf("[%x,%x] = %d\n", i, j, key_count[i][j]);
			//}
			if(key_count[mr][ms] < key_count[i][j]) {
				max = key_count[i][j];
				mr = i;
				ms = j;
			}
		}
	}

	printf("partial key:[%x,%x]  -- count = %d\n", mr, ms,max);

#endif
#ifdef GENERAL

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
#endif

}

#ifdef DIFF_TEST
int main(char argc, char **argv)
{

	diff_c(spn_oracle, id, od);

	return 0;

}
#endif
