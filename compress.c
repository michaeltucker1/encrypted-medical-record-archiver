#include "compress.h"

/* RLE compression: [count][value] where count is number of consecutive identical bytes */
int compress_rle(const char* input, int input_length, char* output, int output_size)
{
    int input_pos = 0;
    int output_pos = 0;

    while (input_pos < input_length && output_pos + 1 < output_size) {
        char current_byte = input[input_pos];
        int run_length = 1;

        /* Count consecutive identical bytes */
        while (input_pos + run_length < input_length &&
               input[input_pos + run_length] == current_byte &&
               run_length < 255) {
            run_length++;
        }

        /* Always encode as [count][value] */
        if (output_pos + 2 > output_size) break;
        output[output_pos++] = (char)run_length;
        output[output_pos++] = current_byte;

        input_pos += run_length;
    }

    return output_pos;
}

int decompress_rle(const char* input, int input_length, char* output, int output_size)
{
    int input_pos = 0;
    int output_pos = 0;

    while (input_pos + 1 < input_length && output_pos < output_size) {
        char count = input[input_pos++];
        char value = input[input_pos++];

        if (count <= 0) break; /* Safety check */

        int remaining_space = output_size - output_pos;
        if (remaining_space < count) {
            count = remaining_space;
        }

        int i;
        for (i = 0; i < count && output_pos < output_size; i++) {
            output[output_pos++] = value;
        }
    }

    return output_pos;
}

/* Little endian utility functions */
void write_u32_le(unsigned char* buffer, unsigned long value)
{
    buffer[0] = (unsigned char)(value & 0xFF);
    buffer[1] = (unsigned char)((value >> 8) & 0xFF);
    buffer[2] = (unsigned char)((value >> 16) & 0xFF);
    buffer[3] = (unsigned char)((value >> 24) & 0xFF);
}

unsigned long read_u32_le(const unsigned char* buffer)
{
    return ((unsigned long)buffer[0]) |
           (((unsigned long)buffer[1]) << 8) |
           (((unsigned long)buffer[2]) << 16) |
           (((unsigned long)buffer[3]) << 24);
}

void write_u64_le(unsigned char* buffer, unsigned long long value)
{
    buffer[0] = (unsigned char)(value & 0xFF);
    buffer[1] = (unsigned char)((value >> 8) & 0xFF);
    buffer[2] = (unsigned char)((value >> 16) & 0xFF);
    buffer[3] = (unsigned char)((value >> 24) & 0xFF);
    buffer[4] = (unsigned char)((value >> 32) & 0xFF);
    buffer[5] = (unsigned char)((value >> 40) & 0xFF);
    buffer[6] = (unsigned char)((value >> 48) & 0xFF);
    buffer[7] = (unsigned char)((value >> 56) & 0xFF);
}

unsigned long long read_u64_le(const unsigned char* buffer)
{
    return ((unsigned long long)buffer[0]) |
           (((unsigned long long)buffer[1]) << 8) |
           (((unsigned long long)buffer[2]) << 16) |
           (((unsigned long long)buffer[3]) << 24) |
           (((unsigned long long)buffer[4]) << 32) |
           (((unsigned long long)buffer[5]) << 40) |
           (((unsigned long long)buffer[6]) << 48) |
           (((unsigned long long)buffer[7]) << 56);
}
