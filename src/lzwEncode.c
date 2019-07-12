#include <sequence.h>
#include <bitStream.h>
#include <dict.h>
#include <lzw.h>

/*
=====================================================================
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Aleksandr Khavarovskiy
 =====================================================================*/
bool lzwEncode(unsigned int bits, unsigned int maxBits,
               int (*readFunc )(void* context), void* readContext,
               int (*writeFunc)(unsigned char c, void* context), void* writeContext) {

    /*=====================================================================*/
    int         lvCode         = 0;
    Dict      * lvDict         = Dict_Create((1 << maxBits));                       /* Create the dictionary */
    BitStream * lvBitOut       = BitStream_Create_Output(writeFunc, writeContext);  /* Create Output BitStream */   
    /*=====================================================================*/
    /* Initialize the first 255 elements in the hash */
    /*=====================================================================*/
    for( unsigned int i = 0; i < 256; i++ ) {
        lvDict->Insert( lvDict, Sequence_Create_Insert( i ), i );
    }
    unsigned int nextCode = 256;
    /*=====================================================================*/
    /* Read the first byte of the file and store in a sequence             */
    /*=====================================================================*/    
    Sequence * w = Sequence_Create_Insert( readFunc( readContext ) );

    /*=====================================================================*/    
    /* Read characters from file untill we hit a EOF( -1 )                */
    /*=====================================================================*/    
    for( int c = readFunc( readContext ); c != -1; c = readFunc( readContext ))
    {
        Sequence * x = Sequence_Create_Copy( w ); 
        x->Append( x, c );

        if( lvDict->Search( lvDict, x, &lvCode ) ) 
        {
            w->dctor( w );
            w = x;
        } 
        else 
        {
            if( lvDict->Search( lvDict, w, &lvCode ) ) {
                lvBitOut->Write( lvBitOut, bits, lvCode );              
            }
            if( nextCode < lvDict->m_size ) {           
                lvDict->Insert( lvDict, x, nextCode );
                nextCode++;

                /* Do we need more bits */
                if( nextCode > ( 1 << bits ))
                    bits++;
            }    
            w->dctor( w );
            w = Sequence_Create_Insert( c );
        }           
    }
    /*=====================================================================*/
    /* If we have a leftover sequence we can assign its corresponding code.*/
    /*=====================================================================*/    
    if( lvDict->Search( lvDict, w, &lvCode ) ) {
        lvBitOut->Write( lvBitOut, bits, lvCode );                
    }
    /*=====================================================================*/
    /* Clean Up */
    /*=====================================================================*/    
    w->dctor( w );
    lvBitOut->dctor( lvBitOut );    
    lvDict->dctor( lvDict );
    return true;
}
