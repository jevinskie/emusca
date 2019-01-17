#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "aes.h"

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
	struct AES_ctx ctx;
	memcpy(pt_out, ct_in, sz_in);
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, pt_out, sz_in);
	return;
}

__attribute__((noinline))
void aes_init_marker(void) {
    asm volatile("nop\n\t");
    return;
}

void aes_do_dec_wrapped(void) {
	static int done_init = 0;
	if (!done_init) {
		aes_init_marker();
	}
	aes_do_dec(key, sizeof(key), iv, ct, pt, sizeof(ct));
#ifdef __arm__
	_exit(243);
#endif
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
	// aes_do_dec(key, sizeof(key), iv, ct, pt, sizeof(ct));
	aes_do_dec_wrapped();
	print_hex(pt, sizeof(pt));
	puts("");
	return 0;
}
