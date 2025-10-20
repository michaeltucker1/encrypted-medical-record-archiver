/* record.c - Patient record linked list implementation */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

/* Create a new patient record */
struct Patient* create_patient(int id, const char* name, int age,
                              const char* diagnosis, const char* treatment,
                              const char* admission_date)
{
    struct Patient* new_patient;
    new_patient = (struct Patient*)malloc(sizeof(struct Patient));
    if (new_patient == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for patient record\n");
        return NULL;
    }

    new_patient->id = id;
    strncpy(new_patient->name, name, sizeof(new_patient->name) - 1);
    new_patient->name[sizeof(new_patient->name) - 1] = '\0';
    new_patient->age = age;
    strncpy(new_patient->diagnosis, diagnosis, sizeof(new_patient->diagnosis) - 1);
    new_patient->diagnosis[sizeof(new_patient->diagnosis) - 1] = '\0';
    strncpy(new_patient->treatment, treatment, sizeof(new_patient->treatment) - 1);
    new_patient->treatment[sizeof(new_patient->treatment) - 1] = '\0';
    strncpy(new_patient->admission_date, admission_date, sizeof(new_patient->admission_date) - 1);
    new_patient->admission_date[sizeof(new_patient->admission_date) - 1] = '\0';
    new_patient->next = NULL;

    return new_patient;
}

/* Add a patient to the linked list */
void add_patient(struct Patient** head, struct Patient* new_patient)
{
    if (*head == NULL) {
        *head = new_patient;
    } else {
        struct Patient* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_patient;
    }
}

/* Print all patients in the list */
void print_patients(const struct Patient* head)
{
    const struct Patient* current = head;
    while (current != NULL) {
        printf("ID: %d\n", current->id);
        printf("Name: %s\n", current->name);
        printf("Age: %d\n", current->age);
        printf("Diagnosis: %s\n", current->diagnosis);
        printf("Treatment: %s\n", current->treatment);
        printf("Admission Date: %s\n", current->admission_date);
        printf("--------------------\n");
        current = current->next;
    }
}

/* Free all patients in the list */
void free_patients(struct Patient* head)
{
    struct Patient* current = head;
    struct Patient* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
