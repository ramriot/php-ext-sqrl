#ifndef B64U_H
#define B64U_H 1

char *b64u_encode( const unsigned char *data, int input_length, int *output_length );
unsigned char *b64u_decode( const char *data, int input_length, int *output_length );

#endif
