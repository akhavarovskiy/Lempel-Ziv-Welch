#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include "sequence.h"
/*
==============================================================================================================
  COURSE:        CS 360 Spring 2018
  ASSIGNMENT:    1
  AUTHORS:       "Aleksandr Khavarovskiy" and Dick Lang
==============================================================================================================
  Dict:
=========
    Module representing a dictionary (hash table) for storing and
    retrieving integer symbol codes using a Sequence as a key.
 
    Dictionaries may be created and deleted.  Sequences can be inserted
    into the dictionary, along with their assigned code.  Sequences can
    be searched for and their code obtained. There is no limit (other
    than available heap memory) to the number of Sequence/code pairs
    that may be stored in a Dictionary.
 
    Consider this interface to be a starting point or a suggestion for
    your code design. Modify it as needed, but obey good object-oriented
    design principles.
==============================================================================================================
  Dictionary elements type
============================================================================================================== */
#ifndef DICTIONARY_DATA_TYPE
#define DICTIONARY_DATA_TYPE Sequence
#endif

#ifndef DICTIONARY_DATA_TYPE_PTR
#define DICTIONARY_DATA_TYPE_PTR DICTIONARY_DATA_TYPE*
#endif

#ifndef DICTIONARY_DATA_TYPE_SIZE
#define DICTIONARY_DATA_TYPE_SIZE sizeof(DICTIONARY_DATA_TYPE)
#endif

#ifndef DICTIONARY_DATA_TYPE_PTR_SIZE
#define DICTIONARY_DATA_TYPE_PTR_SIZE sizeof(DICTIONARY_DATA_TYPE_PTR)
#endif
/*==============================================================================================================*/
/*   Bucket Node Structure                                                                                      */
/*==============================================================================================================*/
typedef struct bucket_node {
    DICTIONARY_DATA_TYPE_PTR m_data;
    unsigned int             m_code;
    struct bucket_node      *m_next; 
}
BucketNode;
/*==============================================================================================================*/
typedef struct bucket_node* iterator_linked_list;
/*==============================================================================================================*/
BucketNode * BucketNode_Create       ( void );
BucketNode * BucketNode_Create_Insert( DICTIONARY_DATA_TYPE_PTR data, BucketNode* next );
/* Destructor */
void         BucketNode_Destroy      ( BucketNode * this_ptr );
/*==============================================================================================================*/
/*  Dictionary                                                                                                  */
/*==============================================================================================================*/
typedef struct _dictionary {
    /* Dictionary Functions */
    void    (*dctor) ( struct _dictionary* this_ptr );                                          /* Destructor on object */
    void    (*Insert)( struct _dictionary* this_ptr, DICTIONARY_DATA_TYPE_PTR ptr, int  code ); /* Insert routine */
    bool    (*Search)( struct _dictionary* this_ptr, DICTIONARY_DATA_TYPE_PTR ptr, int* code );
    /* Dictionary Data */
    BucketNode**          m_data;                                                         /* Hash Elements */
    unsigned int          m_size;                                                         /* Number of elements in hash */
} 
Dict;
/*==============================================================================================================*/
/*   Dictionary Create                                                                                                */
/*==============================================================================================================*/
/* - Create a new Dict with hashSize "buckets" and return a pointer to it                                       */
/*==============================================================================================================*/
Dict* Dict_Create(unsigned long hashSize);

#endif
