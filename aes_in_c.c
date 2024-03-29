/*Julien MAES*/
/*Hugo MAGERAT*/
/*FIPS 197*/
/* angelfire.com/biz7/atleast/mix_columns.pdf*//*fut très TRES utile pour comprendre*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>

static int  round = 0;
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
  
static const uint8_t Rcon[10][4] = {{0x01,0x00, 0x00, 0x00},{0x02,0x00, 0x00, 0x00},{0x04,0x00, 0x00, 0x00},{0x08,0x00, 0x00, 0x00},{0x10,0x00, 0x00, 0x00},{0x20,0x00, 0x00, 0x00},{0x40,0x00, 0x00, 0x00},{0x80,0x00, 0x00, 0x00},{0x1b,0x00, 0x00, 0x00},{0x36,0x00, 0x00, 0x00}};
  
void Key_schedule(uint8_t (*key_dim)[4][4]){
	uint8_t W0[4] = {(*key_dim)[0][0], (*key_dim)[1][0], (*key_dim)[2][0], (*key_dim)[3][0]};
	uint8_t W1[4] = {(*key_dim)[0][1], (*key_dim)[1][1], (*key_dim)[2][1], (*key_dim)[3][1]};
	uint8_t W2[4] = {(*key_dim)[0][2], (*key_dim)[1][2], (*key_dim)[2][2], (*key_dim)[3][2]};
	uint8_t W3[4] = {(*key_dim)[0][3], (*key_dim)[1][3], (*key_dim)[2][3], (*key_dim)[3][3]};
	uint8_t W4[4] = {(*key_dim)[1][3], (*key_dim)[2][3], (*key_dim)[3][3], (*key_dim)[0][3]};
	uint8_t W5[4];
	uint8_t W6[4];
	uint8_t W7[4];
	for (int i=0; i<4; i++){
		W4[i] =  sbox[(int)W4[i]];
	}
	for (int i=0; i<4; i++){
		W4[i] = W4[i]^W0[i]^Rcon[round][i];
	}
	round++;
	for (int i=0; i<4; i++){
		W5[i] = W4[i]^W1[i];
		W6[i] = W5[i]^W2[i];
		W7[i] = W6[i]^W3[i];
	}
	for (int i=0; i<4; i++){
	/*uint8_t* key_dim[i] = {W4, W5, W6, W7}; /* array of pointer, ne fct° pas car on chg le type  */
		switch(i){
			case 0: 
				for (int j=0 ; j<4; j++){	
					(*key_dim)[j][0] = W4[j];
				}
				break;
			case 1: 
				for (int j=0 ; j<4; j++){	
					(*key_dim)[j][1] = W5[j];
				}
				break;
			case 2:
				for (int j=0 ; j<4; j++){	
					(*key_dim)[j][2] = W6[j];
				}
				break;
			case 3:
				for (int j=0 ; j<4; j++){	
					(*key_dim)[j][3] = W7[j];
				}
				break;
		}	
	}
}

void AddRoundKey(uint8_t (*plaintext_dim)[4][4], uint8_t  (*key_dim)[4][4]){
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = (*plaintext_dim)[i][j]^(*key_dim)[i][j];
		}
	}
}

void SubBytes(uint8_t (*plaintext_dim)[4][4]){
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = sbox[(int)(*plaintext_dim)[i][j]];
		}
	}
}

void ShiftRows(uint8_t (*plaintext_dim)[4][4]){ 
	uint8_t pending_dim[4][4];
	for (int i=0; i<4; i++){
		int a =0;
		for (int j=0 ; j<4; j++){
			if (j+i<4){
				pending_dim[i][j] = (*plaintext_dim)[i][(j+i)];
			}
			else{
				pending_dim[i][j] = (*plaintext_dim)[i][a];
				a++;
			}		
		}
	}
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = pending_dim[i][j];
		}
	}
}

void MixColoumns(uint8_t (*plaintext_dim)[4][4]){
	uint8_t Rijndael_matrix[16]= {2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2};
	uint8_t pending[64];
	int a=0;
	for (int k=0; k<4; k++){
		int b=0;
		for (int i=0; i<4; i++){
			for (int j=0 ; j<4; j++){
				if ( Rijndael_matrix[b] == 1){
						pending[a] = (*plaintext_dim)[j][k];}
					
				if ( Rijndael_matrix[b] == 2){
						unsigned result = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
						/*attention si le byte a un 1 au debut, il faut shifter et XORer avec 0x1b*/
							pending[a] = result^(0x1B);
						}
						else{
							pending[a] = result;
						}
				}		
				if ( Rijndael_matrix[b] == 3){
						unsigned result = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
						/*attention si le byte a un 1 au debut(position2⁷, il faut shifter et XORer avec 0x1b*/
							pending[a] = (result)^(0x1B)^((*plaintext_dim)[j][k]);
						}
						else{
							pending[a] = (result)^((*plaintext_dim)[j][k]);
						}
				
				}
				a++;
				b++;
			}
		}
	}
	for (int j=0; j<4; j++){
		for (int i=0; i<4; i++){/*attention*/
			(*plaintext_dim)[i][j] = pending[(i*4)+(j*16)]^pending[((i*4)+(j*16)+1)]^pending[((i*4)+(j*16)+2)]^pending[((i*4)+(j*16)+3)];
		}
	}
}

void main(){
	uint8_t key_line[16] = {0x54, 0x68, 0x61, 0x74,
				0x73, 0x20, 0x6d, 0x79,
				0x20, 0x4b, 0x75, 0x6e,
				0x67, 0x20, 0x46, 0x75};
				
	uint8_t plaintext_line[16] = {0x54, 0x77, 0x6f, 0x20,
				     0x4f, 0x6e, 0x65, 0x20,
				     0x4e, 0x69, 0x6e, 0x65,
				     0x20, 0x54, 0x77, 0x6f};
	/* ceci est le format espéré en sortie de fichier quand on appelera AES*/			

	uint8_t key_dim[4][4]; 
	int a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			key_dim[j][i] = key_line[a];
			a++;
			}
	}

	uint8_t plaintext_dim[4][4]; 
	a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			plaintext_dim[j][i] = plaintext_line[a];
			a++;
			}
	}


	AddRoundKey(&plaintext_dim, &key_dim); /*ok*/
	for ( int i = 0; i<9 ; i++){
		SubBytes(&plaintext_dim); /*ok*/
		ShiftRows(&plaintext_dim); /*ok*/
		MixColoumns(&plaintext_dim); /*ok*/
		Key_schedule(&key_dim);  /*ok*/
		AddRoundKey(&plaintext_dim, &key_dim);
	}
	SubBytes(&plaintext_dim);
	ShiftRows(&plaintext_dim);	
	Key_schedule(&key_dim);
	AddRoundKey(&plaintext_dim, &key_dim);
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			printf("%hhx ", plaintext_dim[j][i]);
			}
	}
	printf("\n");
	/*29 c3 50 5f 57 14 20 f6 40 22 99 b3 1a 2 d7 3a */

	
}














