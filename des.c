#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "test.h"

#define BLOCK_SIZE 8

//permutation table
int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};
//Final permutation
int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};
//Expansion
int EP[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 31, 31, 32, 01
};
//Straight
int SP[32] = {
    16, 7,20 ,21 ,29 ,12 ,28 ,17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

//패리티 table 64 -> 58
int PC[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};
//Compression Permutation 58 -> 48
int CP[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};
//bit shift
int BS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};
// DES S-Box 테이블 선언 (int 사용)
const int S_BOX_TABLE[8][4][16] = {
    { // S1
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    { // S2
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    { // S3
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    { // S4
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    { // S5
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    { // S6
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    { // S7
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    { // S8
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

uint64_t test_text_to_uint64(const char* text) {

    uint64_t result = 0;
    int len = strlen(text);

    // 최대 8글자까지만 처리
    for (int i = 0; i < len && i < 8; i++) {
        result |= (uint64_t)text[i] << (8 * (7 - i));
    }

    return result;
}

void uint64_to_text(uint64_t value, char* output) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[BLOCK_SIZE - 1 - i] = (char)((value >> (i * 8)) & 0xFF);
    }
    output[BLOCK_SIZE] = '\0'; // 널 종료
}

// key로 사용할 Random 64bit
uint64_t generate_random_64bit() {
    uint64_t random_value = 0;
    /*random_value |= ((uint64_t)rand() << 32); 
    random_value |= (uint64_t)rand();   */      
    random_value = ((uint64_t)rand() << 32) | rand();

    return random_value;
}

uint64_t permutation(uint64_t input, int* table, int n) {

    uint64_t output = 0;
    for (int i = 0; i < n; i++) {
        int bit = (input >> (64 - table[i])) & 1;
        output |= (uint64_t)bit << (n - 1 - i);
    }
    return output;
}
//패리티 제거
uint64_t parity(uint64_t key) {
    uint64_t result = 0;
    for (int i = 0; i < 56; i++) {
        result |= ((key >> (64 - PC[i])) & 1) << (55 - i);
    }
    return result;
}

uint64_t compression_permutate(uint64_t key56) {
    uint64_t result = 0;
    for (int i = 0; i < 48; i++) {
        result |= ((key56 >> (56 - CP[i])) & 1) << (47 - i);
    }
    return result;
} 

//shift 수행
uint32_t left_rotate(uint32_t value, int shifts) {
    return ((value << shifts) | (value >> (28 - shifts))) & 0x0FFFFFFF;
}

void generate_subkeys(uint64_t initial_key, uint64_t subkeys[16]) {
   
    uint64_t key56 = parity(initial_key);

    // 분할
    uint32_t C = (key56 >> 28) & 0x0FFFFFFF;
    uint32_t D = key56 & 0x0FFFFFFF;

    // 16개의 서브키 생성
    for (int i = 0; i < 16; i++) {
       
        C = left_rotate(C, BS[i]);
        D = left_rotate(D, BS[i]);

        //  28 + 28 => 56 
        uint64_t combined = ((uint64_t)C << 28) | D;

        // Compression 적용하여 48비트 서브키 생성
        subkeys[i] = compression_permutate(combined);
    }
}

uint64_t expansion_pbox(uint32_t right) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        
        int bit_position = EP[i] - 1;
        uint64_t bit = (right >> bit_position) & 1; 
        expanded |= (bit << (47 - i)); // 해당 위치에 비트를 설정
    }

    return expanded;
 }

uint32_t sbox(uint64_t input) {
    uint32_t result = 0; 
    for (int i = 0; i < 8; i++) {
        
        uint8_t block = (input >> (42 - (i * 6))) & 0x3F;

        
        int row = ((block & 0x20) >> 4) | (block & 0x01);
        int col = (block >> 1) & 0x0F; 

        // S-Box 결과 가져오기
        uint8_t sbox_value = S_BOX_TABLE[i][row][col];

        // 결과를 합치기
        result |= (sbox_value << (28 - (i * 4)));
    }

    return result;
}
uint32_t straight_pbox(uint32_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 32; i++) {
        int bit_position = SP[i] - 1; 
        uint32_t bit = (input >> bit_position) & 1;
        output |= (bit << (31 - i)); 
    }

    return output;
}
uint32_t des_function(uint32_t right, uint64_t subkey) {
    //32 bit -> 48 bit Expansion P-Box
    uint64_t expanded = expansion_pbox(right);
    /*printf("expanded : %016llX\n", expanded);*/
    //xor with subkey(48bit)
    uint64_t xor_result = expanded ^ subkey;
    /*printf("after xor : %012llX\n", xor_result);*/
    //S-Box 48bit -> 32bit
    uint32_t sbox_output = sbox(xor_result);
    /*printf("after sbox : %08llX\n", sbox_output);*/
    //Straight P-Box
    uint32_t result = straight_pbox(sbox_output);
    return result;

}

//  flag 0 encryption, flag -1 decryption
uint64_t round(uint64_t binary,uint64_t subkey,int count,int flag) {
    // 32bit로 수정.
    uint64_t result = 0;
    //left
    uint32_t L = (binary >> 32) & 0xFFFFFFFF;
    //right
    uint32_t R = binary & 0xFFFFFFFF;
    
    //Mixer
     //des_function
    uint32_t new_R = L ^ des_function(R, subkey);
    uint32_t new_L = R;

    //swapper
    if (flag == 0) {
        //Swapper count == 15의 경우 Swapp진행X
        if (count == 15) {
            result = ((uint64_t)new_R << 32) | new_L; // 스왑 없이 
        }
        else {
            result = ((uint64_t)new_L << 32) | new_R; // 스왑진행
        }
    }

    if (flag == -1) {
        //Swapper count == 0의 경우 Swapp진행X
        if (count == 0) {
            result = ((uint64_t)new_R << 32) | new_L; // 스왑 없이 
        }
        else {
            result = ((uint64_t)new_L << 32) | new_R; // 스왑진행
        }
    }
    
    return result;
}

 uint64_t des_encryption(uint64_t binary, uint64_t subkeys[16]) {
    //init permutation
    uint64_t output = permutation(binary, IP, 64); //test 완료
    uint64_t result = 0;
    //Round 1~15(16)

    for (int i = 0; i < 16; i++) {
        //재선언 이슈X
       output = round(output, subkeys[i],i,0);
       
       /*printf("round %d : %016llX\n", i+1, output);*/
    }
    result = permutation(output, FP, 64);
    //final permuation
    return result;
}
 uint64_t des_decryption(uint64_t binary, uint64_t subkeys[16]) {
     uint64_t output = permutation(binary, IP, 64); //test 완료
     uint64_t result = 0;
     //subkey 역순사용
     for (int i = 15; i > -1; i--) {
         //재선언 이슈X
         output = round(output, subkeys[i], i, -1);
         
         /*printf("round %d : %016llX\n", i+1, output);*/
     }
     result = permutation(output, FP, 64);
     return result;
 };

 void process_file(const char* input_filename, const char* cy_filename, const char* de_filename) {
     FILE* input_file = fopen(input_filename, "r");
     if (input_file == NULL) {
         perror("파일을 열 수 없습니다.");
         return;
     }

     char plaintext[BLOCK_SIZE + 1] = { 0 }; // 8바이트 block

     uint64_t key = generate_random_64bit(); // 랜덤 키 생성
     uint64_t subkeys[16];
     generate_subkeys(key, subkeys); // 서브키 생성

     printf("\nProcessing file: %s\n", input_filename);
     printf("Random Key: %016llX\n", key);

     FILE* cy_file = fopen(cy_filename, "w");
     FILE* de_file = fopen(de_filename, "w");

     if (cy_file == NULL || de_file == NULL) {
         perror("출력 파일을 열 수 없습니다.");
         fclose(input_file);
         if (cy_file) fclose(cy_file);
         if (de_file) fclose(de_file);
         return;
     }

     while (fgets(plaintext, sizeof(plaintext), input_file)) {
         size_t len = strlen(plaintext);

         // 8바이트 패딩
         if (len < BLOCK_SIZE) {
             memset(plaintext + len, 0, BLOCK_SIZE - len);
         }

         // 텍스트를 64비트로 변환
         uint64_t bit_plaintext = test_text_to_uint64(plaintext);

         // 암호화 및 복호화 수행
         uint64_t cypher = des_encryption(bit_plaintext, subkeys);
         uint64_t decryption = des_decryption(cypher, subkeys);

         char decrypted_text[BLOCK_SIZE + 1] = { 0 };
         uint64_to_text(decryption, decrypted_text);

         // 암호화 및 복호화 결과 저장
         fprintf(cy_file, "%016llX\n", cypher);
         fprintf(de_file, "%s", decrypted_text);

         /*printf("Plaintext: %016llX\n", bit_plaintext);
         printf("Ciphertext: %016llX\n", cypher);
         printf("Decrypted Text: %016llX\n", decryption);*/
     }

     fclose(input_file);
     fclose(cy_file);
     fclose(de_file);
 }

int main() {
    srand((unsigned int)time(NULL));

    const char *input_filename1 = "Plain Text 1.txt";
    const char *input_filename2 = "Plain Text 2.txt";
    const char *cy_filename1 = "Cypher Text 1.txt";
    const char *cy_filename2 = "Cypher Text 2.txt";
    const char *de_filename1 = "Decryp Text 1.txt";
    const char *de_filename2 = "Decryp Text 2.txt";

    //new code
    process_file(input_filename1, cy_filename1, de_filename1);
    process_file(input_filename2, cy_filename2, de_filename2);
	
	return 0;
}