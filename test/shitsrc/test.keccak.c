//
// Created by mbikola on 10/31/17.
//

#include <stdio.h>
#include <stdlib.h>

#include "keccak.h"


#define c2i(x) (( x>='0' && x<='9' ) ? x-'0' :  ( x>='a' && x<='f' ? 10+x-'a' : 10+x-'A'))
#define min(x,y) ( x < y ? x : y)

void hexStringToBytes(const unsigned char * const line, uint8_t * const buffer, size_t  lineLength) {
    size_t i = 0 ;
    unsigned char lo, hi;
    while ( i+1 < lineLength ) {
        lo = c2i(line[i]);
        hi = c2i(line[i+1]);
        buffer[i*2] = (hi << 4) | lo;
        i++;
    }
}
void bytesToHexString(const uint8_t * const byteStream, unsigned char * const buffer, size_t byteStreamLength) {
    static char hexconvtab[] = "0123456789abcdef";
    size_t i = 0;
    while( i < byteStreamLength ) {
        buffer[i*2] = hexconvtab[byteStream[i] >> 4];
        buffer[i*2+1] = hexconvtab[byteStream[i] & 0x0f];
        i+=2;
    }
}

int main(int argc, const char **argv) {
    if ( argc > 1 ) {
        fprintf(stderr, "Usage: cat file_with_input_messages_line_by_line | %s\n", argv[0]);
        return -1;
    }

    const size_t BLOCK_SIZE = 200;
    unsigned char *line = NULL;

    size_t len = 0;
    int read = 0;


    uint8_t inputByteStream[BLOCK_SIZE];
    uint8_t outByteStream[BLOCK_SIZE];

    unsigned char outStream[BLOCK_SIZE*2+1];
    outStream[BLOCK_SIZE*2] = '\0';


    while ( (read  = getline(&line, &len, stdin)) > 0 ) {
        fprintf(stderr, "read %d bytes", read );
        hexStringToBytes(line, inputByteStream, min(BLOCK_SIZE*2, read-1));  // last '\n'

        fprintf(stderr, "%s %d %s --> \n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
        keccak1600(inputByteStream, (read-1)/2, outByteStream);
        fprintf(stderr, "<--- %s %d %s \n", __FILE__, __LINE__, __PRETTY_FUNCTION__);

        bytesToHexString(outByteStream, outStream, BLOCK_SIZE);

        fprintf(stdout, "%s\n", outStream);

    }
    if ( line ) {
        free(line);
    }
    exit(EXIT_SUCCESS);
}