#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "mbedtls/aes.h"

void read_hex(const char *hex_str, uint8_t *out_buf, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		assert(sscanf(hex_str, "%02hhx", &out_buf[i]) == 1);
		hex_str += 2;
	}
}

void print_hex(const uint8_t *buf, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%02hhx", buf[i]);
	}
}

uint8_t key[0x10];
uint8_t iv[0x10];
uint8_t ct[0x10];
uint8_t pt[0x10];

void aes_do_dec(const uint8_t *key_in, size_t key_sz, uint8_t *iv_in, const uint8_t *ct_in, uint8_t *pt_out, size_t sz_in) {
	mbedtls_aes_context ctx;
	mbedtls_aes_setkey_dec(&ctx, key_in, key_sz*8);
	mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, sz_in, iv_in, ct_in, pt_out);
	return;
}

void aes_do_dec_wrapped(void) {
	aes_do_dec(key, sizeof(key), iv, ct, pt, sizeof(ct));
	_exit(243);
}

int main(int argc, const char **argv) {
	_Static_assert(sizeof(ct) == sizeof(pt), "ct and pt size differ");
	if (argc != 4) {
		printf("Usage: %s <key hex> <iv hex> <ct hex>\n", argv[0]);
		return -1;
	}
	read_hex(argv[1], key, sizeof(key));
	read_hex(argv[2], iv, sizeof(iv));
	read_hex(argv[3], ct, sizeof(pt));
	aes_do_dec(key, sizeof(key), iv, ct, pt, sizeof(ct));
	print_hex(pt, sizeof(pt));
	puts("");
	return 0;
}
