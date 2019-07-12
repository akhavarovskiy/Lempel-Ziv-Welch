#include <sequence.h>
#include <bitStream.h>
#include <lzw.h>
#include <dict.h>
/*
=====================================================================
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Aleksandr Khavarovskiy
=====================================================================*/
bool lzwDecode(unsigned int bits, unsigned int maxBits,
               int (*readFunc )(void* context), void* readContext,
               int (*writeFunc)(unsigned char c, void* context), void* writeContext) {

    /*=====================================================================*/
    /* Create the Dictionary and BitStream Objects*/
    /*=====================================================================*/
    Dict      * lvDict      = Dict_Create((1 << maxBits));
    BitStream * lvBitStream = BitStream_Create_Input(readFunc, readContext);
    /*=====================================================================*/
    /* Init the first 255 elements in the hash */
    /*=====================================================================*/    
    unsigned int lvLzwCode = 0;
    unsigned int lvPrevCode = 0;
    unsigned int lvCurrentCode = 0;

    for( unsigned int i = 0; i < 256; i++ ) {
        lvDict->m_data[ i ] = BucketNode_Create_Insert( Sequence_Create_Insert( i ), (void*)0 );
    }
    lvLzwCode = 256;
    /*=====================================================================*/
    /* Grab the first code avalable and ouput its sequence. */
    /*=====================================================================*/   
    lvBitStream->Read( lvBitStream, bits, &lvPrevCode );

    Sequence * lvSequenceOut = lvDict->m_data[ lvPrevCode ]->m_data;
    lvSequenceOut->Output( lvSequenceOut, writeFunc, writeContext );               
    /*=====================================================================*/   
    /* Do we need a bigger bitsize to store the codes. */     
    /*=====================================================================*/    
    if( lvLzwCode >= (1 << bits) ) 
        bits++;

    /*=====================================================================*/
    /* Loop while we can still fetch the bytes from the stream. */
    /*=====================================================================*/    
    for( unsigned char c = 0; lvBitStream->Read( lvBitStream, bits, &lvCurrentCode ); ) 
    {      
        if( lvDict->m_data[ lvCurrentCode ] != 0 )  {
            c = *lvDict->m_data[ lvCurrentCode ]->m_data->m_begin;
        }
        else {
            c = *lvDict->m_data[ lvPrevCode ]->m_data->m_begin;
        }
        /* If we can assign more codes we do so.*/
        if( lvLzwCode < lvDict->m_size ) 
        {
            Sequence * w = Sequence_Create_Copy( lvDict->m_data[ lvPrevCode ]->m_data );
            w->Append( w, c );

            lvDict->m_data[ lvLzwCode++ ] = BucketNode_Create_Insert( w, (void*)0 ); 

            /* Do we need a bigger bitsize to store the codes. */
            if( lvLzwCode >= (1 << bits)) 
                bits++;
        }       
        lvSequenceOut = lvDict->m_data[ lvCurrentCode ]->m_data;
        lvSequenceOut->Output( lvSequenceOut, writeFunc, writeContext );                
                         
        lvPrevCode    = lvCurrentCode;  
        lvCurrentCode = 0;                    
    }
    /*=====================================================================*/                    
    /* Do some memory clean up.*/
    /*=====================================================================*/                
    lvBitStream->dctor( lvBitStream );
    lvDict->dctor( lvDict );
    return true;
}
