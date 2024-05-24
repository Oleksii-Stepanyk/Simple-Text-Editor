#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char **text; //const but not totally
int array_rows = 10; //initial value
int array_cols = 128; //initial value
int total_rows = 0; //of text

char **allocate_array(const int *rows, const int *cols) {
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

char **reallocate_rows(const int *old_rows, const int *new_rows, const int *cols, char **array) {
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
    array_rows = *new_rows;
    return array;
}

char **reallocate_cols(const int *new_cols, char **array) {
    for (int i = 0; i < array_rows; i++) {
        array[i] = realloc(array[i], *new_cols * sizeof(char));
        if (array[i] == NULL) {
            perror("Failed to allocate memory");
            return NULL;
        }
    }
    array_cols = *new_cols;
    return array;
}

void shift_right(const int *row, const int *start_index, const int *shift_amount) {
    if (*start_index + *shift_amount >= array_cols) {
        int new_cols = array_cols + 128;
        text = reallocate_cols(&new_cols, text);
    }
    for (int i = array_cols; i >= *start_index; i--) {
        text[*row][i + *shift_amount] = text[*row][i];
    }
    text[*row][strlen(text[*row]) + 1] = '\0';
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

char *append_text(char *last_char) {
    char buffer[256];
    printf("Enter text to append:\n");
    getchar();
    fgets(buffer, 256, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(text[total_rows]) + strlen(buffer) >= array_cols) {
        int new_cols = array_cols + 128;
        text = reallocate_cols(&new_cols, text);
        last_char = strstr(text[total_rows], "\0");
        strcat(last_char, buffer);
    } else {
        strcat(last_char, buffer);
    }
    return last_char + strlen(buffer);
}

char *start_newline(char *last_char) {
    *last_char = '\n';
    last_char++;
    *last_char = '\0';
    if (total_rows + 1 >= array_rows) {
        int new_rows = array_rows + 10;
        text = reallocate_rows(&array_rows, &new_rows, &array_cols, text);
    }
    total_rows++;
    *text[total_rows] = '\0';
    return text[total_rows];
}

void save_file() {
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

char *load_file() {
    FILE *file;
    char file_name[32];
    char buffer[128];
    int rows = 0;
    printf("Enter the file name for loading:");
    scanf("%s", file_name);
    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file");
        return NULL;
    }
    while (fgets(buffer, 128, file) != NULL) {
        if (strlen(text[rows]) + strlen(buffer) >= array_cols) {
            int new_cols = array_cols + 128;
            text = reallocate_cols(&new_cols, text);
        }
        strcat(text[rows], buffer);

        if (strchr(text[rows], '\n') != NULL) {
            if (total_rows + 1 >= array_rows) {
                int new_rows = array_rows + 10;
                text = reallocate_rows(&array_rows, &new_rows, &array_cols, text);
            }
            text[rows + 1] = start_newline(text[rows] + strlen(text[rows]));
            text[rows][strlen(text[rows]) - 1] = '\0';
            rows++;
        }
    }
    fclose(file);
    return text[rows] + strlen(text[rows]);
}

void print_text() {
    for (int i = 0; i <= total_rows; i++) {
        printf("%s", text[i]);
    }
    printf("\n");
}

void insert_text() {
    int row, col;
    char entered_text[64];
    printf("Enter the row and column to insert text:");
    scanf("%d %d", &row, &col);
    if (row > array_rows) {
        int new_rows = array_rows + 10;
        text = reallocate_rows(&array_rows, &new_rows, &array_cols, text);
    }
    char **position = text;
    if (row > total_rows) {
        for (int i = total_rows + 1; i <= row; i++) {
            position[i] = start_newline(text[total_rows] + strlen(text[total_rows]));
        }
    }
    printf("Enter text to insert:");
    getchar();
    fgets(entered_text, 64, stdin);
    entered_text[strcspn(entered_text, "\n")] = '\0';
    int shift_amount = (int)strlen(entered_text);
    shift_right(&row, &col, &shift_amount);
    for (int i = 0; i < shift_amount; i++) {
        text[row][col + i] = entered_text[i];
    }
}

void search_text() {
    char search_text[64];
    char *position;
    bool found = false;
    int index;
    printf("Enter text you want to find:");
    getchar();
    fgets(search_text, 64, stdin);
    search_text[strcspn(search_text, "\n")] = '\0';
    printf("Text found in:");
    for (int i = 0; i <= total_rows; ++i) {
        position = text[i];
        while ((position = strstr(position, search_text)) != NULL) {
            index = (int)(position - text[i]);
            printf("%d %d; ", i, index);
            position += strlen(search_text);
            found = true;
        }
    }
    printf("\n");
    if (!found) {
        printf("Text not found\n");
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
    text = allocate_array(&array_rows, &array_cols);
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
                last_char = start_newline(last_char);
                break;
            case 3:
                save_file();
                break;
            case 4:
                last_char = load_file();
                break;
            case 5:
                print_text();
                break;
            case 6:
                insert_text();
                break;
            case 7:
                search_text();
                break;
            case 8:
                clear_console();
                break;
            case 9:
                print_help();
                break;
            case 0:
                for (int i = 0; i < array_rows; ++i) {
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
