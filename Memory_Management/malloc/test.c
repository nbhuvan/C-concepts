/*
memcpy is a standard function under string.h

void* memcpy(void*dest,const void*src, size_t count);

implicityly creates objects at dest
copies count characters from src into object pointed to by dest

returns dest

*/
#include<stdio.h>
#include "malloc.c"


int main(){
    t_block b;

    char* x;
    x = (char*)malloc(sizeof(char) * 4);
    if(x == (char*)-1){
        printf("Fail\n");
        return 0;
    }
    b = get_block(x);

    printf("size meta data %ld\n",b->size);

    for(int i=0;i<4;i++){
        x[i] = i;
    }
    for(int i=0;i<4;i++){
        printf("%d\n",x[i]);
    }
    free(x);
    x = NULL;
    // b = (struct s_block*)(x-BLOCKSIZE);

    // printf("size meta data %d\n",(int)b->size);
    return 0;

}
