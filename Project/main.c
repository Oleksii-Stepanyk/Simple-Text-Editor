#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char *start_newline(char *text) {
    *text = '\n';
    return text + 1;
}

void save_file(char *text) {
    FILE *file;
    char file_name[32];
    printf("Enter the file name for saving:");
    scanf("%s", file_name);
    file = fopen(file_name, "w");
    if (file != NULL) {
        fputs(text, file);
        fclose(file);
        printf("Text saved successfully\n");
    }
}

void load_file(char *text) {
    FILE *file;
    char file_name[32];
    printf("Enter the file name for loading:");
    scanf("%s", file_name);
    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file");
    } else {
        while (fgets(text, 100, file) != NULL) {
            text = text + strlen(text);
        }
        printf("Text loaded successfully\n");
        fclose(file);
    }
}

void print_text(char *text){
    printf("%s\n", text);
}

void insert_text();

void search_text();

void clear_console(){
#ifdef _WIN64
    system("cls");
#else
    system("clear");
#endif
}

int main(void) {
    char *text = malloc(100 * sizeof(char));
    char *last_char = text; //movable pointer to the last_char symbol
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
                save_file(text);
                break;
            case 4:
                load_file(text);
                break;
            case 5:
                print_text(text);
                break;
//            case 6:
//                insert_text();
//                break;
//            case 7:
//                search_text();
//                break;
            case 8:
                clear_console();
                break;
            case 9:
                print_help();
                break;
            case 0:
                exit(0);
            default:
                printf("The command is not implemented\n");
                break;
        }
    }
    return 0;
}
