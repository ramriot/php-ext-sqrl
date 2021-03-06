#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "php.h"
#include "b64u.h"

static char encoding_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '-', '_'
};
static char *decoding_table = NULL;

void b64u_build_decoding_table() {
	int i;
	decoding_table = malloc(256);

	for (i = 0; i < 64; i++)
		decoding_table[(unsigned char) encoding_table[i]] = i;
	decoding_table[0] = 0;
}


void b64u_cleanup() {
	if (decoding_table != NULL)	free(decoding_table);
}

char *b64u_encode(const unsigned char *data,
		int input_length,
		int *output_length) {

	int i, j;
	*output_length = 4 * ((input_length + 2) / 3);

	char *encoded_data = emalloc(*output_length);
	if (encoded_data == NULL) return NULL;

	uint32_t octet_a, octet_b, octet_c, triple;
	for (i = 0, j = 0; i < input_length;) {

		octet_a = i < input_length ? data[i++] : 0;
		octet_b = i < input_length ? data[i++] : 0;
		octet_c = i < input_length ? data[i++] : 0;

		triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}
	if( (input_length % 3) > 0 ) {
	  if( (input_length % 3) > 1 ) {
	    *output_length = *output_length - 1;
	  } else {
	    *output_length = *output_length - 2;
	  }
	}
	return encoded_data;
}

unsigned char *b64u_decode(const unsigned char *data,
							 int input_length,
							 int *output_length) {
	int i, j;

	if (decoding_table == NULL) b64u_build_decoding_table();
	*output_length = ceil(input_length / 4.0) * 3;
	
	unsigned char *decoded_data = emalloc(*output_length);
	if (decoded_data == NULL) return NULL;

	uint32_t sextet_a, sextet_b, sextet_c, sextet_d, triple;
	for (i = 0, j = 0; i < input_length;) {

		sextet_a = (i < input_length) ? decoding_table[data[i++]] : 0;
		sextet_b = (i < input_length) ? decoding_table[data[i++]] : 0;
		sextet_c = (i < input_length) ? decoding_table[data[i++]] : 0;
		sextet_d = (i < input_length) ? decoding_table[data[i++]] : 0;
		
		triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6)
				+ (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

		if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}
	return decoded_data;
}
