/* record.c - Medical record linked list implementation */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "encrypt.h"
#include "compress.h"

/* Create a new record */
struct Record* create_record(unsigned int id, const char* data)
{
    struct Record* new_record;
    new_record = (struct Record*)malloc(sizeof(struct Record));
    if (new_record == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for record\n");
        return NULL;
    }

    new_record->id = id;
    new_record->data = strdup(data);
    if (new_record->data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for record data\n");
        free(new_record);
        return NULL;
    }
    new_record->next = NULL;

    return new_record;
}

/* Add a record to the linked list */
void add_record(struct Record** head, struct Record* new_record)
{
    if (*head == NULL) {
        *head = new_record;
    } else {
        struct Record* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_record;
    }
}

/* Print all records in the list */
void print_records(const struct Record* head)
{
    const struct Record* current = head;
    while (current != NULL) {
        printf("ID: %u\n", current->id);
        printf("Data: %s\n", current->data);
        printf("--------------------\n");
        current = current->next;
    }
}

/* Free all records in the list */
void free_records(struct Record* head)
{
    struct Record* current = head;
    struct Record* next;

    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

/* Find a record by ID */
struct Record* find_record(struct Record* head, unsigned int id)
{
    struct Record* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Load records from archive file */
int load_records(const char* filename, const char* password, struct Record** head)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0; /* No file exists yet */
    }

    char header[8];
    if (fread(header, 1, 7, file) != 7) {
        fclose(file);
        return 0;
    }
    header[7] = '\0';

    /* Check if header matches "ARCHV1" followed by newline */
    if (strncmp(header, "ARCHV1", 6) != 0) {
        fclose(file);
        fprintf(stderr, "Error: Invalid archive format\n");
        return 0;
    }

    /* Skip newline if present */
    if (header[6] == '\n') {
        /* Already consumed the newline */
    } else {
        fgetc(file); /* Skip newline */
    }

    unsigned int next_id = 1;
    int records_loaded = 0;

    while (!feof(file)) {
        unsigned char length_bytes[4];
        unsigned char timestamp_bytes[8];

        if (fread(length_bytes, 1, 4, file) != 4) break;
        if (fread(timestamp_bytes, 1, 8, file) != 8) break;

        unsigned long record_length = read_u32_le(length_bytes);

        if (record_length == 0 || record_length > 65536) { 
            break;
        }

        char* compressed_data = malloc(record_length);
        if (compressed_data == NULL) {
            fclose(file);
            return records_loaded;
        }

        if (fread(compressed_data, 1, record_length, file) != record_length) {
            free(compressed_data);
            break;
        }

        /* Decrypt the data */
        xor_encrypt(compressed_data, record_length, password[0]);

        /* Decompress the RLE data */
        char* decompressed_data = malloc(65536); /* Max 64KB */
        if (decompressed_data == NULL) {
            free(compressed_data);
            fclose(file);
            return records_loaded;
        }

        int decompressed_length = decompress_rle(compressed_data, record_length, decompressed_data, 65536);
        free(compressed_data);

        if (decompressed_length <= 0) {
            free(decompressed_data);
            break;
        }

        decompressed_data[decompressed_length] = '\0';

        /* Create record */
        struct Record* record = create_record(next_id++, decompressed_data);
        free(decompressed_data);

        if (record != NULL) {
            add_record(head, record);
            records_loaded++;
        }
    }

    fclose(file);
    return records_loaded;
}

/* Save records to archive file */
int save_records(const char* filename, const char* password, const struct Record* head)
{
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot create archive file\n");
        return 0;
    }

    /* Write header */
    if (fwrite("ARCHV1\n", 1, 7, file) != 7) {
        fclose(file);
        return 0;
    }

    const struct Record* current = head;
    int records_saved = 0;

    while (current != NULL) {
        int data_length = strlen(current->data);

        /* Compress the data */
        char* compressed_data = malloc(data_length * 2 + 1); /* Worst case expansion */
        if (compressed_data == NULL) {
            fclose(file);
            return records_saved;
        }

        int compressed_length = compress_rle(current->data, data_length, compressed_data, data_length * 2 + 1);

        /* Encrypt the compressed data */
        xor_encrypt(compressed_data, compressed_length, password[0]);

        /* Write record: 4-byte length, 8-byte timestamp, compressed data */
        unsigned char length_bytes[4];
        unsigned char timestamp_bytes[8];

        write_u32_le(length_bytes, (unsigned long)compressed_length);

        if (fwrite(length_bytes, 1, 4, file) != 4 ||
            fwrite(timestamp_bytes, 1, 8, file) != 8 ||
            fwrite(compressed_data, 1, compressed_length, file) != compressed_length) {
            free(compressed_data);
            fclose(file);
            return records_saved;
        }

        free(compressed_data);
        current = current->next;
        records_saved++;
    }

    fclose(file);
    return records_saved;
}

/* Search records by term */
struct Record* search_records(struct Record* head, const char* term)
{
    struct Record* results = NULL;
    struct Record* current = head;

    while (current != NULL) {
        if (strstr(current->data, term) != NULL) {
            struct Record* new_result = create_record(current->id, current->data);
            add_record(&results, new_result);
        }
        current = current->next;
    }
    return results;
}
