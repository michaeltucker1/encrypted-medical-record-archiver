#include "compress.h"

/* Need to implement an actual RLE compression and decompression algorithm */
int compress_rle(const char* input, int input_length, char* output, int output_size)
{
    int copy_length = (input_length < output_size) ? input_length : output_size - 1;

    int i;
    for (i = 0; i < copy_length; i++) {
        output[i] = input[i];
    }
    output[copy_length] = '\0';

    return copy_length;
}

int decompress_rle(const char* input, int input_length, char* output, int output_size)
{
    int copy_length = (input_length < output_size) ? input_length : output_size - 1;

    int i;
    for (i = 0; i < copy_length; i++) {
        output[i] = input[i];
    }
    output[copy_length] = '\0';

    return copy_length;
}
