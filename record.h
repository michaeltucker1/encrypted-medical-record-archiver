#ifndef RECORD_H
#define RECORD_H

/* Patient record */
struct Patient {
    int id;
    char name[100];
    int age;
    char diagnosis[200];
    char treatment[200];
    char admission_date[20]; /* YYYY-MM-DD */
    struct Patient *next;
};

/* Function prototypes */
struct Patient* create_patient(int id, const char* name, int age,
                              const char* diagnosis, const char* treatment,
                              const char* admission_date);
void add_patient(struct Patient** head, struct Patient* new_patient);
void print_patients(const struct Patient* head);
void free_patients(struct Patient* head);

#endif 
