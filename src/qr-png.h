#ifndef QR_PNG_H
#define QR_PNG_H 1

#define INCHES_PER_METER (100.0/2.54)

void qr_png_write_flush( png_structp png );
void qr_png_write_data( png_structp png_ptr, png_bytep data, png_size_t length );
int qr_png_print(QRcode *qrcode);

#endif
