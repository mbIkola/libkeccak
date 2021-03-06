// keccak.h
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>

#ifndef KECCAK_H
#define KECCAK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility ("default"))) __attribute__((used))
void keccak(const uint8_t *in, int inlen, uint8_t *md, int mdlen);

__attribute__((visibility ("default"))) __attribute__((used))
void keccakf(uint64_t st[25], int norounds);

__attribute__((visibility ("default"))) __attribute__((used))
void keccak1600(const uint8_t *in, int inlen, uint8_t *md);

__attribute__((visibility ("default"))) __attribute__((used))
int selfTest(int pizdec);

__attribute__((visibility ("default"))) __attribute__((used))
int selfTest2(int pizdec);

#ifdef __cplusplus
}
#endif

#endif
