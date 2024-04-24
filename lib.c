#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BOOK_NAME_LEN 100
#define ISBN_LEN 10
#define BORROWER_NAME_LEN 100
#define DUE_DATE_LEN 11
#define FIELD_LEN 50

void read_string(char* string, int size, FILE* fp) {

    fgets(string, size + 1, fp);

    string[strlen(string) - 1] = '\0';    
}

typedef struct Book {
    char bookName[BOOK_NAME_LEN];
    char bookISBN[ISBN_LEN];
    int borrowed;
    char borrowerName[BORROWER_NAME_LEN];
    char dueDate[DUE_DATE_LEN];
    char field[FIELD_LEN];
}Book;

typedef struct Node{

    Book book;

    struct Node* next;

    struct Node* prev;
}Node;

typedef struct DoublyLinkedList{

    struct Node* head;
    struct Node* tail;
}DoublyLinkedList;

void append_end(DoublyLinkedList* list, Node* node) {

    if (list->head == NULL) {

        list->head = list->tail = node;
    } else {

        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
}

void add(DoublyLinkedList* list, Book* book) {

    Node* node = malloc(sizeof(Node));

    if (node == NULL) {
        printf ("Failed to allocate memory!\n");
        exit (EXIT_FAILURE);
    }

    node->next = node->prev = NULL;
    node->book = *book;

    append_end(list, node);
}

void add_sorted(DoublyLinkedList* list, Book* book) {

    Node* node = malloc(sizeof(Node));

    if (node == NULL) {
        printf ("Failed to allocate memory!\n");
        exit (EXIT_FAILURE);
    }

    node->next = node->prev = NULL;
    node->book = *book;

    Node* tempNode = list->head;
    while (tempNode != NULL && strcasecmp(tempNode->book.bookName, book->bookName) <= 0) {

        tempNode = tempNode->next;
    }

    if (tempNode == NULL) {

        append_end(list, node);
    } else {

        Node* prevNode = tempNode->prev;
        node->prev = tempNode->prev;
        node->next = tempNode;
        tempNode->prev = node;

        if (prevNode == NULL) {

            list->head = node;
        } else {

            prevNode->next = node;
        }
    }
}

Node* find_node(DoublyLinkedList* list, char* ISBN) {

    Node* tempNode = list->head;

    while (tempNode != NULL) {

        if (strcasecmp(tempNode->book.bookISBN, ISBN) == 0)

            return tempNode;

        tempNode = tempNode->next;
    }

    return NULL;
}

void delete_node(DoublyLinkedList* list, Node* node) {

    if (node->prev != NULL) node->prev->next = node->next;
    else                    list->head = node->next;

    if (node->next != NULL) node->next->prev = node->prev;
    else                    list->tail = node->prev;

    free (node);
}

void display(DoublyLinkedList* list) {

    int count = 0;

    printf ("%-30s%-10s%-10s%-20s%-15s%-10s\n", "BookName", "BookISBN", "Borrowed", "BorrowerName", "DueDate", "Field");

    Node* tempNode = list->head;

    while (tempNode != NULL) {

        Book book = tempNode->book;
        printf ("%-30s%-10s%-10s%-20s%-15s%-10s\n", book.bookName, book.bookISBN, (book.borrowed ? "YES" : "NO"), book.borrowerName, book.dueDate, book.field);

        tempNode = tempNode->next;

        count++;
    }

    printf ("Total %d record(s)\n", count);
}

void display_book(Book* book) {
    printf ("BookName: %s\n", book->bookName);
    printf ("BookISBN: %s\n", book->bookISBN);
    printf ("Borrowed: %s\n", book->borrowed ? "YES" : "NO");
    if (book->borrowed) {
        printf ("BorrowerName: %s\n", book->borrowerName);
        printf ("DueDate: %s\n", book->dueDate);
    }
    printf ("Field: %s\n", book->field);
}

void insert(DoublyLinkedList* list){

    Book book;
    printf ("Enter book details: \n");
    printf ("BookName: ");
    read_string(book.bookName, BOOK_NAME_LEN, stdin);
    printf ("Book ISBN: ");
    read_string(book.bookISBN, ISBN_LEN, stdin);

    printf ("Borrowed(0/1)? ");
    fscanf(stdin, "%d%*c", &book.borrowed);

    if (book.borrowed) {
        printf ("Borrower Name: ");
        read_string(book.borrowerName, BORROWER_NAME_LEN, stdin);
        printf ("Due Date: ");
        read_string(book.dueDate, DUE_DATE_LEN, stdin);        
    } else {

        strcpy(book.borrowerName, "None");
        strcpy(book.dueDate, "None");
    }
    printf ("Field: ");
    read_string(book.field, FIELD_LEN, stdin);

    if (find_node(list, book.bookISBN) != NULL) {
        printf ("There is already a book with ISBN %s\n", book.bookISBN);
    } else {

        add_sorted(list, &book);
        printf ("Book added!\n");
    }
}

void update(DoublyLinkedList* list){ 

    char isbn[ISBN_LEN];
    printf ("Enter the ISBN of the book: ");
    read_string(isbn, ISBN_LEN, stdin);

    Node* node = find_node(list, isbn);
    if (node == NULL) {

        printf ("No book with ISBN %s exist!\n", isbn);
    } else {

        printf ("Borrowed(0/1)? ");
        fscanf(stdin, "%d%*c", &node->book.borrowed);

        if (node->book.borrowed) {
            printf ("Borrower Name: ");
            read_string(node->book.borrowerName, BORROWER_NAME_LEN, stdin);
            printf ("Due Date: ");
            read_string(node->book.dueDate, DUE_DATE_LEN, stdin);
            printf("Field:");
            read_string(node->book.field, FIELD_LEN, stdin);
            printf ("Book details updated!");   
        } else {

            strcpy(node->book.borrowerName, "None");
            strcpy(node->book.dueDate, "None");
        }
    }
}

void delete(DoublyLinkedList* list){

    char isbn[ISBN_LEN];
    printf ("Enter the ISBN of the book: ");
    read_string(isbn, ISBN_LEN, stdin);

    Node* node = find_node(list, isbn);

    if (node == NULL) {

        printf ("No book with ISBN %s exist!\n", isbn);
    } else {

        delete_node(list, node);
        printf ("Book deleted!\n");
    }
}

void search(DoublyLinkedList* list){

    int choice;

    printf ("1 Search by ISBN\n");
    printf ("2 Search by field\n");
    printf ("3 Search by name\n");
    printf ("4 Search for `Borrowed Books` by borrower name\n");
    printf ("Enter your choice: ");

    scanf ("%d%*c", &choice);

    if (1 <= choice && choice <= 4) {

        int size = 0;
        if (choice == 1) {
            printf ("ISBN: ");
            size = ISBN_LEN;
        } else if (choice == 2) {
            printf ("Field: ");
            size = FIELD_LEN;
        } else if (choice == 3){ 
            printf ("Book Name: ");
            size = BOOK_NAME_LEN;
        } else {
            printf ("Borrower Name: ");
            size = BORROWER_NAME_LEN;
        }

        char str[size];

        read_string(str, size, stdin);

        int count = 0;
        printf("Here are the search results: \n");

        Node* tempNode = list->head;

        while (tempNode != NULL) {

            bool matches = false;
            matches = matches || (choice == 1 && strcasecmp(tempNode->book.bookISBN, str) == 0);
            matches = matches || (choice == 2 && strcasecmp(tempNode->book.field, str) == 0);
            matches = matches || (choice == 3 && strcasecmp(tempNode->book.bookName, str) == 0);
            matches = matches || (choice == 4 && tempNode->book.borrowed && strcasecmp(tempNode->book.borrowerName, str) == 0);
            if (matches){

                display_book(&tempNode->book);
                count++;
            }

            tempNode = tempNode->next;
        }
        printf ("Total %d matches found!\n", count);
    } else {

        printf ("Invalid choice!\n");
    }

}

void terminate(DoublyLinkedList* list) {
    int filename_size = 100;
    char filename[filename_size];
    printf ("Enter filename to save details: ");
    read_string(filename, filename_size, stdin);
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf ("Failed to open `%s`\n", filename);
        exit (EXIT_FAILURE);
    }
    int count = 0;
    Node* tempNode = list->head;
    while (tempNode != NULL) {
        fprintf (fp, "%s\n", tempNode->book.bookName);
        fprintf (fp, "%s\n", tempNode->book.bookISBN);
        fprintf (fp, "%d\n", tempNode->book.borrowed);
        fprintf (fp, "%s\n", tempNode->book.borrowerName);
        fprintf (fp, "%s\n", tempNode->book.dueDate);
        fprintf (fp, "%s\n", tempNode->book.field);
        tempNode = tempNode->next;  
        count++;      
    }
    fclose (fp);
    
    exit (EXIT_SUCCESS);
}

int main(){

    DoublyLinkedList list = {.head = NULL, .tail = NULL};

    int filename_size = 100;
    char filename[filename_size];

    printf("Enter file path: ");

    read_string(filename, filename_size, stdin);

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {

        printf ("Failed to read `%s`\n", filename);
    } else {

        int count = 0;

        while (true) {

            Book book;

            read_string(book.bookName, BOOK_NAME_LEN, fp);
            read_string(book.bookISBN, ISBN_LEN, fp);

            fscanf(fp, "%d%*c", &book.borrowed);
            read_string(book.borrowerName, BORROWER_NAME_LEN, fp);
            read_string(book.dueDate, DUE_DATE_LEN, fp);
            read_string(book.field, FIELD_LEN, fp);

            if (find_node(&list, book.bookISBN) != NULL) {

                printf ("There is already a book with ISBN %s\n", book.bookISBN);
            } else {

                add_sorted(&list, &book);
            }

            count++;

            if (feof(fp)) break;
        }

        fclose (fp);
    }

    int choice;

    while (true) {

        printf ("\n\nMENU\n");
        printf ("1. Display all books\n");
        printf ("2. Insert a new book\n");
        printf ("3. Update a book by ISBN\n");
        printf ("4. Delete a book by ISBN\n");
        printf ("5. Search\n");
        printf ("6. Exit\n");
        printf ("Enter your choice: ");

        scanf ("%d%*c", &choice);

        switch (choice) {
            case 1: display(&list); break;
            case 2: insert(&list);  break;
            case 3: update(&list);  break;
            case 4: delete(&list);  break;
            case 5: search(&list);  break;
            case 6: terminate(&list); break; 
            case 7: printf ("Invalid choice!");
        }

    }

    return 0;
}