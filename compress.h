#ifndef COMPRESS_H
#define COMPRESS_H

/* RLE compression functions */
int compress_rle(const char* input, int input_length, char* output, int output_size);
int decompress_rle(const char* input, int input_length, char* output, int output_size);

/* Little-endian utility functions */
void write_u32_le(unsigned char* buffer, unsigned long value);
unsigned long read_u32_le(const unsigned char* buffer);
void write_u64_le(unsigned char* buffer, unsigned long long value);
unsigned long long read_u64_le(const unsigned char* buffer);

#endif
