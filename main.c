#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "encrypt.h"
#include "compress.h"

#define MAX_RECORD_SIZE 65536 
#define MAX_PASSWORD_LENGTH 256
#define DEFAULT_ARCHIVE_FILE "medical.dat"
#define DEFAULT_PASSWORD "default123"

/* Function prototypes */
void parse_arguments(int argc, char* argv[]);
void display_help(const char* program_name);
void do_add(void);
void do_view(void);
void do_search(const char* term);
void do_sort(void);
void do_delete(const char* target);
void initialize_archive(void);

/* Global variables */
char* current_command = NULL;
char* current_term = NULL;
char* archive_password = NULL;

/* Main function */
int main(int argc, char* argv[])
{
    parse_arguments(argc, argv);

    if (current_command == NULL) {
        display_help(argv[0]);
        return 1;
    }

    /* Initialize archive and get password */
    initialize_archive();

    if (strcmp(current_command, "add") == 0) {
        do_add();
    } else if (strcmp(current_command, "view") == 0) {
        do_view();
    } else if (strcmp(current_command, "search") == 0) {
        if (current_term == NULL) {
            fprintf(stderr, "Error: search command requires a search term\n");
            return 1;
        }
        do_search(current_term);
    } else if (strcmp(current_command, "sort") == 0) {
        do_sort();
    } else if (strcmp(current_command, "delete") == 0) {
        if (current_term == NULL) {
            fprintf(stderr, "Error: delete command requires an ID or search term\n");
            return 1;
        }
        do_delete(current_term);
    } else if (strcmp(current_command, "help") == 0) {
        display_help(argv[0]);
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", current_command);
        display_help(argv[0]);
        return 1;
    }

    /* Free the password if it was prompted */
    if (archive_password != NULL && strcmp(archive_password, DEFAULT_PASSWORD) != 0) {
        free(archive_password);
    }

    return 0;
}

/* Parse command line arguments */
void parse_arguments(int argc, char* argv[])
{
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--add") == 0) {
            current_command = "add";
        } else if (strcmp(argv[i], "--view") == 0) {
            current_command = "view";
        } else if (strcmp(argv[i], "--search") == 0) {
            current_command = "search";
            if (i + 1 < argc) {
                current_term = argv[++i];
            }
        } else if (strcmp(argv[i], "--sort") == 0) {
            current_command = "sort";
        } else if (strcmp(argv[i], "--delete") == 0) {
            current_command = "delete";
            if (i + 1 < argc) {
                current_term = argv[++i];
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            current_command = "help";
        } else {
            fprintf(stderr, "Warning: Unknown argument '%s'\n", argv[i]);
        }
    }
}

/* Display help message */
void display_help(const char* program_name)
{
    printf("Medical Record Archiver\n");
    printf("Usage: %s [MODE]\n", program_name);
    printf("\nModes:\n");
    printf("  --add     Add a new patient record\n");
    printf("  --view    View all patient records\n");
    printf("  --search <term>  Search records by term\n");
    printf("  --delete <id>    Delete record by ID or search term\n");
    printf("  --sort    Sort records by name\n");
    printf("  --help    Show this help message\n");
}

/* Initialize archive and get password */
void initialize_archive(void)
{
    /* Try to use default password first */
    archive_password = DEFAULT_PASSWORD;

    /* If no archive exists, create one */
    struct Record* temp_head = NULL;
    if (load_records(DEFAULT_ARCHIVE_FILE, archive_password, &temp_head) == 0) {
        /* No existing archive, create one */
        save_records(DEFAULT_ARCHIVE_FILE, archive_password, temp_head);
    }
    free_records(temp_head);
}

/* Add a new record */
void do_add(void)
{
    struct Record* head = NULL;

    /* Load existing records */
    load_records(DEFAULT_ARCHIVE_FILE, archive_password, &head);

    /* Get record data from user */
    char data[MAX_RECORD_SIZE];
    printf("Enter record data (format: name:John Doe;age:45;diagnosis:Flu;notes:Recovered):\n");
    if (fgets(data, sizeof(data), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        free_records(head);
        return;
    }

    /* Remove newline */
    int len = strlen(data);
    if (len > 0 && data[len-1] == '\n') {
        data[len-1] = '\0';
    }

    /* Create new record */
    struct Record* new_record = create_record(0, data);

    if (new_record == NULL) {
        fprintf(stderr, "Error: Failed to create record\n");
        free_records(head);
        return;
    }

    /* Assign ID (find max existing ID + 1) */
    struct Record* current = head;
    unsigned int max_id = 0;
    while (current != NULL) {
        if (current->id > max_id) {
            max_id = current->id;
        }
        current = current->next;
    }
    new_record->id = max_id + 1;

    /* Add to list */
    add_record(&head, new_record);

    /* Save all records */
    int saved = save_records(DEFAULT_ARCHIVE_FILE, archive_password, head);
    free_records(head);

    if (saved > 0) {
        printf("Record added successfully (ID: %u).\n", new_record->id);
    } else {
        printf("Error: Failed to save record.\n");
    }
}

/* View all records */
void do_view(void)
{
    struct Record* head = NULL;

    int loaded = load_records(DEFAULT_ARCHIVE_FILE, archive_password, &head);

    if (loaded == 0) {
        printf("No patient records found.\n");
    } else {
        printf("Found %d patient record(s):\n", loaded);
        print_records(head);
    }

    free_records(head);
}

/* Search records by term */
void do_search(const char* term)
{
    struct Record* head = NULL;

    load_records(DEFAULT_ARCHIVE_FILE, archive_password, &head);

    struct Record* results = search_records(head, term);

    if (results == NULL) {
        printf("No records found matching '%s'.\n", term);
    } else {
        printf("Records matching '%s':\n", term);
        print_records(results);
    }

    free_records(head);
    free_records(results);
}

/* Sort records by name */
void do_sort(void)
{
    struct Record* head = NULL;

    load_records(DEFAULT_ARCHIVE_FILE, archive_password, &head);

    if (head == NULL) {
        printf("No records to sort.\n");
        return;
    }

    /* Simple bubble sort by name */
    int swapped;
    struct Record* current;
    struct Record* last = NULL;

    do {
        swapped = 0;
        current = head;

        while (current->next != last) {
            if (strcmp(current->data, current->next->data) > 0) {
                /* Swap data */
                char* temp_data = current->data;
                current->data = current->next->data;
                current->next->data = temp_data;

                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);

    printf("Records sorted by name:\n");
    print_records(head);

    free_records(head);
}

/* Delete records by ID or search term */
void do_delete(const char* target)
{
    struct Record* head = NULL;

    /* Load existing records */
    load_records(DEFAULT_ARCHIVE_FILE, archive_password, &head);

    if (head == NULL) {
        printf("No records to delete.\n");
        return;
    }

    /* Check if target is a number (ID) or text (search term) */
    char* endptr;
    unsigned int delete_id = (unsigned int)strtoul(target, &endptr, 10);

    if (*endptr == '\0') {
        /* Target is a number - delete by ID */
        struct Record* record_to_delete = find_record(head, delete_id);
        if (record_to_delete == NULL) {
            printf("No record found with ID %u.\n", delete_id);
            free_records(head);
            return;
        }

        /* Remove the record from the linked list */
        struct Record* current = head;
        struct Record* previous = NULL;

        while (current != NULL) {
            if (current->id == delete_id) {
                if (previous == NULL) {
                    /* Delete head */
                    head = current->next;
                } else {
                    /* Delete middle/end */
                    previous->next = current->next;
                }

                printf("Deleted record ID %u: %s\n", current->id, current->data);
                free(current->data);
                free(current);
                break;
            }
            previous = current;
            current = current->next;
        }
    } else {
        /* Target is text - delete all matching records */
        struct Record* results = search_records(head, target);
        if (results == NULL) {
            printf("No records found matching '%s'.\n", target);
            free_records(head);
            return;
        }

        printf("Found %d record(s) matching '%s'. Deleting:\n", 1, target); /* Simplified count */

        /* Delete matching records */
        struct Record* current = results;
        while (current != NULL) {
            struct Record* record_to_delete = find_record(head, current->id);
            if (record_to_delete != NULL) {
                /* Remove from main list */
                struct Record* temp = head;
                struct Record* prev = NULL;

                while (temp != NULL) {
                    if (temp->id == record_to_delete->id) {
                        if (prev == NULL) {
                            head = temp->next;
                        } else {
                            prev->next = temp->next;
                        }

                        printf("  - ID %u: %s\n", temp->id, temp->data);
                        free(temp->data);
                        free(temp);
                        break;
                    }
                    prev = temp;
                    temp = temp->next;
                }
            }
            current = current->next;
        }

        free_records(results);
    }

    /* Save the updated list */
    int saved = save_records(DEFAULT_ARCHIVE_FILE, archive_password, head);
    free_records(head);

    if (saved > 0) {
        printf("Records deleted and archive updated.\n");
    } else {
        printf("Error: Failed to save updated archive.\n");
    }
}
