#include "dynamico.h"
#include <stdio.h>

int main()
{
    string(str);
    string_init(str);
    string_push_string(str, "Hello, ");

    printf("What's your name?: ");

    char ch;
    while ((ch = getchar()) != '\n')
        string_push(str, ch);
    string_push(str, '!');
    
    printf("%s\n", str.buffer);

    return 0;
}