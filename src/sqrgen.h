#ifndef SQRGEN_H
#define SQRGEN_H 1

#define INCHES_PER_METER (100.0/2.54)

void sqrgen_write_flush( png_structp png );
void sqrgen_write_data( png_structp png_ptr, png_bytep data, png_size_t length );
int sqrgen_print_png(QRcode *qrcode);

#endif
