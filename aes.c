#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


FILE *input,*output;
DIR *dir;
int round;
char txt_data[300];
char dir_name[100];

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
    char output_file[30];
    char output_path[300];
    search_file();
    char key[100];
    printf("What is the key you would like to use: \n");
    fgets(key,sizeof(key),stdin);
    key[strcspn(key, "\n")] = 0;

    printf("*** make it a .enc *** \n");
    printf("What is the name of the exported file: \n");
    fgets(output_file,sizeof(output_file),stdin);
    output_file[strcspn(output_file, "\n")] = 0;
    sprintf(output_path,"%s/%s",dir_name,output_file);

    output = fopen(output_path,"w");

    for (round = 1; round < 14; round++) {
        xor(txt_data,key);
        //add other steps
    }
    if (round == 14) {
        xor(txt_data,key);
        //add other steps
        //don't mix colums here
    }
    /*
     14 rounds so a loop
    all the aes math logic
    */
    //fwrite();


    return 0;
}