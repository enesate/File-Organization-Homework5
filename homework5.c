#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100 // Maksimum satır uzunluğu
#define MAX_BOOKS 1000 // Maksimum kitap sayısı
#define TABLE_SIZE 1000
struct Book {
    char title[MAX_LENGTH];
    char author[MAX_LENGTH];
    int year;
    char isbn[MAX_LENGTH];
};
struct Node {
    struct Book book;
    struct Node* next;
};

struct Node* hashTable[TABLE_SIZE] = {NULL};

struct Book books[MAX_BOOKS]; // Kitapları tutacak dizi
int bookCount = 0; // Toplam kitap sayısı

// Basit bir hash fonksiyonu
unsigned int hashFunction(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = 31 * hash + key[i];
    }
    return hash % TABLE_SIZE;
}

// Hash tablosuna kitap ekleme
void addToHashTable(const char* key, const struct Book* book) {
    unsigned int index = hashFunction(key);
    
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Bellek yetersiz!\n");
        exit(1);
    }
    newNode->book = *book;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Hash tablosundan kitap arama
struct Book* searchHashTable(const char* key) {
    unsigned int index = hashFunction(key);
    
    struct Node* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->book.title, key) == 0 || strcmp(current->book.author, key) == 0) {
            return &(current->book);
        }
        current = current->next;
    }
    return NULL;
}

void readDataFromFile(const char* filename) {
    //printf("burda!\n");
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açma hatasi!\n");
        exit(1);
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d,%s", books[bookCount].title, books[bookCount].author, &books[bookCount].year, books[bookCount].isbn);
         //printf("Kitap Adi: %s", books[bookCount].title);
         //printf("Kitap Adi: %s\n", books[bookCount].author);
        bookCount++;
    }

    fclose(file);
}

struct Book* linearSearch(const char* key) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, key) == 0 || strcmp(books[i].author, key) == 0) {
            return &books[i];
        }
    }
    return NULL;
}

void displayBookDetails(const struct Book* book) {
    if (book != NULL) {
        printf("Kitap Adi: %s\n", book->title);
        printf("Yazar: %s\n", book->author);
        printf("Yayin Yili: %d\n", book->year);
        printf("ISBN: %s\n", book->isbn);
    } else {
        printf("Kitap bulunamadi!\n");
    }
}

int main() {
    readDataFromFile("tr_books.csv");

     int choice;
    printf("Arama yontemini secin:\n");
    printf("1. Lineer Arama\n");
    printf("2. Hash Tablosu\n");
    scanf("%d", &choice);
    getchar(); // \n karakterini temizle

    // Hash tablosuna kitapları ekle
    for (int i = 0; i < bookCount; i++) {
        addToHashTable(books[i].title, &books[i]);
        addToHashTable(books[i].author, &books[i]);
    }

    char searchKey[MAX_LENGTH];
    printf("Aranacak kitabin adini veya yazarini girin: ");
    fgets(searchKey, sizeof(searchKey), stdin);
    // Eğer fgets sonunda bir satır sonu karakteri bıraktıysa, onu kaldıralım
    size_t len = strlen(searchKey);
    if (searchKey[len - 1] == '\n') {
        searchKey[len - 1] = '\0';
    }
    struct Book* foundBook;
    if (choice == 1) {
        foundBook = linearSearch(searchKey);
    } else if (choice == 2) {
        foundBook = searchHashTable(searchKey);
    } else {
        printf("Geçersiz seçim!\n");
        return 1;
    }

    //struct Book* foundBook = linearSearch(searchKey);
    displayBookDetails(foundBook);

    return 0;
}
