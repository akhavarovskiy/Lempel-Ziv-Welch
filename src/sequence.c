#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 
#include <sequence.h>

/*==============================================================
    COURSE:        CS 360 Spring 2018
    ASSIGNMENT:    1
    AUTHOR:        Aleksandr Khavarovskiy
  ==============================================================*/
/*==============================================================
  Sequnce Destructor
  ==============================================================*/
void Sequence_Destroy( Sequence* this ) {
    free( this->m_begin );
    free( this );
}

/*==============================================================
    Number of elements in the sequence
  ==============================================================*/
unsigned long Sequence_Length( Sequence * this )
{
    /* We divide that by the size of our smallest element to get the 
      amout of elements in our structure.*/
    return (unsigned)((SEQUENCE_DATA_TYPE*)this->m_end - (SEQUENCE_DATA_TYPE*)this->m_begin);
}

/*==============================================================
   Size of sequence
  ==============================================================*/
unsigned long Sequence_Size( Sequence * this )
{
    /* subtracting the end pointer from the start pointer will
       yeild the amout of bytes held in the array. */
    return (unsigned)((unsigned char*)this->m_end - (unsigned char*)this->m_begin);
}

/*==============================================================
    Resize the dynamic array 
  ==============================================================*/
void Sequence_Resize( Sequence* this, unsigned long size /*size in bytes*/ )
{
    /* The current sequence length, stored locally to prevent redundant 
       calculations.*/        
    int lv_size        = this->Size( this );

    /* We are shringing the array to a smaller size then the one required to hold our elements.*/    
    if( lv_size < size ) 
        lv_size = size;
    /* We can chose to hault if this even happens*/
    //assert((lv_size > size) && "[ Error ] Resizing sequence caused a loss of data!\n" );

    /* Set the pointer to point to the new region of memory */
    this->m_begin = realloc( this->m_begin, size );
    this->m_end   = &this->m_begin[ lv_size ];

    /* Change the size indicator then reallocate the memmory */
    this->m_capacity = size;
}                   

/*==============================================================
  Append and element to the sequence.
  ==============================================================*/
void Sequence_Append( Sequence * this, SEQUENCE_DATA_TYPE data ) 
{
    /* The current sequence length, stored locally to prevent redundant 
       calculations.*/              
    int lv_length        = this->Length( this ),
        lv_size          = this->Size( this );

    /* If the sequence length is longer than the avalable allocated 
       memory within the structure, reallocate the data.*/     
    if( lv_length >= this->m_capacity ) {
        this->Resize( this, lv_size + SEQUENCE_DATA_CHUNK_SIZE );
    }
    /* Set the our data */
    *this->m_end = data;    
    /* Move the pointer over one space */    
    this->m_end++;
}

/*==============================================================
  Compare two sequences.
  ==============================================================*/
bool Sequence_Compare( Sequence* this, Sequence* s )
{   
    /* if the lengths are not the same, then the two must be different. */
    if( this->Length( this ) !=  s->Length( s ) ) 
        return false;
        
    /* Create two iterators to traverse the sequence */
    iterator_sequence iter   = this->m_begin;
    iterator_sequence iter_s =    s->m_begin;
        
    /* Compare each element of the array */
    for( ;iter != this->m_end && iter_s != s->m_end; iter++, iter_s++ ) {
        if( *iter != *iter_s  )
            return false;
    }
    return true;
}

/*==============================================================
   Combine the two 
  ==============================================================
   Combine two sequences into one */
void Sequence_Combine( Sequence*  this, Sequence* s ) 
{
    int lv_len = this->Size( this ) + s->Size( s );
    
    /* If the this contanier dosent have enough space we will resize */
    if(lv_len > this->m_capacity) 
        this->Resize( this, ((unsigned long)(lv_len/SEQUENCE_DATA_CHUNK_SIZE) + 1) * SEQUENCE_DATA_CHUNK_SIZE );
    
    iterator_sequence iter = s->m_begin;
    for( ;iter != s->m_end; this->m_end++, iter++ )
        *this->m_end = *iter;
}

/*==============================================================
  Sequnce Output
  ==============================================================
   - Writes the sequence into a file */
void Sequence_Output(Sequence* this_ptr, int (*writeFunc)(unsigned char c, void* context), void* context) 
{
    for( iterator_sequence iter = this_ptr->m_begin; iter != this_ptr->m_end; iter++ ) {
        writeFunc( *iter, context );
    }
}
/*==============================================================
  Sequnce Constructor
  ==============================================================
   - Creates an empty sequence object. */
Sequence* Sequence_Create( void )
{
    /* Sequence Structure Allocation */ 
    Sequence* r_sequence = (Sequence*)malloc( sizeof( Sequence ) );
    
    /* Allocating the first chunk of the sequnce.*/
    r_sequence->m_begin             = (SEQUENCE_DATA_TYPE*)malloc( SEQUENCE_DATA_CHUNK_SIZE ); 
    r_sequence->m_end               = r_sequence->m_begin;
    r_sequence->m_capacity          = SEQUENCE_DATA_CHUNK_SIZE;
    /*  Setting the function pointers of the struct. */
    r_sequence->dctor               = Sequence_Destroy;
    r_sequence->Length              = Sequence_Length;
    r_sequence->Size                = Sequence_Size;
    r_sequence->Resize              = Sequence_Resize;
    r_sequence->Append              = Sequence_Append;
    r_sequence->Compare             = Sequence_Compare;
    r_sequence->Combine             = Sequence_Combine;
    r_sequence->Output              = Sequence_Output;
    
    return r_sequence;
}

/*==============================================================
  Sequnce Constructor
  ==============================================================
   - Creates an sequence object with one element.  */
Sequence* Sequence_Create_Copy( Sequence* s )
{
    /* Sequence Structure Allocation */ 
    Sequence* r = Sequence_Create();

    /* Resize the newly created sequence to match the new one.*/
    if( r->m_capacity < s->m_capacity )
        r->Resize( r, s->m_capacity * SEQUENCE_DATA_TYPE_SIZE );

    iterator_sequence iter   = s->m_begin;
    iterator_sequence iter_r = r->m_begin;

    for( ;iter != s->m_end; iter++, iter_r++ ) 
        *iter_r = *iter;
    
    r->m_end = iter_r;

    return r;
}

/*==============================================================
  Sequnce Constructor
  ==============================================================
   - Creates an sequence object with one element.  */
Sequence* Sequence_Create_Insert( SEQUENCE_DATA_TYPE firstElement )
{
    Sequence* r_sequence = Sequence_Create();

    r_sequence->Append( r_sequence, firstElement );

    return r_sequence;
}

