#ifndef RECORD_H
#define RECORD_H

/* Record structure for medical archiver */
struct Record {
    unsigned int id;
    char *data;
    struct Record *next;
};

/* Function prototypes */
struct Record* create_record(unsigned int id, const char* data);
void add_record(struct Record** head, struct Record* new_record);
void print_records(const struct Record* head);
void free_records(struct Record* head);
int load_records(const char* filename, const char* password, struct Record** head);
int save_records(const char* filename, const char* password, const struct Record* head);
struct Record* find_record(struct Record* head, unsigned int id);
struct Record* search_records(struct Record* head, const char* term);

#endif 
