# Lempel-Ziv-Welch
 
 A C library implementation of Lempel, Ziv and Welch compression algorithm.

[Wiki](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)

# Example Usage
```C
int readFunc(void* context)
{
    return fgetc( context );
}

int writeFunc(unsigned char c, void* context)
{
    return fputc( c, context ); 
}

...
 FILE * readFile = fopen( "sample.txt", "rw+"  );
 if( readFile == NULL ) {
     fprintf( stdout, "[ Error ] Cannot open sample.txt\n");
     return 0;
 }
 FILE * writeFile = fopen( "sample.lzw", "w+" );
 if( writeFile == NULL ) {
     fprintf( stdout, "[ Error ] Cannot open sample.lzw\n");
     return 0;
 }
 lzwEncode( 8, 24, readFunc, readFile , writeFunc, writeFile );
 fclose( readFile );
 fclose( writeFile );  
```
