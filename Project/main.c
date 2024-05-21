#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int initial_rows = 10;
int initial_cols = 256;
int total_rows = 0;

char **allocate_2d_array(int *rows, int *cols) {
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

char **reallocate_2d_array(const int *old_rows, const int *new_rows, const int *cols, char **array) {
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
    initial_rows = *new_rows;
    return array;
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
    fgets(text, 100, stdin);
    text[strcspn(text, "\n")] = '\0';
    return text + strlen(text);
}

char *start_newline(char **text, char *last_char) {
    *last_char = '\n';
    total_rows++;
    *text[total_rows] = '\0';
    return text[total_rows];
}

void save_file(char **text) {
    FILE *file;
    char file_name[32];
    printf("Enter the file name for saving:");
    scanf("%s", file_name);
    file = fopen(file_name, "w");
    if (file != NULL) {
        for (int i = 0; i <= total_rows; i++) {
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
        while (fgets(*text, initial_cols, file) != NULL) {
            char *last_char = *text + strlen(*text);
            *last_char = '\0';
            text++;
            total_rows++;
        }
        printf("Text loaded successfully\n");
        fclose(file);
    }
    return *text + strlen(*text);
}

void print_text(char **text) {
    for (int i = 0; i <= total_rows; i++) {
        printf("%s", text[i]);
    }
}

void insert_text(char *text);

void search_text(char *text);

void clear_console() {
#ifdef _WIN64
    system("cls");
#else
    system("clear");
#endif
}

int main(void) {
    char **text = allocate_2d_array(&initial_rows, &initial_cols);
    char *last_char = *text; //movable pointer to the last symbol
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
//            case 6:
//                insert_text();
//                break;
//            case 7:
//                search_text(text);
//                break;
            case 8:
                clear_console();
                break;
            case 9:
                print_help();
                break;
            case 0:
                free(text);
                exit(0);
            default:
                printf("The command is not implemented\n");
                break;
        }
    }
    return 0;
}
