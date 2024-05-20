#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define command1 77622
#define command2 77623
#define command3 77624
#define command4 77625
#define command5 77626
#define command6 77627
#define command7 77628
#define help 41629
#define prog_exit 54414

int hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return (int) (hash % 100000);
}

void command_1(char user_text[]){
    char input[32];
    printf("Enter text to append: \n");
    scanf("%s", input);
}

int main(void) {
    char *user_text = (char *) malloc(128*sizeof(char));
    while (true) {
        char input[6];
        int last = 0;
        printf("Choose the command:\n");
        scanf("%s", &input[6]);
        int input_hash = hash(&input[6]);
        switch (input_hash) {
            case command1:
                command_1(user_text);
                break;
            case command2:
                printf("2\n");
                break;
            case command3:
                printf("3\n");
                break;
            case command4:
                printf("4\n");
                break;
            case command5:
                printf("5\n");
                break;
            case command6:
                printf("6\n");
                break;
            case command7:
                printf("7\n");
                break;
            case help:
                printf("Manual...\n");
                break;
            case prog_exit:
                exit(0);
            default:
                printf("The command is not implemented\n");
                break;
        }
    }
    return 0;
}
