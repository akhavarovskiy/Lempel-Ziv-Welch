#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdbool.h>

/*==============================================================================================================
    COURSE:        CS 360 Spring 2018
    ASSIGNMENT:    1
    AUTHORS:       "Aleksandr Khavarovskiy" and Dick Lang
  ==============================================================================================================
  Sequence:
  =========
     Module representing character strings of arbritrary length, where
     the characters can be any 8-bit integer value, including zero.
 
     The module supports creating and deleting sequences, writing them
     to some destination, copying and appending a character and comparing
     them for equality.
 
     Consider this interface to be a starting point or a suggestion for
     your code design. Modify it as needed, but obey good object-oriented
     design principles.
  ==============================================================================================================
    The smallest division of a sequence used for elements in a dynamic array.
  ==============================================================================================================*/
#ifndef SEQUENCE_DATA_TYPE
#define SEQUENCE_DATA_TYPE       unsigned char
#endif

#ifndef SEQUENCE_DATA_TYPE_SIZE
#define SEQUENCE_DATA_TYPE_SIZE  sizeof( SEQUENCE_DATA_TYPE )
#endif
/* ==============================================================================================================
    To prevent unnessasery calls to realloc with every append we will allocate a chunck of memory.
  ==============================================================================================================*/
#ifndef SEQUENCE_DATA_CHUNK
#define SEQUENCE_DATA_CHUNK    64    
#endif                  

#ifndef SEQUENCE_DATA_CHUNK_SIZE                                              
#define SEQUENCE_DATA_CHUNK_SIZE SEQUENCE_DATA_CHUNK * SEQUENCE_DATA_TYPE_SIZE  // Size in KB
#endif

/* Iteratetor pointer */
typedef SEQUENCE_DATA_TYPE* iterator_sequence;

typedef struct _sequence {
  /*==============================================================================================================
      Sequence Functions 
    ==============================================================================================================*/
    void          (*dctor)  ( struct _sequence* this_ptr );                          /* Destructor of object */
    unsigned long (*Length) ( struct _sequence* this_ptr );                          /* Sequence Length */
    unsigned long (*Size)   ( struct _sequence* this_ptr );                          /* Size of sequence */
    void          (*Resize) ( struct _sequence* this_ptr, unsigned long size );      /* Resize the dynamic array */
    void          (*Append) ( struct _sequence* this_ptr, SEQUENCE_DATA_TYPE data ); /* Append an element to a sequence */    
    bool          (*Compare)( struct _sequence* this_ptr, struct _sequence* s );     /* Compare */
    void          (*Combine)( struct _sequence* this_ptr, struct _sequence* s );     /* Combine two sequences into one */
    void          (*Output) ( struct _sequence* this_ptr, int (*writeFunc)(unsigned char c, void* context), void* context);

  /*==============================================================================================================
     Sequence Data 
    ==============================================================================================================*/
    SEQUENCE_DATA_TYPE *m_begin;                                                /* Pointer to start of array memory */
    SEQUENCE_DATA_TYPE *m_end;                                                  /* Pointer to end of array memory */ 
    unsigned long       m_capacity;                                             /* Capacity of allocated memory in bytes */
} 
Sequence;

/*==============================================================================================================
  Sequence_Create
  ==============================================================================================================     
   Create and return a newSequence object with its first and only character equal
   to firstByte. It can be noted that there is no need to support a Sequence that
   has zero characters.  All Sequences used by LZW have at least one character. */
     
Sequence* Sequence_Create       ( void );     
Sequence* Sequence_Create_Copy  ( Sequence* s );     
Sequence* Sequence_Create_Insert( SEQUENCE_DATA_TYPE firstElement );

/*==============================================================================================================
  Dump Sequence into a file.
  ============================================================================================================== */
void OutputSequence(Sequence* this_ptr, int (*writeFunc)(unsigned char c, void* context), void* context);

#endif
