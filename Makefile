CFLAGS= -Wall -Wextra -g
CC=gcc
SOURCES=basicSPN.c sbox4.c permute16.c diff_crypto.c diff_table.c

EXECUTABLES=basicSPN a.out diff_c sbox permute diff_table

basicSPN:
	$(CC) -o  basicSPN -D SPN_TEST $(SOURCES)
diff-class:
	$(CC) -o diff_c -D DIFF_TEST -D CLASS $(SOURCES)
diff-book:
	$(CC) -o diff_c -D DIFF_TEST -D BOOK $(SOURCES)

diff-general:
	$(CC) -o diff_c -D DIFF_TEST -D GENERAL $(SOURCES)
sbox:
	$(CC) -o sbox -D SBOX_TEST $(SOURCES)
permute:
	$(CC) -o permute -D PERM_TEST $(SOURCES)
table:
	$(CC) -o diff_table -D TABLE_TEST $(SOURCES)
clean:
	rm -f $(EXECUTABLES)
