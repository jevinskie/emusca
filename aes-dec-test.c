#include <assert.h>
#include <stdint.h>
#include <stdio.h>

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

int main(int argc, const char **argv) {
	mbedtls_aes_context ctx;
	uint8_t key[0x10] = {0};
	uint8_t iv[0x10] = {0};
	uint8_t ct[0x10] = {0};
	uint8_t pt[0x10] = {0};
	_Static_assert(sizeof(ct) == sizeof(pt), "ct and pt size differ");
	if (argc != 4) {
		printf("Usage: %s <key hex> <iv hex> <ct hex>\n", argv[0]);
		return -1;
	}
	read_hex(argv[1], key, sizeof(key));
	read_hex(argv[2], iv, sizeof(iv));
	read_hex(argv[3], ct, sizeof(pt));
	mbedtls_aes_setkey_dec(&ctx, key, sizeof(key)*8);
	mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, sizeof(pt), iv, ct, pt);
	print_hex(pt, sizeof(pt));
	puts("");
	return 0;
}
