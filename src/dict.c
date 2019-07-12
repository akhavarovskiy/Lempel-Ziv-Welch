#include <stdlib.h>
#include <dict.h>
#include <inttypes.h>

/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Alex Khavarovskiy
 */

/*==============================================================================================================
    Our core hashing function
  ==============================================================================================================*/
unsigned long Bernstein_Hash( DICTIONARY_DATA_TYPE_PTR data ) 
{
    unsigned long h = 5831;

    /* Bernstein Hash */
    for( iterator_sequence iter = data->m_begin; iter != data->m_end; iter++ )
        h = ((h << 5) + h) ^ *iter;
    
    return h;
}

/*==============================================================================================================
    BucketNode Creation Routines
  ==============================================================================================================*/
BucketNode * BucketNode_Create( void ) 
{
    return BucketNode_Create_Insert( NULL, NULL );
}
/* Alternative */
BucketNode* BucketNode_Create_Insert( DICTIONARY_DATA_TYPE_PTR data, BucketNode* next )
{
    BucketNode *lvRet = malloc( sizeof( BucketNode ) );

    /* Clear the Data */
    lvRet->m_data = data;
    lvRet->m_next = next;

    return lvRet;   
}
/* Destructor */
void BucketNode_Destroy( BucketNode * this )
{
    /* Recursive end condition */
    if( this == NULL )
        return;
    
    /* Deallocate the Sequence */
    this->m_data->dctor( this->m_data );

    /* Go to the Next Element */
    BucketNode_Destroy( this->m_next );

    /* Free the node data. */
    free( this );
    /* Cleare the Pointer */
    this = NULL;
}

/*==============================================================================================================
    Dict destroy Function
  ==============================================================================================================*/
void Dict_Destroy( Dict * this )
{
    BucketNode * node = NULL;

    for( unsigned int i = 0; i < this->m_size; i++ ) 
    {
        /* Get reference to the data */
        node = this->m_data[i];

        /* Is node a null, go to next iteration of the loop */
        if( node == NULL )
            continue;

        /* Deallocates All the nodes*/
        BucketNode_Destroy( node );
    }
    free( this->m_data );
    free( this );
}

/*==============================================================================================================
    Dict Insert
  ==============================================================================================================*/
void Dict_Insert( Dict * this, DICTIONARY_DATA_TYPE_PTR data, int code )
{   
    int index = Bernstein_Hash( data ) % this->m_size;
    
    ///int index = Bernstein_Hash( data );
    /* Is the current bucket empty */
    if( this->m_data[ index ] == NULL) {
        this->m_data[ index ] = BucketNode_Create_Insert( data, NULL );
        this->m_data[ index ]->m_code = code;
        return;
    }
    /* Otherwise we crate */    
    this->m_data[ index ] = BucketNode_Create_Insert( data, this->m_data[ index ] );
    this->m_data[ index ]->m_code = code;
    
}

/*==============================================================================================================
    Dict Search, Is the Sequence Contained within the hash.
  ==============================================================================================================*/
bool Dict_Search( struct _dictionary* this, DICTIONARY_DATA_TYPE_PTR ptr, int* code )
{
    /*If two sequences are identical they must have the same hash.*/    
    int          lvIndex = Bernstein_Hash( ptr ) % this->m_size;
    BucketNode * lvNode  = this->m_data[ lvIndex ];

    /* If there is no allocated linked list at the hash index not found*/
    if( lvNode == NULL ) {
        (*code) = 0;
        return false;
    }
    for( iterator_linked_list iter = lvNode; iter != NULL; iter = iter->m_next ) 
    {
        if ( iter->m_data->Compare( iter->m_data, ptr ) ) {
            (*code) = iter->m_code;
            return true;
        }
    }
    *code = 0;
    return false;
}
/*==============================================================================================================
    Dict Create Function
  ==============================================================================================================*/
Dict* Dict_Create(unsigned long hashSize)
{
    Dict* r_dict   = malloc( sizeof( Dict ) );

    /* Pointer size may vary with x86 & x64 architecture so we fetch pointer size wile allocating. */
    r_dict->m_data = calloc(sizeof( BucketNode* ), hashSize );
    /*
    for( unsigned int i = 0; i < hashSize; i++ )
        r_dict->m_data[ i ] = NULL;*/

    /* Store hash size for hashing function */
    r_dict->m_size = hashSize;

    /* Assign Function Pointer */
    r_dict->dctor  = Dict_Destroy;
    r_dict->Insert = Dict_Insert;
    r_dict->Search = Dict_Search;

    return r_dict;
}
