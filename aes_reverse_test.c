/*Julien MAES*/
/*Hugo MAGERAT*/
/*FIPS 197*/
/* angelfire.com/biz7/atleast/mix_columns.pdf*//*fut très TRES utile pour comprendre*/
/*https://crypto.stackexchange.com/questions/2569/how-does-one-implement-the-inverse-of-aes-mixcolumns*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>


static int step = 0;
static const uint8_t rsbox[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

static const uint8_t Rcon[10][4] = {{0x01,0x00, 0x00, 0x00},{0x02,0x00, 0x00, 0x00},{0x04,0x00, 0x00, 0x00},{0x08,0x00, 0x00, 0x00},{0x10,0x00, 0x00, 0x00},{0x20,0x00, 0x00, 0x00},{0x40,0x00, 0x00, 0x00},{0x80,0x00, 0x00, 0x00},{0x1b,0x00, 0x00, 0x00},{0x36,0x00, 0x00, 0x00}};
  
void Key_schedule(uint8_t (*key_dim)[11][4][4]){
	int round = 1;
	for (int i=0; i<10; i++){
		uint8_t W0[4] = {(*key_dim)[round][0][0], (*key_dim)[round][1][0], (*key_dim)[round][2][0], (*key_dim)[round][3][0]};
		uint8_t W1[4] = {(*key_dim)[round][0][1], (*key_dim)[round][1][1], (*key_dim)[round][2][1], (*key_dim)[round][3][1]};
		uint8_t W2[4] = {(*key_dim)[round][0][2], (*key_dim)[round][1][2], (*key_dim)[round][2][2], (*key_dim)[round][3][2]};
		uint8_t W3[4] = {(*key_dim)[round][0][3], (*key_dim)[round][1][3], (*key_dim)[round][2][3], (*key_dim)[round][3][3]};
		uint8_t W4[4] = {(*key_dim)[round][1][3], (*key_dim)[round][2][3], (*key_dim)[round][3][3], (*key_dim)[round][0][3]};
		uint8_t W5[4];
		uint8_t W6[4];
		uint8_t W7[4];
		for (int i=0; i<4; i++){
			W4[i] =  sbox[(int)W4[i]];
		}
		for (int i=0; i<4; i++){
			W4[i] = W4[i]^W0[i]^Rcon[round][i];
		}
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
						(*key_dim)[round][i][j] = W4[j];
					}
					break;
				case 1: 
					for (int j=0 ; j<4; j++){	
						(*key_dim)[round][i][j] = W5[j];
					}
					break;
				case 2:
					for (int j=0 ; j<4; j++){	
						(*key_dim)[round][i][j] = W6[j];
					}
					break;
				case 3:
					for (int j=0 ; j<4; j++){	
						(*key_dim)[round][i][j] = W7[j];
					}
					break;
			}		
		}
		step++;
	}
}

void AddRoundKey(uint8_t (*plaintext_dim)[4][4], uint8_t  (*key_dim)[4][4]){
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = (*plaintext_dim)[i][j]^(*key_dim)[step][i][j];
		}
	}
	step++;
}

void inv_SubBytes(uint8_t (*plaintext_dim)[4][4]){
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = rsbox[(int)(*plaintext_dim)[i][j]];
		}
	}
}

void inv_ShiftRows(uint8_t (*plaintext_dim)[4][4]){ /* a tester*/
	uint8_t pending_dim[4][4];
	uint8_t pending_dim[4][4];
	for (int i=0; i<4; i++){
		int a =3;
		for (int j=0 ; j<4; j++){
			if (j-i>=0){
				pending_dim[i][j] = (*plaintext_dim)[i][(j-i)];
			}
			else{
				pending_dim[i][j] = (*plaintext_dim)[i][a];
				a--;
			}		
		}
	}
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			(*plaintext_dim)[i][j] = pending_dim[i][j];
		}
	}
}
void inv_MixColoumns(uint8_t (*plaintext_dim)[4][4]){ /* a tester*/
	uint8_t Rijndael_matrix[16]= {0x0e,0x0b,0x0d,0x09,0x09,0x0e,0x0b,0x0d,0x0d,0x09,0x0e,0x0b,0x0b,0x0d,0x09,0x0e};
	uint8_t pending[64];
	int a=0;
	unsigned result1;
	unsigned result2;
	unsigned result3;
	unsigned result4;
	unsigned result5;
	unsigned result6;
	for (int k=0; k<4; k++){
		int b=0;
		for (int i=0; i<4; i++){
			for (int j=0 ; j<4; j++){
				if ( Rijndael_matrix[b] == 0x09){
						result1 = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
							result1 = result1^(0x1B);
						}
						result2 = ((result1)<<1);
						if ((result1>>4)>=0x8){
							result2 = result2^(0x1B);
						}
						result3 = ((result2)<<1);
						if ((result2>>4)>=0x8){
							result3 = result3^(0x1B);
						}
						pending[a] = result3^(*plaintext_dim)[j][k]);				
				}
				if ( Rijndael_matrix[b] == 0x0b){
						result1 = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
							result1 = result1^(0x1B);
						}
						result2 = ((result1)<<1);
						if ((result1>>4)>=0x8){
							result2 = result2^(0x1B);
						}
						result2 = result2^(*plaintext_dim)[j][k]);
						result3 = ((result2)<<1);
						if ((result2>>4)>=0x8){
							result3 = result3^(0x1B);
						}				
						pending[a] = result3^(*plaintext_dim)[j][k]);
				}
				if ( Rijndael_matrix[b] == 0x0d){
						result1 = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
							result1 = result1^(0x1B);
						}
						result1 = result1^(*plaintext_dim)[j][k]);
						result2 = ((result1)<<1);
						if ((result1>>4)>=0x8){
							result2 = result2^(0x1B);
						}
						result3 = ((result2)<<1);
						if ((result2>>4)>=0x8){
							result3 = result3^(0x1B);
						}
						pending[a] = result3^(*plaintext_dim)[j][k]);
				}
				if ( Rijndael_matrix[b] == 0x0e){
						result1 = (((*plaintext_dim)[j][k])<<1);
						if (((*plaintext_dim)[j][k]>>4)>=0x8){
							result1 = result1^(0x1B);
						}
						result1 = result1^(*plaintext_dim)[j][k]);
						result2 = ((result1)<<1);
						if ((result1>>4)>=0x8){
							result2 = result2^(0x1B);
						}
						result2 = result2^(*plaintext_dim)[j][k]);
						result3 = ((result2)<<1);
						if ((result2>>4)>=0x8){
							result3 = result3^(0x1B);
						}
						pending[a] = result3;
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

	uint8_t key_dim[11][4][4]; 
	int a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			key_dim[0][i][j] = key_line[a];
			a++;
			}
	}

	uint8_t plaintext_dim[4][4]; 
	a = 0;
	for (int i=0; i<4; i++){
		for (int j=0 ; j<4; j++){
			plaintext_dim[i][j] = plaintext_line[a];
			a++;
			}
	}
	
	key_schedule(&key_dim)
	AddRoundKey(&plaintext_dim, &key_dim);
	for ( int i = 0; i<9 ; i++){
		inv_ShiftRows(&plaintext_dim); 
		inv_SubBytes(&plaintext_dim);
		AddRoundKey(&plaintext_dim, &key_dim);}
		inv_MixColoumns(&plaintext_dim); 
	inv_ShiftRows(&plaintext_dim); 
	inv_SubBytes(&plaintext_dim);	
	AddRoundKey(&plaintext_dim, &key_dim);
	
}
















































