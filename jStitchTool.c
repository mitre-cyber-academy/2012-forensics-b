#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


#define JPEG_ENDBYTES 0xFFD9

int main(int argc, char* argv[])
{
	char* imgFileName = argv[1];
	char* archFileName = argv[2];
	char* outFileName = argv[3];
	char* imgBuffer, *archiveBuffer;
	FILE* imgFile, *archFile, *outFile;
	struct stat imgFileStats, archFileStats;
	int iFD, aFD, readBytes;
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s <jpgFileName> <archiveFileName> <outFileName>\n", argv[0]);
		exit(0);
	}
	imgFile = fopen(imgFileName, "r");
	if(imgFile == NULL)
	{
		perror("Couldn't open file");
		exit(1);
	}
	archFile = fopen(archFileName, "r");
	if(archFile == NULL)
	{
		perror("Couldn't open file");
		fclose(imgFile);
		exit(2);
	}
	outFile = fopen(outFileName, "w");
	if(outFile == NULL)
	{
		perror("Couldn't open file");
		fclose(imgFile);
		fclose(archFile);
		exit(3);
	}
	/*Assuming all of the files are good now.*/
	/*Now, will get the fd's for these for stat'ing*/
	iFD = fileno(imgFile);
	aFD = fileno(archFile);
	fstat(iFD, &imgFileStats);
	fstat(aFD, &archFileStats);

	/*Now, have the stats, which means we have*/
	/*the size*/
	printf("File size: %ld\n", imgFileStats.st_size);
	imgBuffer = (char*) malloc(imgFileStats.st_size * sizeof(char));
	readBytes = fread(imgBuffer, imgFileStats.st_size, 1, imgFile);
	fclose(imgFile);

	/*Now, we have the image in our buffer. Let's read the archive*/
	
	archiveBuffer = (char*) malloc(archFileStats.st_size * sizeof(char));
	readBytes = fread(archiveBuffer, archFileStats.st_size, 1, archFile);
	fclose(archFile);
	
	fwrite(imgBuffer, imgFileStats.st_size, 1, outFile);
	free(imgBuffer);
	fwrite(archiveBuffer, archFileStats.st_size, 1, outFile);
	free(archiveBuffer);
	fprintf(outFile, "%x", JPEG_ENDBYTES);
	fclose(outFile);

	printf("Operations successful\n");

	return 0;
}
