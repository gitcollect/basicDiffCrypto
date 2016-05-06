#include "basicSPN.h"

union nibble apply_sbox4(union nibble in, struct Sbox sbox, int flag)
{
	union nibble out;

	if(flag == -1) {
		out = sbox.inv_pi[in.lsn];
	}
	else {
		out = sbox.pi[in.lsn];
	}
	out.msn = 0;

	return out;

}

union uint16 apply_sbox(union uint16 in, struct Sbox sbox,int flag)
{
	union uint16 out;

	if(flag == -1){
		out.n1 = sbox.inv_pi[in.n1].lsn;
		out.n2 = sbox.inv_pi[in.n2].lsn;
		out.n3 = sbox.inv_pi[in.n3].lsn;
		out.n4 = sbox.inv_pi[in.n4].lsn;
	}
	else {
		out.n1 = sbox.pi[in.n1].lsn;
		out.n2 = sbox.pi[in.n2].lsn;
		out.n3 = sbox.pi[in.n3].lsn;
		out.n4 = sbox.pi[in.n4].lsn;
	}

	return out;

}
#ifdef SBOX_TEST

int main(int argc, char** argv)
{

	union nibble x;
	x.byte = 0x0;
	struct Sbox sbox = { {0x4, 0x1, 0xe, 0x8, 0xd, 0x6, 0x2, 0xb, 0xf, 0xc, 0x9, 0x7, 0x3, 0xa, 0x5, 0x0}, {0xf, 0x1, 0x6, 0xc, 0x0, 0xe, 0x5, 0xb, 0x3, 0xa, 0xd, 0x7, 0x9, 0x4, 0x2, 0x8}};

	union uint16 test;
	test.value = 0x0204;
	test = apply_sbox(test, sbox, -1);
	printf("%x\n", test.value);
	printf("%x %x\n", sbox.pi[0].msn, sbox.pi[0].lsn);
	printf("0x%x \n", (apply_sbox4(x, sbox,-1).byte));

	return 0;

}

#endif
