/*Julien MAES*/
/*Hugo MAGERAT*/
/*FIPS 197*/


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>

/*
  plain-text:
    6bc1bee22e409f96e93d7e117393172a
    ae2d8a571e03ac9c9eb76fac45af8e51
    30c81c46a35ce411e5fbc1191a0a52ef
    f69f2445df4f9b17ad2b417be66c3710
    
    
  key:
    2b7e151628aed2a6abf7158809cf4f3c*/
    
void shiftRows(uint8_t* plaintext_line){
	char ciphertext_dim[4][4];
	int i,j,a;
	a=0;
	for (i=0; i<4; i++){
		for (j=0 ; j<4; j++){
			int index = a+i;
			if(((4-i)-j)<=0){
				index = a - (4-i); 
				/* 
				a	0 1 2 3		0 1 2 3
				a-3	5 6 7 4		4 5 6 7
				a-2	10 11 8 9	8 9 10 11
				a-1	15 12 13 14	12 13 14 15 */		
			} 
			ciphertext_dim[i][j] = plaintext_line[index];
			a++; 
			}
		}
}

void main(){
	uint8_t plaintext_line[16] = {0x6b, 0xc1, 0xbe, 0xe2,
				      0x2e, 0x40, 0x9f, 0x96,
				      0xe9, 0x3d, 0x7e, 0x11,
				      0x73, 0x93, 0x17, 0x2a};
	uint8_t key_line[16] = {0x2b, 0x7e, 0x15, 0x16, 0x2c, 0x8a, 0xed, 0x2a, 0x6a, 0xbf, 0x71, 0x58, 0x80, 0x9c, 0xf4, 0xf3};
	char key_dim[4][4]; 
	int a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			key_dim[i][j] = key_line[a];
			a++;
			}
	}
	shiftRows(plaintext_line);
}
