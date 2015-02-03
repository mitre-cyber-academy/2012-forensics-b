CC=gcc
CFLAGS=-Wall -pedantic

all: stitchJ.bin ripSeamJ.bin

stitchJ.bin: jStitchTool.c
	$(CC) $(CFLAGS) -o stitchJ.bin jStitchTool.c

ripSeamJ.bin: jRipSeam.c
	$(CC) $(CFLAGS) -o ripSeamJ.bin jRipSeam.c

clean:
	rm *.bin

