#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sodium.h>

FILE *input,*output;
DIR *dir;
size_t file_size;
int round;
unsigned char txt_data[300];
char dir_name[100];
unsigned char key[100];
unsigned char final_key[40];
unsigned char sbox[256] {
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
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
int search_file() {
    char final_path[100];
    char file_name[30];
    size_t bytes_read;

    getcwd(dir_name, sizeof(dir_name));
    printf("*** If you spell it wrong it will not work *** \n");
    printf("What is the name of the file you would like to encrypt: \n");
    fgets(file_name,sizeof(file_name),stdin);
    file_name[strcspn(file_name, "\n")] = 0;

    snprintf(final_path,sizeof(final_path),"%s/%s",dir_name,file_name);

    input = fopen(final_path,"r");

    bytes_read = fread(txt_data, 1, sizeof(txt_data) - 1, input);
    txt_data[bytes_read] = '\0';
    file_size = bytes_read;

    if (input == NULL) {
        printf("File not found\n");
        return 1;
    }

    if (bytes_read == 0) {
        if (feof(input)) {
            printf("Finished reading file \n");
        }else {
            printf("Failed to read file \n");
        }
    }
    printf("%s\n",txt_data);
    fclose(input);
    return 0;
}
void xor(char *msg,char *key, size_t msg_len){
    size_t key_index = 0;
    size_t key_len = strlen((char*) key);
    for (int i = 0; i <msg_len;i++ ) {
        msg[i] ^= key[key_index % key_len];
        key_index++;
    }
}
unsigned char gf_multiply(unsigned char a, unsigned char b) {
    int result = 0;
    int temp_a = a;
    int high_bit_set = 0x00;
    for (int i = 0; i < 8; i++) {
        if (b & (1 << i)) {
            result = result ^ temp_a;
        }

        high_bit_set = temp_a & 0x80; //checks if high bit is a 1
        temp_a = temp_a << 1;

        if (high_bit_set != 0) {
            temp_a = temp_a ^ 0x1B;
        }
    }
    return result & 0xFF; //makes sure it stays within GF field
}
void sbox_swap(size_t length) {
    for (int i = 0; i < length; i++) {
        txt_data[i] = sbox[txt_data[i]];
    }
}
void shift_row(size_t length) {
    int row = 1;
    int column = 0;
    unsigned char grid[4][4];

    for (int i = 0; i < 16; i++) {
        grid[i/4][i%4] = txt_data[i];
    }

    for (row = 1;row < 4;row++) {
        unsigned char temp[4];

        for (column = 0;column < 4;column++) {
            temp[column] = grid[row][(column + row) % 4];
        }

        for (int col = 0; col < 4; col++) {
                grid[row][col] = temp[col];
        }
    }

    for (int i = 0; i < 16; i++) {
        txt_data[i] = grid[i/4][i%4];
    }
}
void mix_column(unsigned char *state,size_t length) {
    unsigned char temp_column[];
    unsigned char new_column[];
    //copy state in its specific column array to temp
    for (int column = 0; column < 4; column++) {
        //have shifting logic work for rows for stuff below mostly in the copying part of logic
        // 2 3 1 1
        // 1 2 3 1
        // 1 1 2 3
        // 3 1 1 2
        //fix this whole things formatting
        new_column[0] =
            gf_multiply(0x02 ,temp_column[0]);
            xor(gf_multiply(0x03,temp_column[1]), ,length);
            xor(temp_column[2], ,length);
            xor(temp_column[3], ,length);
        new_column[1] =
           temp_column[0];
           xor(gf_multiply(0x02,temp_column[1]), ,length);
           xor(gf_multiply(0x03,temp_column[2]), ,length);
           xor(temp_column[3], ,length);
        new_column[2] =
            temp_column[0];
            xor(temp_column[1], ,length);
            xor(gf_multiply(0x02,temp_column[2]), ,length);
            xor(gf_multiply(0x03,temp_column[3]), ,length);
        new_column[3] =
            gf_multiply(0x03 ,temp_column[0]);
            xor(temp_column[1], ,length);
            xor(temp_column[2], ,length);
            xor(gf_multiply(0x02,temp_column[3]), ,length);
        //copy new column back to original state
        state[] = new_column[];
    }
}
void key_derivation(size_t key_len) {
    unsigned char salt[32];
    size_t salt_size = 32 - key_len; //forces key to be 32 bytes
    randombytes_buf(salt,salt_size);

    memcpy(final_key,key, key_len);
    memcpy(final_key+ key_len,salt,salt_size);
}
void add_key() {
    unsigned char key_grid[4][4];
    unsigned char txt_grid[4][4];
    int row = 0;
    int column = 0;

    for (int i = 0; i < 16; i++) { //turn key into a 4x4 grid
        key_grid[i/4][i%4] = key[i];
    }

    for (int i = 0; i < 16; i++) { //turn txt_data into a 4x4 grid
        txt_grid[i/4][i%4] = txt_data[i];
    }

    for (row = 0;row < 4;row++) { //something in this double for loop is wrong cause of the  k1 rule
        for (column = 0;column < 4;column++) {
            xor(txt_grid[row][column],key_grid[row][column],sizeof(txt_grid));
        }
    }
    //that is only first round tho
    //or is it just done over and over and over again the needed times
    /*
     w0  w4  w8   w12
     w1  w5  w9   w13
     w2  w6  w10  w14
     w3  w7  w11  w15
     k1  k2  k3   k4   = 1st round
    /*
     xor txt_grid with key_grid
     60 words needed to meet requirement
     15 keys
     */
}
int main(void) {
    char output_file[30];
    char output_path[300];

    search_file();

    printf("What is the key you would like to use: \n");
    fgets(key,sizeof(key),stdin);
    key[strcspn(key, "\n")] = 0;
    printf("%s\n",key);
    size_t key_len = strlen(key);

    printf("*** make it a .enc *** \n");
    printf("What is the name of the exported file: \n");
    fgets(output_file,sizeof(output_file),stdin);
    output_file[strcspn(output_file, "\n")] = 0;
    sprintf(output_path,"%s/%s",dir_name,output_file);

    output = fopen(output_path,"a");

    key_derivation(key_len);
    add_key();

    for (round = 1; round < 14; round++) {
        xor(txt_data,key,file_size);
        sbox_swap(file_size);
        shift_row(file_size);
        mix_column(txt_data,file_size);
        add_key();
    }

    if (round == 14) {
        xor(txt_data,key,file_size);
        sbox_swap(file_size);
        shift_row(file_size);
        add_key();
    }

    for () {
        
        //add a way to print to file
    }

fclose(output);

    return 0;
}
