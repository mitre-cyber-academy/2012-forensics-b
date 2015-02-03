#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#define __USE_GNU
#include <string.h>

#define JPEG_ENDBYTES 0xFFD9

int main(int argc, char* argv[])
{
	char* imgFileName = argv[1];
	char* archFileName = argv[2];
	char* inFileName = argv[3];
	char* inFileBuffer;
	char endChars[] = {'\xFF', '\xD9'};
	char* imgEnd;
	FILE* imgFile, *archFile, *inFile;
	struct stat inFileStats;
	int iFD, tempSize;
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s <jpgFileName> <archiveFileName> <inFileName>\n", argv[0]);
		exit(0);
	}
	imgFile = fopen(imgFileName, "w");
	if(imgFile == NULL)
	{
		perror("Couldn't open file");
		exit(1);
	}
	archFile = fopen(archFileName, "w");
	if(archFile == NULL)
	{
		perror("Couldn't open file");
		fclose(imgFile);
		exit(2);
	}
	inFile = fopen(inFileName, "r");
	if(inFile == NULL)
	{
		perror("Couldn't open file");
		fclose(imgFile);
		fclose(archFile);
		exit(3);
	}
	/*Assuming all of the files are good now.*/
	/*Now, will get the fd's for these for stat'ing*/
	iFD = fileno(inFile);
	fstat(iFD, &inFileStats);

	/*Now, have the stats, which means we have
	  the size*/
	printf("File size: %ld\n", inFileStats.st_size);
	inFileBuffer = (char*) malloc(inFileStats.st_size * sizeof(char));
	fread(inFileBuffer, inFileStats.st_size, 1, inFile);
	fclose(inFile);

	/*Now, we have the stitched file in our buffer
	 * Let's search for the guard characters next. */
	
	/*The sequence that we're looking for is defined in endChars, so
	 * going to use the GNU memory search memmem */
	imgEnd = memmem(inFileBuffer, inFileStats.st_size, 
			endChars, 2);
	if(imgEnd == NULL)
	{
		fprintf(stderr, "Couldn't find the guard bytes. Exiting...\n");
		free(inFileBuffer);
		fclose(imgFile);
		fclose(archFile);
		exit(4);
	}
	printf("We found the bytes. Processing...\n");
	/* Want to get the second bit from two bytes later */
	imgEnd += 2;
	/* Size of first chunk = difference between start ptr and end */
	tempSize = imgEnd - inFileBuffer; 

	fwrite(inFileBuffer, tempSize, 1, imgFile);
	fclose(imgFile);
	fwrite(imgEnd, inFileStats.st_size - tempSize, 1, archFile);
	fclose(archFile);

	printf("Operations successful\n");

	free(inFileBuffer);
	return 0;
}
