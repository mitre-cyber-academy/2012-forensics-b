# Forensics 100 – Multi-Level Steganography

### Description of challenge
Participants are provided with an image file which appears to be the character Guard Duck from 'Pearls before Swine.' However, there is another file tacked on to the end of the jpeg, separated by the 0xFF 0xD9 end bytes which delimit a JPEG file. The following file is a bz2 compressed tar archive, which has another 0xFF 0xD9 at the end to make it not quite as obviously an archive. Included is a tool, ripSeamJ.bin (source in jRipSeam.c) which is designed to split a jpeg file into two files based on the guard bytes in the middle. A Makefile is included and the tool is intended to build on a Linux system with glibc >= 2.1 with gcc.

Once the archive file is revealed, it can be extracted. Inside are four files: an image of Hagar and Helga's duck Kvack, from 'Hagar the Horrible’ and the images of real ducklings. If the strings program is run on kvack.jpg, it reveals the text: “steghidepw=helga'spet.” This should prompt the participant to acquire the steghide program. When run on the other jpegs and the correct password is entered, a set of cartoon duck images is generated.

In the three duck files, separate parts of the key can be found. In the picture of Daffy Duck, the first part of the key, 'MCA-,' can be found by zooming into his belly. In the picture of Scrooge McDuck, four characters in the key can be found on his little red bucket. In the picture of Donald Duck, an xcf with layers, the flag is reversed and flipped in a layer behind the image of Donald. Assembling these three sets of flags gives a fifty/fifty chance of having the right flag, which is a low number of combinations required to get the correct answer.

### Required programs:
* gcc
* make
* steghide
* strings
* An image viewer (for jpeg, xcf, png)
* An archive extraction tool which can handle .tar.bz2 archives (like 7 zip)

## Files included:

#### Generation
* jStitchTool.c
  * This takes in two files, a jpeg and an archive, then stitches them together into a jpeg

#### Decoding 
* jRipSeam.c
  * This takes in one file, then splits it back into a jpeg and an archive
* Makefile
  * This is used to compile the two tools

#### Challenge
* forensics100.jpg
  * This is the challenge image

Flag: MCA-570F5B8A
