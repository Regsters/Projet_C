/*Julien MAES*/
/*Hugo MAGERAT*/
/*FIPS 197*/
/* angelfire.com/biz7/atleast/mix_columns.pdf*//*fut très TRES utile pour comprendre*/


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
    
void Mixcolumns(uint8_t* plaintext_line){
	uint8_t ciphertext_dim[4][4];
	int i,j,a,k;
	a=0;
	for (i=0; i<4; i++){
		for (j=0 ; j<4; j++){
			ciphertext_dim[i][j] = plaintext_line[a];
			a++;}			
	}
	uint8_t Rijndael_matrix[16]= {2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2};
	uint8_t pending[64];
	a=0;
	for (k=0; k<4; k++){
		int b=0;
		for (i=0; i<4; i++){
			for (j=0 ; j<4; j++){
				if ( Rijndael_matrix[b] == 1){
						pending[a] = ciphertext_dim[j][k];}
					
				if ( Rijndael_matrix[b] == 2){
						unsigned result = ((ciphertext_dim[j][k])<<1);
						if ((ciphertext_dim[j][k]>>4)>=8){
						/*attention si le byte a un 1 au debut, il faut shifter et XOR avec 0x1b*/
							pending[a] = result^(0x1B);
						}
						else{
							pending[a] = result;
						}
				}		
				if ( Rijndael_matrix[b] == 3){
						unsigned result = ((ciphertext_dim[j][k])<<1);
						if ((ciphertext_dim[j][k]>>4)>=0x8){
						/*attention si le byte a un 1 au debut(position2⁷, il faut shifter et XOR avec 0x1b*/
							pending[a] = (result)^(0x1B)^(ciphertext_dim[j][k]);
						}
						else{
							pending[a] = (result)^(ciphertext_dim[j][k]);
						}
				
				}
				a++;
				b++;
			}
		}
	}
	for (j=0; j<4; j++){
		for (i=0; i<4; i++){/*attention*/

			ciphertext_dim[i][j] = pending[(i*4)+(j*16)]^pending[((i*4)+(j*16)+1)]^pending[((i*4)+(j*16)+2)]^pending[((i*4)+(j*16)+3)];
		}
	}
}	

void main(){
	uint8_t plaintext_line[16] = {0xd4, 0xbf, 0x5d, 0x30,
				      0xbf, 0x40, 0x9f, 0x96,
				      0x5d, 0x3d, 0x7e, 0x11,
				      0x30, 0x93, 0x17, 0x2a};
	uint8_t key_line[16] = {0x2b, 0x7e, 0x15, 0x16, 0x2c, 0x8a, 0xed, 0x2a, 0x6a, 0xbf, 0x71, 0x58, 0x80, 0x9c, 0xf4, 0xf3};
	char key_dim[4][4]; 
	int a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			key_dim[i][j] = key_line[a];
			a++;
			}
	}
	Mixcolumns(plaintext_line);
}
