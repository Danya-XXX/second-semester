

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <intrin.h>
long cnt = 0;
void delcom(FILE* input, FILE* output) {
    int flag = 0; 
    int simb;     
    int presimb = 0;

    while ((simb = fgetc(input)) != EOF) {
        switch (flag) {
        case 1: 
            fputc(simb, output);
            if (simb == '\\') {
                int next = fgetc(input);
                if (next == EOF) break;
                fputc(next, output);
            }
            else if (simb == '"') {
                flag = 0;
            }
            break;

        case 5: 
            fputc(simb, output);
            if (simb == '\\') {
                int next = fgetc(input);
                if (next == EOF) break;
                fputc(next, output);
            }
            else if (simb == '\'') {
                flag = 0;
            }
            break;

        case 2: 
            if (simb == '/') {
                flag = 3; 
            }
            else if (simb == '*') {
                flag = 4; 
            }
            else {
                fputc('/', output);
                fputc(simb, output);
                flag = (simb == '"') ? 1 : (simb == '\'') ? 5 : 0;
            }
            break;

        case 3: 
            if (simb == '\\' && fgetc(input) == '\n') {
                continue;
            }
            if (simb == '\n') {
                fputc(simb, output);
                flag = 0;
            }
            break;

        case 4: 
            if (presimb == '*' && simb == '/') {
                flag = 0;
            }
            presimb = simb;
            break;

        default: 
            if (simb == '"') {
                flag = 1;
                fputc(simb, output);
            }
            else if (simb == '\'') {
                flag = 5;
                fputc(simb, output);
            }
            else if (simb == '/') {
                flag = 2;
            }
            else {
                fputc(simb, output);
            }
        }
    }

    
    if (flag == 2) {
        fputc('/', output);
    }
}

typedef struct SplayTree {
    int count;
    char numb16[100];
    unsigned long long numb10;
    struct SplayTree* parent;
    struct SplayTree* left;
    struct SplayTree* right;

};

unsigned long long hexToDecimal(char* hexString) {
    unsigned long long result = 0;
    int i = 2;
    while (hexString[i] != '\0') {
        char c = toupper(hexString[i]);
        int digit;
        if (isdigit(c)) {
            digit = c - '0';
        }
        else if ((c >= 'A' && c <= 'F')|| (c >= 'a' && c <= 'f')) {
            digit = c - 'A' + 10;
        }
        else {
            fprintf(stderr, "Ошибка: Недопустимый символ в 16-ричной строке: %c\n", c);
            return 0;
        }
        result = result * 16 + digit;
        i++;
    }
   
    return result;
}

SplayTree* create_node(char* numb16) {
    SplayTree* new_node = (SplayTree*)malloc(sizeof(SplayTree));
    if (!new_node) return NULL;

    new_node->count = 1;
    strncpy(new_node->numb16, numb16, sizeof(new_node->numb16) - 1);
    new_node->numb16[sizeof(new_node->numb16) - 1] = '\0';  
    new_node->numb10 = hexToDecimal(numb16);  
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
void printTree(struct SplayTree* root) {
    
    if (root == NULL)
        return;
    printf("Число %s (%llu) встречается %d раз\n", root->numb16, root->numb10, root->count); 
    printTree(root->left);
    printTree(root->right);
}

void zig(SplayTree* node) {
    SplayTree* parent = node->parent;
    SplayTree* grandparent = parent->parent;

    
    parent->left = node->right;
    if (node->right) {
        node->right->parent = parent;
    }

    
    node->parent = grandparent;
    if (grandparent) {
        if (parent == grandparent->left) {
            grandparent->left = node;
        }
        else {
            grandparent->right = node;
        }
    }

    
    node->right = parent;
    parent->parent = node;
}
void zag(SplayTree* node) {
    SplayTree* parent = node->parent;
    SplayTree* grandparent = parent->parent;
    parent->right = node->left;
    if (node->left) {
        node->left->parent = parent;
    }
    node->parent = grandparent;
    if (grandparent) {
        if (parent == grandparent->left) {
            grandparent->left = node;
        }
        else {
            grandparent->right = node;
        }
    }
    node->left = parent;
    parent->parent = node;
}
SplayTree* splay(SplayTree* node) {  
    if (node == NULL) return node;
    while (node->parent) {
        SplayTree* parent = node->parent;
        SplayTree* grandparent = parent->parent;

        if (!grandparent) {
            
            if (node == parent->left) {
                zig(node);  
            }
            else {
                zag(node);
            }
        }
        else {
            if (parent == grandparent->left) {
                
                if (node == parent->left) {
                    
                    zig(parent); 
                    zig(node);
                }
                else {
                    
                    zag(node);
                    zig(node);
                }
            }
            else {
                if (node == parent->right) {
                    
                    zag(parent);
                    zag(node);
                }
                else {
                    
                    zig(node);
                    zag(node);
                }
            }
        }
    }
    return node; 
}

SplayTree* insert(SplayTree* root, char* numb16, unsigned long long numb10) {
    if (root == NULL) {
        return create_node(numb16);
    }

    SplayTree* current = root;
    SplayTree* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (numb10 == current->numb10) {
            current->count++; 
            root = splay(current); 
            return root;
        }
        else if (numb10 < current->numb10) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    
    SplayTree* node = create_node(numb16);
    node->parent = parent;

    if (numb10 < parent->numb10) {
        parent->left = node;
    }
    else {
        parent->right = node;
    }
   
    root = splay(node); 
    
    return root;
}
struct SplayTree* search(struct SplayTree* root, char* numb16) {
    unsigned long long numb10 = hexToDecimal(numb16);
    if (root == NULL) { 
        printf("Число не найдено\n"); 
        return NULL;
    }

    if (numb10 < root->numb10) {
        return search(root->left, numb16); 
    }
    else if (numb10 > root->numb10) {
        return search(root->right, numb16);
    }
    else { 
        printf("Число %s встречается %d раз\n", root->numb16, root->count); 
        root = splay(root);  
        return root; 
    }
}
SplayTree* find_numb16(FILE* file, SplayTree* root) {
  
    char c;  
    while ((c = fgetc(file)) != EOF) {

        if (c != '0') {
            continue;
        }
        int next = fgetc(file);
        if (next == EOF) break;

        if (next == 'x' || next == 'X') {

            char numb16[100] = { '0', next, '\0' };
            int i = 2;


            while (i < sizeof(numb16) - 1) {
                c = fgetc(file);
                if (c == EOF) break;

                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
                    numb16[i++] = c;
                }
                else {
                    ungetc(c, file);
                    break;
                }
            }
            numb16[i] = '\0';

            if (i > 2) {
                unsigned long long num = hexToDecimal(numb16);
                if (num != (unsigned long long) - 1) {
                    root = insert(root, numb16, num);
                    cnt++;
                    
                }
            }
        }
        else if (next != EOF) {
            ungetc(next, file);  
        }
    }
    return root;
}
void freeSplayTree(struct SplayTree* root) {
    if (root == NULL) {
        return;
    }

    freeSplayTree(root->left);
    freeSplayTree(root->right);

    
    free(root);
}
int main()
{
    setlocale(LC_ALL, "rus");
 
    unsigned __int64 start = __rdtsc();
    FILE* input = fopen("Sutable_data_struct.cpp", "r");
    FILE* output = fopen("test_output.c", "w+");
    delcom(input, output);
    fclose(input);
    fclose(output);
    unsigned __int64 end = __rdtsc();
    printf("Открытие файла, удаление комментариев и запись нового проходят за %f секунд\n", (double)(end - start) / 1900000000.0);
   
    FILE* test_prog = fopen("test_output.c", "r");
    SplayTree* root = NULL;
    
    
   
    start = __rdtsc();
    root = find_numb16(test_prog, root);
    end = __rdtsc();
    printf("Всего в файле  %ld 16ричных чисел \nПоиск и вставка проходят за %f секунд\n", cnt,(double)(end - start) / 1900000000.0);
    printTree(root);
    
    
    while (true) {
        char numb16[100];
        printf("Введите число которое хотите найти: ");
        fgets(numb16, sizeof(numb16), stdin);
        numb16[strcspn(numb16, "\n")] = 0;
        unsigned long long numb10;
        search(root, numb16);
    }
    
    freeSplayTree(root);
    fclose(test_prog);
    
    return 0;
}


