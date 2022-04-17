//sl-4

#include<stdio.h>
#include<stdlib.h>

typedef struct heap_data
{
    int block_id;
    int process_id;
    int data_size;
    struct heap_data *prev;
    struct heap_data *next;
}Heap_block;

typedef struct free_heap
{
    int block_id;
    int flag;
    int free_memory;
    struct free_heap *prev;
    struct free_heap *next;
}free_block;

void create_heap(int size,free_block **fptr)
{
    free_block *f=(free_block*)malloc(sizeof(free_block));
    if(f!=NULL&&size!=0)
    {
        f->prev=NULL;
        f->next=NULL;
        f->free_memory=size;
        f->flag=1;
        if(*fptr==NULL)
        {
            f->block_id=0;
            *fptr=f;
        }
        else
		{
            f->block_id=(*fptr)->block_id+1;
            (*fptr)->next=f;
            f->prev=*fptr;
            *fptr=f;
        }
    }
}
void allocate(int i,int size,free_block *free_ptr,Heap_block **h1,Heap_block **h2)
{
    int flag1=1;
    if(size)
    {
        while(free_ptr!=NULL&&flag1)
        {
            if(free_ptr->free_memory>=size)
            {
                Heap_block *h=(Heap_block*)malloc(sizeof(Heap_block));
                if(h!=NULL)
                {
                    h->block_id=free_ptr->block_id;
                    h->data_size=size;
                    free_ptr->free_memory=free_ptr->free_memory-size;
                    h->prev=NULL;
                    h->next=NULL;
                    h->process_id=i;
                    if(!free_ptr->free_memory)
                    {
                        free_ptr->flag=0;
                    }
                    if(*h1==NULL)
                    {
                        *h1=h;
                        *h2=h;
                    }
                    else
                    {
                       (*h1)->next=h;
                       h->prev=*h1;
                       *h1=h;
                    }
                    flag1=0;
                }
            }
            else
            {
                free_ptr=free_ptr->next;
            }
        }
        if(free_ptr==NULL&&flag1)
        {
            printf("No space in the heap to allocate size:%d \n",size);
        }
    }
}
void free_memory(int id,free_block *free_ptr,Heap_block **p1,Heap_block **p2)
{
    Heap_block *hptr=*p1;
    Heap_block *rptr=NULL,*lptr=NULL;
    int flag1=1,flag2=1;
    while(hptr!=NULL&&flag1)//to iterate memory space
    {
        if(hptr->process_id==id)
        {
            while(free_ptr!=NULL&&flag2)//for iterating the space allocated to the user
            {
                if(hptr->block_id==free_ptr->block_id)
                {
                    free_ptr->flag=1;
                    free_ptr->free_memory=free_ptr->free_memory+hptr->data_size;
                    flag2=0;
                }
                else{
                    free_ptr=free_ptr->next;
                }
            }
            if(hptr->prev==NULL)//first node to be deleted
            {
                *p1=(*p1)->next;
                if(hptr->next!=NULL)
                {
                    (*p1)->prev=NULL;
                }
            }
            if(hptr->next==NULL)//last node to be deleted
            {
                *p2=(*p2)->prev;
                if(hptr->prev!=NULL)
                {
                    (*p2)->next=NULL;
                }
            }
            else
            {
                rptr=hptr->next;
                rptr->prev=hptr->prev;
                if(hptr->prev!=NULL)
                {
                    lptr=hptr->prev;
                    lptr->next=rptr;
                }
            }
            free(hptr);
            flag1=0;
        }
        else
        {
            hptr=hptr->next;
        }
    }
}
void print_allocated(Heap_block *hptr)
{
	printf("\nMemory allocated:");
	printf("\nBlock_id      process_id    data_size\n");
	while(hptr!=NULL)
	{
		printf(" %d  		%d	    %d\n",hptr->block_id,hptr->process_id,hptr->data_size);
        hptr=hptr->next;
	}
}
void print_memory(free_block *fptr)
{
	printf("\nMemory available in heap after deletion:\n");
    printf("\n Block_id     free_memory\n");
    while(fptr!=NULL)
    {
        printf(" %d                 %d\n",fptr->block_id,fptr->free_memory);
        fptr=fptr->next;
    }
}
int main()
{
    int i,n1,n2;
    printf("Enter no of blocks available for allocating memory:");
    scanf("%d",&n1);
    int B[n1];
    for(i=0;i<n1;i++)
    {
        scanf("%d",&B[i]);
    }
    printf("Enter process size:");
    scanf("%d",&n2);
    int A[n2];
    for(i=0;i<n2;i++)
    {
        scanf("%d",&A[i]);
    }
    free_block *x,*y;
    x=NULL;
    y=NULL;
    for(i=0;i<n1;i++)
    {
        create_heap(B[i],&x);
        if(!i)
        {
		  y=x;
        }
    }
    Heap_block *p,*q;
    p=NULL;
    q=NULL;
    for(i=0;i<n2;i++)
    {
        allocate(i,A[i],y,&p,&q);
    }
    
    print_allocated(q); 
    int id;
    
   while(1)
    {
    	int ch;
    	printf("enter 0 to free :");
    	scanf("%d",&ch);
    
      if(ch==0 &&q!=NULL)
     {
    printf("\nEnter process_id to free:");
    scanf("%d",&id);
    free_memory(id,y,&q,&p);
    print_allocated(q); 
    print_memory(y);
	 }
	 else{
	 	break;
	  }
    }
    return 0;
}
