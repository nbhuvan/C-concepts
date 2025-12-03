/*
    Allocate memory using malloc, malloc is a Standard Library Fuction which allocates requested memory and returns pointer to that.

    Allocation of memory is done through syscall - sbrk(),mmap()

    sbrk is used for small memory size
    mmap is used for large memeory size

    brk(0) return the place the break at the given addr and return 0 is if success full

    -(Heap Start)########(break)######################
    Break is the limit where heap mapped memory ends



*/

#include <sys/types.h>
#include <unistd.h>

typedef struct s_block *t_block;

struct s_block{
    size_t size; //Meta data of the memory block
    t_block next;
    t_block prev;
    int free;
    // void *ptr;
    // char data[1];
};

#define BLOCKSIZE sizeof(struct s_block)

void *base = NULL;

t_block extend_heap(t_block last,size_t s){
    void* sb;
    t_block b;
    b = sbrk(0);
    sb = sbrk(BLOCKSIZE+s);

    if(sb==(void*)-1){
        return (NULL);
    }

    b->size = s;
    b->next = NULL;
    b->prev = last;
    // b->ptr = b->data;
    if(last){
        last->next = b;
    }
    b->free = 0;
    // base = s;
    return b;
}

void split_block(t_block b,size_t s){
    t_block new;
    new = (t_block)((char*)(b+1) + s);
    new->size = b->size -(BLOCKSIZE+s);
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    // new->ptr = new->data;

    b->size = s;
    b->next = new;
    if(new->next){
        new->next->prev = new;
    }
}


t_block find_block(t_block *last,size_t size){
    t_block b = base;
    while(b && !(b->free && (b->size >= size))){
        *last = b;
        b = b->next;
    }
    return b;
}

#define align4(x) (((((x)-1)>>2)<<2)+4)

void *malloc(size_t size){


    t_block b,last;
    size_t s;
    s = align4(size);
    // printf("s = %zu",s);
    if(base){
        last = base;
        b = find_block(&last,s);
        if(b){
            if((b->size-s)>=(BLOCKSIZE+4)){
                split_block(b,s);
            }
            b->free = 0;
        }
        else{
            b = extend_heap(last,s);
            if(!b) return NULL;
        }
    }
    else{
        b = extend_heap(NULL,s);
        if(!b) return NULL;

        base = b;
    }

    return (b+1);
    // b = sbrk(0);
    // sbrk(sizeof(struct s_block)+size);
    // b->size = size;

    // if(sbrk(size)==(void*)-1){
    //     return NULL;
    // }
    // b->free = 0;
    // b->next = NULL;
    // return b;
}

t_block get_block(void *p){
    t_block temp = (t_block)p-1;
    // char *tmp;
    // tmp = p;
    return temp;
}

int valid_addr(void *p){
    if(base){
        if( p>base && p<sbrk(0)){
            return (1);
        }
    }
    return (0);
}

t_block fusion(t_block b){
    if(b->next && b->next->free){
        b->size += BLOCKSIZE + b->next->size;
        b->next = b->next->next;
        if(b->next){
            b->next->prev = b;
        }
        return b;
    }
}

void free(void *p){
    t_block b;
    if(valid_addr(p)){
        b = get_block(p);
        b->free = 1;
        if(b->prev && b->prev->free){
            b = fusion(b->prev);
        }
        
        if(b->next){
            fusion(b);
        }
        else{
            if(b->prev){
                b->prev->next = NULL;
            }
            else{
                base = NULL;
            }
            brk(b);
        }
    }

}


