#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BYTE_IN_FRAME (512)
#define LOG 0
using namespace std;

void printBuffer(unsigned char *buffer, int len, int frame, int onlyB64){
    printf("line %d: ", frame);
    if(onlyB64){
        for(int i = 0; i < len; i++){
            if((buffer[i] >= 'A' && buffer[i] <= 'z') || buffer[i] == '-' || buffer[i] == '_'|| buffer[i] == '=' || (buffer[i] >= '0' && buffer[i] <= '9')){
                printf("%c", buffer[i]);

            }
            #if LOG_BULLSHIT 
                if(i == (len/2 -1)){
                    printf(" [====>LHM<====] ");
                }
            #endif
        }
    } else {
        for(int i = 0; i < len; i++){

            if(isprint(int (buffer[i]))){
                printf("%c", buffer[i]);
            } else {
                cout << endl << i << " can't print" << endl;
            }
            #if LOG_BULLSHIT 
                if(i == (len/2 -1)){
                    printf(" [====>LHM<====] ");
                }
            #endif
        }        
    }
    printf("\n");
}

string getExtension(string filename){
    for(int i = filename.length() - 1; i >= 0; i--){
        if(filename[i] == '.'){
            return filename.substr(i+1, filename.length() - i - 1);
        }
    }
    return "[LHM] bug";
}

int checkMatch(unsigned char *ptr, string str){
    for(unsigned int i = 0; i < str.length(); i++){
        if(ptr[i] != str[i]){
            return 0;
        }
    }
    #if LOG
        cout << "step 1: map prefix success" << endl;
        printBuffer(ptr, 172 + str.length(), 0, 1);
    #endif
    return 1;
}

int checkB64Element(unsigned char *buffer, int len){    
    for(int i = 0; i < len; i++){
        if(!(\
            (buffer[i] >= 'A' && buffer[i] <= 'z') || \
            buffer[i] == '-' || \
            buffer[i] == '_'|| \
            buffer[i] == '=' || \
            (buffer[i] >= '0' && buffer[i] <= '9'))\
        ){
            #if LOG
                printf("bug in %d with unsigned char = %c\n", i, buffer[i]);
            #endif
            return 0;
        }
    }
    return 1;
}

string getTitle(string prefix, int keyLength, string suffix, string path2file){
    #if LOG
        cout << "[LHM] get title =====> prefix: " << prefix << " len: " << keyLength << " suffix: " << suffix << endl;
    #endif
    

    FILE *f1 = fopen(path2file.c_str(), "r");
    unsigned char *buffer = (unsigned char*)malloc(2*BYTE_IN_FRAME + 5);
    memset(buffer, 0, 2*BYTE_IN_FRAME);
    int num;
    int readAllbyte = 0;
    while(1){
        memcpy(buffer, buffer + BYTE_IN_FRAME, BYTE_IN_FRAME);
        memset(buffer + BYTE_IN_FRAME, 0, BYTE_IN_FRAME);
        num = fread(buffer + BYTE_IN_FRAME, sizeof(unsigned char), BYTE_IN_FRAME, f1);
        if ( num ) {  /* fread success */
            readAllbyte += num;
            for(int i  = 0; i < BYTE_IN_FRAME; i++){
                if( checkMatch(buffer+i, prefix) &&\
                    checkB64Element(buffer + i + prefix.length(), keyLength) &&\
                    checkMatch(buffer + i + prefix.length() + keyLength, suffix)\
                  ){
                    #if LOG
                        cout << "match data in " << i << endl;
                    #endif
                    string key(buffer + i + prefix.length(), buffer + i + prefix.length() + keyLength);
                    fclose(f1);
                    return key;
                }
            }
        } else {
              /* fread failed */
            #if LOG
                cout << "Read all: " << readAllbyte << "bytes" << endl;
            #endif

            if ( ferror(f1) ){ /* possibility 1 */

                fclose(f1);    
                return "false";
            }
            else if ( feof(f1) ){  /* possibility 2 */
                fclose(f1);
                return "EOF found";
            }
        }
    }
    return "hello world";

}
string getTitleNoneTool(string fileName){
    string ext = getExtension(fileName);
    #if LOG
        cout << "[LHM] get extensions =====> ext = " << ext << endl;
    #endif

    if(ext == "png"){
//        return getTitle("Title", 171, "=", fileName) + "=";
        return "false";
    } 
    return getTitle("<rdf:li xml:lang='x-default'>", 172, "</rdf:li>", fileName);

}