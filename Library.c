#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100

typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
} Book;

void addBook() {
    FILE *fp = fopen("library.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Book b;
    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar(); // to consume newline after ID
    printf("Enter Book Title: ");
    fgets(b.title, MAX_TITLE_LEN, stdin);
    strtok(b.title, "\n"); // remove newline
    printf("Enter Author Name: ");
    fgets(b.author, MAX_AUTHOR_LEN, stdin);
    strtok(b.author, "\n");

    fprintf(fp, "%d|%s|%s\n", b.id, b.title, b.author);
    fclose(fp);
    printf("Book added successfully.\n");
}

void displayBooks() {
    FILE *fp = fopen("library.txt", "r");
    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    Book b;
    printf("\n%-5s %-30s %-30s\n", "ID", "Title", "Author");
    printf("---------------------------------------------------------------\n");
    while (fscanf(fp, "%d|%99[^|]|%99[^\n]\n", &b.id, b.title, b.author) == 3) {
        printf("%-5d %-30s %-30s\n", b.id, b.title, b.author);
    }

    fclose(fp);
}

void searchBook() {
    char keyword[MAX_TITLE_LEN];
    printf("Enter book title to search: ");
    getchar(); // clear newline
    fgets(keyword, MAX_TITLE_LEN, stdin);
    strtok(keyword, "\n");

    FILE *fp = fopen("library.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Book b;
    int found = 0;
    while (fscanf(fp, "%d|%99[^|]|%99[^\n]\n", &b.id, b.title, b.author) == 3) {
        if (strstr(b.title, keyword)) {
            printf("Found: ID=%d, Title=%s, Author=%s\n", b.id, b.title, b.author);
            found = 1;
        }
    }

    if (!found)
        printf("No matching book found.\n");

    fclose(fp);
}

void deleteBook() {
    int del_id;
    printf("Enter ID of the book to delete: ");
    scanf("%d", &del_id);

    FILE *fp = fopen("library.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening files.\n");
        return;
    }

    Book b;
    int found = 0;
    while (fscanf(fp, "%d|%99[^|]|%99[^\n]\n", &b.id, b.title, b.author) == 3) {
        if (b.id != del_id) {
            fprintf(temp, "%d|%s|%s\n", b.id, b.title, b.author);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("library.txt");
    rename("temp.txt", "library.txt");

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book not found.\n");
}

int main() {
    int choice;
    do {
        printf("\n=== Library Book Tracker ===\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book by Title\n");
        printf("4. Delete Book by ID\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}