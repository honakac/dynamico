#include "../dynamico.h"
#include <stdio.h>

int main()
{
    Vector str = vector_create(sizeof(char));                        // Initialize a string vector
    vector_push_multiple(&str, "Hello, ", strlen("Hello, ")); // Push multiple characters

    printf("What's your name?: ");

    for (char ch; (ch = getchar()) != '\n';)
        vector_push(&str, &ch);                           // Push characters until newline

    vector_push_multiple(&str, "!", 1);            // Push exclamation mark
    
    printf("%s\n", vector_buffer(&str, char));

    vector_free(&str);                                        // Free the vector

    return 0;
}