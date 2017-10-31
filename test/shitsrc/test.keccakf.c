//
// Created by mbikola on 10/31/17.
//

#include <stdio.h>
#include <stdlib.h>

#include "../keccak.h"

#define c2i(x) (( x>='0' && x<='9' ) ? x-'0' :  ( x>='a' && x<='f' ? 10+x-'a' : 10+x-'A'))
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
    const unsigned int ROUNDS = 24;

    unsigned char line[BLOCK_SIZE*2 + 1];
    line[BLOCK_SIZE*2] = 0;

    size_t len = 0;
    int read = 0;


    uint8_t inputByteStream[BLOCK_SIZE];
    uint8_t outByteStream[BLOCK_SIZE];
    unsigned char outStream[BLOCK_SIZE*2+1];
    outStream[BLOCK_SIZE*2] = '\0';


    read = getline(&line, &len, stdin);
    while ( read != -1 ) {

        hexStringToBytes(line, inputByteStream, read);
        memcpy(outByteStream, inputByteStream, BLOCK_SIZE);

        keccakf(outByteStream, ROUNDS);
        bytesToHexString(outByteStream, outStream, BLOCK_SIZE);

        fprintf(stdout, "%s\n", outStream);
        read = getline(&line, &len, stdin);
    }

    exit(EXIT_SUCCESS);
}