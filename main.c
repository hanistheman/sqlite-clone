#import <stdlib.h>
#import <stdio.h>
#import <string.h> // strcmp
#import <stdbool.h> // for true and false keyword

struct InputBuffer_t {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
};
typedef struct InputBuffer_t InputBuffer;
// saves unnecessary & repetitive use of struct keyword

InputBuffer* create_new_buffer() {
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer -> buffer = NULL;
    input_buffer -> buffer_length = 0;
    input_buffer -> input_length = 0;

    return input_buffer;
}
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;
typedef enum {
    PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer -> buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}
// Enum for meta command success/failure states
typedef enum {
    STATEMENT_INSERT, STATEMENT_SELECT
} StatementType;
typedef struct {
    StatementType type;
} Statement;
// enum for statement
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer -> buffer, "insert", 6) == 0) {
        statement -> type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer -> buffer, "select") == 0) {
        statement -> type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}
// prepare_statement method
void execute_statement(Statement* statement) {
    switch (statement -> type) {
        case(STATEMENT_INSERT):
            printf("This is where we would do an insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("This is where we would do a select. \n");
            break;
    }
}
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer -> buffer);
    free(input_buffer);
}

void print_prompt() {
    printf("db > ");
}

void read_input (InputBuffer* input_buffer) {
    ssize_t bytes_read =
            getline(&(input_buffer -> buffer), &(input_buffer -> buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error readng input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer -> buffer[bytes_read - 1] = 0;
    input_buffer -> input_length = bytes_read - 1;
}

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = create_new_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer -> buffer[0] == '.') {
            switch(do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer -> buffer);
                    continue;
            }
        }

        Statement statement;
        switch(prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n",
                       input_buffer -> buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
    return 0;
}