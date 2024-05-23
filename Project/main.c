#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int arrayRows = 10;
int arrayCols = 256;
int totalRows = 0; //of text

char **allocateArray(const int *rows, const int *cols) {
    char **array = malloc(*rows * sizeof(char *));
    if (array == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    for (int i = 0; i < *rows; i++) {
        array[i] = malloc(*cols * sizeof(char));
        if (array[i] == NULL) {
            perror("Failed to allocate memory");
            return NULL;
        }
    }
    return array;
}

char **reallocateRows(const int *old_rows, const int *new_rows, const int *cols, char **array) {
    array = realloc(array, *new_rows * sizeof(char *));
    if (array == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    for (int i = *old_rows; i < *new_rows; i++) {
        array[i] = malloc(*cols * sizeof(char));
        if (array[i] == NULL) {
            perror("Failed to allocate memory");
            return NULL;
        }
    }
    arrayRows = *new_rows;
    return array;
}

void shiftRight(char *array, const int *startIndex, const int *shiftAmount) {
    int arraySize = arrayCols;
    for (int i = arraySize - *shiftAmount; i >= *startIndex; i--) {
        array[i + *shiftAmount] = array[i];
    }
    array[strlen(array) + 1] = '\0';
}

void print_help() {
    printf("Commands: \n"
           "1: Append text symbols to the end\n"
           "2: Start the new line\n"
           "3: Save text into file\n"
           "4: Load text from file\n"
           "5: Print the current text\n"
           "6: Insert text by line and index\n"
           "7: Search text placement\n"
           "8: Clear console\n"
           "0: Exit program\n");
}

char *append_text(char *text) {
    printf("Enter text to append:\n");
    getchar();
    fgets(text, 255, stdin);
    text[strcspn(text, "\n")] = '\0';
    return text + strlen(text);
}

char *start_newline(char **text, char *last_char) {
    *last_char = '\n';
    last_char++;
    *last_char = '\0';
    totalRows++;
    *text[totalRows] = '\0';
    return text[totalRows];
}

void save_file(char **text) {
    FILE *file;
    char file_name[32];
    printf("Enter the file name for saving:");
    scanf("%s", file_name);
    file = fopen(file_name, "w");
    if (file != NULL) {
        for (int i = 0; i <= totalRows; i++) {
            fputs(text[i], file);
        }
        fclose(file);
        printf("Text saved successfully\n");
    }
}

char *load_file(char **text) {
    FILE *file;
    char file_name[32];
    printf("Enter the file name for loading:");
    scanf("%s", file_name);
    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file");
    } else {
        for (int i = 0; i < arrayRows; i++) {
            text[i][0] = '\0';
        }
        while (fgets(*text, 255, file) != NULL) {
            char *last_char = *text + strlen(*text);
            *last_char = '\0';
            text++;
            totalRows++;
        }
    }
    totalRows--;
    fclose(file);
    return *text + strlen(*text);
}

void print_text(char **text) {
    for (int i = 0; i <= totalRows; i++) {
        printf("%s", text[i]);
    }
    printf("\n");
}

void insert_text(char **text) {
    int row, col;
    char entered_text[128];
    printf("Enter the row and column to insert text:");
    scanf("%d %d", &row, &col);
    printf("Enter text to insert:");
    getchar();
    fgets(entered_text, 128, stdin);
    entered_text[strcspn(entered_text, "\n")] = '\0';
    int shift_amount = (int) strlen(entered_text);
    shiftRight(text[row], &col, &shift_amount);
    for (int i = 0; i < shift_amount; i++) {
        text[row][col + i] = entered_text[i];
    }
}

void search_text(char **text) {
    char search_text[128];
    char *position;
    bool found;
    int index;
    printf("Enter text you want to find:");
    getchar();
    fgets(search_text, 128, stdin);
    search_text[strcspn(search_text, "\n")] = '\0';
    printf("Text found in:");
    for (int i = 0; i <= totalRows; ++i) {
        position = text[i];
        while ((position = strstr(position, search_text)) != NULL) {
            index = (int) (position - text[i]);
            printf("%d %d; ", i, index);
            position += strlen(search_text);
            found = true;
        }
    }
    printf("\n");
    if (!found) {
        printf("Text not found");
    }
}

void clear_console() {
#ifdef _WIN64
    system("cls");
#else
    system("clear");
#endif
}

int main(void) {
    char **text = allocateArray(&arrayRows, &arrayCols);
    char *last_char = *text; //movable pointer to the last symbol
    *last_char = '\0';
    while (true) {
        int input;
        printf("Choose the command or enter 9 for commands list:\n");
        scanf("%d", &input);
        switch (input) {
            case 1:
                last_char = append_text(last_char);
                break;
            case 2:
                last_char = start_newline(text, last_char);
                break;
            case 3:
                save_file(text);
                break;
            case 4:
                last_char = load_file(text);
                break;
            case 5:
                print_text(text);
                break;
            case 6:
                insert_text(text);
                break;
            case 7:
                search_text(text);
                break;
            case 8:
                clear_console();
                break;
            case 9:
                print_help();
                break;
            case 0:
                for (int i = 0; i < arrayRows; ++i) {
                    free(text[i]);
                }
                free(text);
                exit(0);
            default:
                printf("The command is not implemented\n");
                break;
        }
    }
}
