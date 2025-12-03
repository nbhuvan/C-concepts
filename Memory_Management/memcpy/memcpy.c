/*
memcpy is a standard function under string.h

void* memcpy(void*dest,const void*src, size_t count);

implicityly creates objects at dest
copies count characters from src into object pointed to by dest

returns dest

*/
#include<stdio.h>
#include "malloc.c"


void* memcpy(void*dest, const void*src, size_t count){
    // dest = 
    char* x = (char*)malloc(count);
    if(x==(char*)-1){
        return NULL;
    }
    char* temp = (char*)src;
    for(int i=0;i<count;i++){
        x[i] = temp[i];
    }
    dest = (void*)x;
    return dest;
}


int main(){
    char c[] = "abcde";
    char* des;
    int x = 4;
    des = (char*)memcpy(des,c,sizeof(char)*x);

    for(int i=0;i<x;i++){
        printf("%c\n",des[i]);
    }
    free(des);

    return 0;

}
