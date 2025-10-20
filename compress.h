#ifndef COMPRESS_H
#define COMPRESS_H


int compress_rle(const char* input, int input_length, char* output, int output_size);
int decompress_rle(const char* input, int input_length, char* output, int output_size);

#endif
