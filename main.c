#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "encrypt.h"
#include "compress.h"

#define MODE_ADD 1
#define MODE_VIEW 2
#define MODE_DEBUG 3

#define DEFAULT_KEY 0x42

/*
HOW TO COMPILE:
Run: make
Run: ./medical_archiver

This will show a help menu.

HOW TO USE:

Show help
./medical_archiver --help

Test add mode (creates a sample patient, with demonstration of encryption/decryption)
./medical_archiver --add

Test view mode (currently shows a placeholder)
./medical_archiver --view
*/


int current_mode = 0;
char encryption_key = DEFAULT_KEY;

/* Function prototypes */
void parse_arguments(int argc, char* argv[]);
void display_usage(const char* program_name);
void handle_add_mode(void);
void handle_view_mode(void);

/* Main function */
int main(int argc, char* argv[])
{

    parse_arguments(argc, argv);

    switch (current_mode) {
        case MODE_ADD:
            handle_add_mode();
            break;
        case MODE_VIEW:
            handle_view_mode();
            break;
        default:
            fprintf(stderr, "Error: No valid mode specified\n");
            display_usage(argv[0]);
            return 1;
    }

    return 0;
}

/* Parse the command line input */
void parse_arguments(int argc, char* argv[])
{
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--add") == 0) {
            current_mode = MODE_ADD;
        } else if (strcmp(argv[i], "--view") == 0) {
            current_mode = MODE_VIEW;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            display_usage(argv[0]);
            exit(0);
        } else {
            fprintf(stderr, "Warning: Unknown argument '%s'\n", argv[i]);
        }
    }
}

/* Displays how to use the program */
void display_usage(const char* program_name)
{
    printf("Encrypted Medical Record Archiver\n");
    printf("Usage: %s [MODE] [OPTIONS]\n", program_name);
    printf("Modes:\n");
    printf("  --add    Add a new patient record\n");
    printf("  --view   View existing patient records\n");
    printf("  --help   Show this help message\n");
    printf("\nExample: %s --add\n", program_name);
}

void handle_add_mode(void)
{
    struct Patient* test_patient;
    char encrypted_name[100];
    char decrypted_name[100];

    printf("Add mode - Adding a new patient record\n");

    test_patient = create_patient(
        1, "John Doe", 30, "Hypertension", "ACE inhibitors", "2025-01-01"
    );

    if (test_patient != NULL) {
        printf("Created test patient record:\n");
        printf("ID: %d\n", test_patient->id);
        printf("Name: %s\n", test_patient->name);
        printf("Age: %d\n", test_patient->age);
        printf("Diagnosis: %s\n", test_patient->diagnosis);
        printf("Treatment: %s\n", test_patient->treatment);
        printf("Admission Date: %s\n", test_patient->admission_date);

        printf("\nDemonstrating encryption:\n");
        strcpy(encrypted_name, test_patient->name);
        encrypt_string(encrypted_name, encryption_key);
        printf("Encrypted name: %s\n", encrypted_name);

        strcpy(decrypted_name, encrypted_name);
        decrypt_string(decrypted_name, encryption_key);
        printf("Decrypted name: %s\n", decrypted_name);

        free(test_patient);
    }
}

void handle_view_mode(void)
{
    printf("View mode - Displaying patient records\n");
    printf("No patient records found.\n");
}

