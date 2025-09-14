#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


FILE *file;
DIR *dir;
int round;


int search_file() {
    char final_path[100];
    char file_name[30];
    char dir_name[100];
    size_t bytes_read;

    getcwd(dir_name, sizeof(dir_name));
    printf("*** If you spell it wrong it will not work *** \n");
    printf("What is the name of the file you would like to encrypt: \n");
    fgets(file_name,sizeof(file_name),stdin);
    file_name[strcspn(file_name, "\n")] = 0;

    snprintf(final_path,sizeof(final_path),"%s/%s",dir_name,file_name);

    file = fopen(final_path,"r");
    char txt_data[300];
    bytes_read = fread(txt_data, 1, sizeof(txt_data) - 1, file);
    txt_data[bytes_read] = '\0';

    if (file == NULL) {
        printf("File not found\n");
        return 1;
    }

    if (bytes_read == 0) {
        if (feof(file)) {
            printf("Finished reading file \n");
        }else {
            printf("Failed to read file \n");
        }
    }
    printf("%s\n",txt_data);
}
void xor(char *msg, char *key){
    size_t key_index = 0;
    size_t msg_len = strlen((char*) msg);
    size_t key_len = strlen((char*) key);
    for (int i = 0; i <msg_len;i++ ) {
        msg[i] ^= key[key_index % key_len];
        key_index++;
    }
}

int main(void) {
   search_file();
  // file = fopen("",'r');

    for (round = 1; round < 14; round++) {
       // xor();
        //add other steps
    }
    if (round == 14) {
        //xor();
        //add other steps
        //don't mix colums here
    }
    /*
     14 rounds so a loop
    all the aes math logic
    */
    //fwrite();
    fflush(file);

    return 0;
}