#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp      free_data;
    CompareDataFp compare_data;
    InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, InsertInOrder insert_sorted) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->insert_sorted = insert_sorted;
}

// Print all elements of the list
void dump_list(const List* p_list) {
    if (p_list->head == NULL)
        return;
    ListElement *p = p_list->head;
    while (p != NULL){
        p_list->dump_data(p->data);
        p = p->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    if (p_list->head == NULL)
        return;
    ListElement *p = p_list->head;
    while (p != NULL){
        if (p_list->compare_data(p->data, data) == 1)
            p_list->dump_data(p->data);
        p = p->next;
    }
}

// Free all elements of the list
void free_list(List* p_list) {
    if (p_list->head == NULL)
        return;
    ListElement *p = p_list->head;
    ListElement *q = NULL;
    while (p != NULL){
        q = p->next;
        p_list->free_data(p->data);
        p = q;
    }
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement *element = malloc(sizeof (ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = p_list->head;
    p_list->head = element;
    if (!p_list->tail)
        p_list->tail = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = malloc(sizeof(ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = NULL;
    if(p_list->tail) p_list->tail->next = element;
    p_list->tail = element;
    if(!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
    if (p_list->head == NULL)
        return;
    if (p_list->head == p_list->tail){
        p_list->free_data(p_list->head);
        p_list->head = NULL;
        p_list->tail = NULL;
    }
    else{
        ListElement *q = p_list->head->next;
        p_list->free_data(p_list->head);
        p_list->head = q;
    }
}

// Reverse the list
void reverse(List *p_list) {
    if (p_list->head == NULL || p_list->head == p_list->tail)
        return;
    ListElement *p = NULL, *q = p_list->head, *r = NULL;
    p_list->tail = q;
    while (q->next != NULL){
        r = q->next;
        q->next = p;
        p = q;
        q = r;
    }
    q->next = p;
    p_list->head = q;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
    p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    if (p_list->head == NULL)
        return NULL;
    if (p_list->compare_data(p_element->data, p_list->head->data)  >= 0)
        return NULL;
    ListElement *p = p_list->head, *r = p->next;
    while (r != NULL){
        if (p_list->compare_data(p->data, p_element->data) > 0 && p_list->compare_data(p_element->data, r->data) >= 0)
            break;
        p = r;
        r = p->next;
    }
    return p;
}

// Insert element after 'previous'
void push_after(List *p_list, ListElement *element, ListElement *previous) {
    if (previous == NULL) {
//        push_front(p_list,data);
        if (p_list->head == NULL){
            p_list->head = element;
            p_list->tail = element;
        }
        else{
            element->next = p_list->head;
            p_list->head = element;
        }
    }
    else{
//        ListElement *element = malloc(sizeof (ListElement));
//        if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
        element->next = previous->next;
        previous->next = element;
        if(previous == p_list->tail)
            p_list->tail = element;
    }
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {
    ListElement *element = malloc(sizeof (ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = p_data;
    element->next = NULL;
    ListElement *prev = find_insertion_point(p_list, element);
    if (prev == NULL){
        if (p_list->head == NULL){
            push_after(p_list, element, prev);
        } else{
            if(p_list->compare_data(p_list->head->data, element->data) == 0){
//                ((DataWord*)(p_list->head->data))->counter += 1;
//                free_word(newData);
                free(element);
            }
            else{
                push_after(p_list, element, prev);
            }
        }
    }
    else{
        if (prev->next == NULL){
            push_after(prev, element, prev);
        } else{
            if(p_list->compare_data(prev->next->data, element->data) == 0){
//                ((DataWord*)(prev->next->data))->counter += 1;
//                free_word(newData);
                free(element);
            }
            else{
                push_after(p_list, element, prev);
            }
        }
    }
    //push_after(p_list, element, );
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
    int id;
} DataInt;

void dump_int(const void *d) {
    printf("%d ", ((DataInt *)(d))->id);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    if ( ((DataInt*)(a))->id < ((DataInt*)(b))->id )
        return 1;
    else if (((DataInt*)(a))->id > ((DataInt*)(b))->id )
        return -1;
    else
        return 0;
}

DataInt *create_data_int(int v) {
    DataInt *newData = malloc(sizeof (DataInt));
    newData->id = v;
    return newData;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    for(char* p = ((DataWord*)d)->word; *p; p++)
        printf( "%c", *p);
    printf("\n");
}

void dump_word_lowercase (const void *d) {
    for(char* p = ((DataWord*)d)->word; *p; p++)
        printf( "%c", tolower(*p));
    printf("\n");
}

void free_word(void *d) {
    free(((DataWord*)d)->word);
    free(d);
}

int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return (d * (-1));
    }
}

// conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    //return strcmp(((DataWord*)a)->word, ((DataWord*)b)->word);
    return strcicmp(((DataWord*)a)->word, ((DataWord*)b)->word);
}

int cmp_word_counter(const void *a, const void *b) {
    //if (((DataWord*)b)->word == NULL) {
    if (((DataWord *) a)->counter == ((DataWord *) b)->counter)
        return 1;
    return 0;
    //}
    //return strcicmp(((DataWord*)a)->word, ((DataWord*)b)->word);
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data) {

    DataWord * newData = malloc(sizeof (DataWord));
    newData->word = data;
    newData->counter = 1;

    ListElement *element = malloc(sizeof (ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = newData;
    element->next = NULL;
    ListElement *prev = find_insertion_point(p_list, element);
    if (prev == NULL){
        if (p_list->head == NULL){
            push_after(p_list, element, prev);
        } else{
            if(p_list->compare_data(p_list->head->data, element->data) == 0){
                ((DataWord*)(p_list->head->data))->counter += 1;
                free_word(newData);
                free(element);
            }
            else{
                push_after(p_list, element, prev);
            }
        }
    }
    else{
        if (prev->next == NULL){
            push_after(prev, element, prev);
        } else{
            if(p_list->compare_data(prev->next->data, element->data) == 0){
                ((DataWord*)(prev->next->data))->counter += 1;
                free_word(newData);
                free(element);
            }
            else{
                push_after(p_list, element, prev);
            }
        }
    }
    //push_after(p_list, element, );
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {
    if(order == 0){
        char str[BUFFER_SIZE];
        char *delim = ".,?!:;-.\n\r\t ";
//printf("%s",delim);
        while (fgets(str, BUFFER_SIZE, stdin) != NULL){
            char * token = strtok(str, delim);
            while (token != NULL){
                char * s = strdup(token);
                DataWord *w = malloc(sizeof(DataWord));
                w->word = s;
                w->counter = 0;
                push_back(p_list, w);
                token = strtok(NULL, delim);
            }
        }
    }
    else{
        char str[BUFFER_SIZE];
        char *delim = ".,?!:;-.\n\r\t ";
        while (fgets(str, BUFFER_SIZE, stdin) != NULL){
            char * token = strtok(str, delim);
            while (token != NULL){
                char * s = strdup(token);
                insert_in_order(p_list, s);
                //dump_list(p_list);
                token = strtok(NULL, delim);
            }
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
    char op[2];
    int v;
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
        //dump_list(p_list);
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int,
                      cmp_int, insert_elem);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 0);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word_lowercase, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 1);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
//			list.dump_data = dump_word_lowercase;
            dump_list_if(&list, &data);
            printf("\n");
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}