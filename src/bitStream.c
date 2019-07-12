#include <stdlib.h>
#include <bitStream.h>
#include <math.h>
#include <stdio.h>
/*
==============================================================================================================
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Aleksandr Khavarovskiy
==============================================================================================================*/
void BitStream_dctor(BitStream* this_ptr) 
{
    /* */
    if( ( this_ptr->m_type == BST_OUTPUT ) && ( this_ptr->m_bitLength > 0 ) )
    {
        this_ptr->Internal_Write((this_ptr->m_bitBuffer << (CHAR_BITS - this_ptr->m_bitLength)) & 0xFF, this_ptr->m_context );                
    }
    free( this_ptr );
}

void BitSteam_Write(BitStream* this_ptr, unsigned int nBits, unsigned int code) 
{
    /* You cant write zero bits. */
    if( nBits == 0 )
        return;

    /* Move buffer over and insert the newbits */
    this_ptr->m_bitBuffer  = ( this_ptr->m_bitBuffer << nBits ) | code;
    this_ptr->m_bitLength  += nBits;

    /* If we have less than 8 bits after insertion we do nothing.*/
    if( this_ptr->m_bitLength < 8 )
        return;

    for( unsigned int c = 0 ;this_ptr->m_bitLength >= CHAR_BITS; ) {
        this_ptr->m_bitLength -= CHAR_BITS;
        c = this_ptr->m_bitBuffer >> this_ptr->m_bitLength;
        this_ptr->Internal_Write( c, this_ptr->m_context );
        this_ptr->m_bitBuffer ^= c << this_ptr->m_bitLength;
    }
    /* Really dumb way of dumping bytes
    unsigned int lvByteCount   = this_ptr->m_bitLength / CHAR_BITS;     
    unsigned int lvHangingBits = this_ptr->m_bitLength % CHAR_BITS;

    unsigned int lvBuffer      = this_ptr->m_bitBuffer >> lvHangingBits;
    unsigned int lvBitCount    = lvByteCount * CHAR_BITS;*/

    /* Dump all the bytes into file in order we revice bits */
    //for( int i = (lvBitCount - CHAR_BITS); i >= 0; i -= CHAR_BITS ) {
      //  this_ptr->Internal_Write((lvBuffer >> i) & 0xFF, this_ptr->m_context );
    //}
    /* Clear the written bits*/
    //this_ptr->m_bitBuffer ^= ( lvBuffer << lvHangingBits );
    //this_ptr->m_bitLength  -= lvBitCount;  
    
/* // If you want to use AND instead of XOR to clear bits.
    unsigned int lvMask        = 0;
    
    if( lvHangingBits != 0 ) {
        lvMask = ~0;
        lvMask = (lvMask >> (0x20 - lvHangingBits));
    }
    else {
            // ( lvMask >> 32 ) does not clear all the bits, as it is undefined behaviour. So we do it manually. 
        lvMask = 0;
    }
    this_ptr->m_bitBuffer &= lvMask;
    this_ptr->m_bitLength  -= lvBitCount;  
    */
}

bool BitSteam_Read(BitStream* this_ptr, unsigned int nBits, unsigned int* code) 
{
    /* We limit 24 bits*/
    if( nBits > 24 )
        return false;

    unsigned int lvHangingBits  = 0;

    /* Fetch more data if we need it. */
    for( ;this_ptr->m_bitLength < nBits; ) {
        /* Get the next charachter */
        int c = this_ptr->Internal_Read( this_ptr->m_context );
        /* Is end of file */
        if( c == -1 )  {
            *code = (this_ptr->m_bitBuffer >> ( CHAR_BITS - this_ptr->m_bitLength ));
            return false;
        }
        this_ptr->m_bitBuffer = ( this_ptr->m_bitBuffer << 8 ) |  c;
        this_ptr->m_bitLength += 8;
    }

    /* How many remainder bits are there, so we know how many to clear.*/
    lvHangingBits  = this_ptr->m_bitLength - nBits;

    /* Clear the hanging bits and return the code */
    *code = ( this_ptr->m_bitBuffer >> lvHangingBits );

    /* Clearing the written bits. */
    this_ptr->m_bitBuffer ^= ( *code << lvHangingBits );
    this_ptr->m_bitLength  -= nBits;
    /*
    // If you want to use AND instead of XOR to clear bits.
    unsigned int lvMask         = 0;
    
    if( lvHangingBits != 0 ) {
        lvMask = ~0;
        lvMask = (lvMask >> (0x20 - lvHangingBits));
    }
    else {
        // ( lvMask >> 32 ) does not clear all the bits, as it is undefined behaviour. So we do it manually. 
        lvMask = 0;
    }
    this_ptr->m_bitBuffer &= lvMask;
    this_ptr->m_bitLength  -= nBits;
    */
    return true;
}

BitStream* BitStream_Create_Input(int (*readFunc)(void* context), void* context) 
{
    BitStream * r_bitStream     = malloc( sizeof( BitStream ) );

    r_bitStream->dctor          = BitStream_dctor;
    r_bitStream->Read           = BitSteam_Read;
    r_bitStream->Write          = BitSteam_Write;
    
    r_bitStream->Internal_Read  = readFunc;
    r_bitStream->Internal_Write = NULL;
    r_bitStream->m_type         = BST_INPUT;

    r_bitStream->m_context      = context;
    r_bitStream->m_bitBuffer    = 0;
    r_bitStream->m_bitLength     = 0;
    
    return r_bitStream;
}

BitStream* BitStream_Create_Output(int (*writeFunc)(unsigned char c,void* context),void* context) {

    /* Fetch some memory */
    BitStream * r_bitStream     = malloc( sizeof( BitStream ) );

    r_bitStream->dctor          = BitStream_dctor;
    r_bitStream->Read           = BitSteam_Read;
    r_bitStream->Write          = BitSteam_Write;

    r_bitStream->Internal_Read  = NULL;
    r_bitStream->Internal_Write = writeFunc;
    r_bitStream->m_type         = BST_OUTPUT;

    r_bitStream->m_context      = context;
    r_bitStream->m_bitBuffer    = 0;
    r_bitStream->m_bitLength     = 0;

    return r_bitStream;
}



