kilo: kilo.c #file needed for building
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99 

#-Wall -> All Warnings, shows more errors to avoid simple mistakes
#-Wextra and -pedantic -> show even more errors (i.e unused variables)
#-std=c99 -> Selected C Standard version, in C99 we can declare variables everywhere instead of on top of block.

