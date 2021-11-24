
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int search(char frames[80][5], char frame_data[80][100] ,char frame_name[5]) {
    int k = 0;
    while(k < 80 && strcmp(frames[k], frame_name))
        k++;
    
    if(frame_data[k][0] != '\0') {
        return k;
    }
    return -1;
}

void set(FILE *file, FILE *fileout, char frames[80][5], char frame_data[80][100] , int tag_data_byte[80], char frame_name[5], char value[100],int byte) {
    int key = search(frames, frame_data, frame_name);
    
    int k = 0;
    char lc[2];
    while(value[k] != '\0') {
        k++;
    }
    
    if(key != -1) {
        int z = 0;
        while(frame_data[key][z] != '\0') {
            z++;
        }
        fseek(file, tag_data_byte[key], 0);
        fwrite(value, sizeof(char), k, file);
        fseek(fileout, tag_data_byte[key] + z, 0);
        while(feof(fileout) == 0) {
            lc[0] = fgetc(fileout);
            fwrite(lc, sizeof(char), 1, file);
        }
    }
}

int main(int argc, const char * argv[]) {
    
    char file_name[100];
    int z = 0;
    int v = 0;
    
    while(argv[1][z] != '=')
        z++;
    z++;
    
    while(argv[1][z] != '\0') {
        file_name[v] = argv[1][z];
        z++;
        v++;
    }
    
    file_name[v] = '\0';
    
    printf("<><><><><><><><><><><><><><><><><><>\n");
    
    FILE *file, *fileout;
    file = fopen(file_name, "r+b");
    fclose(fopen("/Users/dastardmoron/Documents/Программирование/read.txt", "w"));
    fileout = fopen("/Users/dastardmoron/Documents/Программирование/read.txt", "r+b");
    
    char frames[80][5];
    char frame_data[80][100];
    char c;
    int tag_data_byte[80];
    int index = 0;
    int i = 0;
    int byte = 21;
    
    for(int k = 0; k < 77; k++) {
        for(int z = 0; z < 5; z++)
            frames[k][z] = '\0';
        for(int z = 0; z < 100; z++)
            frame_data[k][z] = '\0';
    }

    fseek(file, 10, 1);
    
    while((c = fgetc(file)) != '\0') {
        frames[0][i] = c;
        i++;
    }
    
    tag_data_byte[0] = 21;
    
    fseek(file, 6, 1);
    c = fgetc(file);
    
    while(c != '\0') {
        i = 0;
        
        while(c != '\0') {
            frame_data[index][i] = c;
            c = fgetc(file);
            i++;
            if(c == '\0' && (c = fgetc(file)) != '\0') {
                frame_data[index][i] = ' ';
                i++;
            }
        }
        
        byte = byte + 7 + i;
        
        fseek(file, 5, 1);
        if((c = fgetc(file)) != '\0') {
            for(int k = 0; k < 4; k++) {
                frames[index + 1][3 - k] = frame_data[index][i - k - 1];
                frame_data[index][i - k - 1] = '\0';
            }
            tag_data_byte[index + 1] = byte;
        }
        index++;
    }
    
    char oper_value[5];
    char operation[7];
    
    if(strcmp(argv[2], "--show") == 0) {
        
        for(int k = 0; k < index; k++) {
            printf("%s %s\n", frames[k], frame_data[k]);
        }
        
    }
    
    else {
        
        for(int d = 0; d < 10; d++) {
            char val = argv[2][d];
            
            if(d > 5)
                oper_value[d - 6] = val;
            
            else
                operation[d] = val;
            
        }
        operation[6] = '\0';
        oper_value[4] = '\0';
    }
    
    if(strcmp(operation, "--get=") == 0) {
        
        int res = search(frames, frame_data, oper_value);
        
        if(res == -1)
            printf("NO DATA FOR %s\n", oper_value);
        
        else
            printf("DATA FOR %s: %s\n", oper_value, frame_data[res]);
    }
    
    if(strcmp(operation, "--set=") == 0) {
        
        char data[100];
        
        for(int d = 8; argv[3][d] != '\0'; d++)
             data[d - 8] = argv[3][d];
            
        fseek(file, 0, 0);
            
        while(feof(file) == 0)
            fprintf(fileout, "%c", fgetc(file));
            
        set(file, fileout, frames, frame_data, tag_data_byte, oper_value, data, byte);
    }
    
    fclose(fileout);
    fclose(file);
    return 0;
}
