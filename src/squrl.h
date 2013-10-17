#ifndef SQURL_H
#define SQURL_H 1

char *squrl_encode( const unsigned char *data, int input_length, int *output_length );
unsigned char *squrl_decode( const char *data, int input_length, int *output_length );

#endif
