#include "dynamico.h"
#include <stdio.h>

int main()
{
    string(str);
    string_init(str);

    string_push_string(str, "Test message");
    
    printf("Char: %c\n", string_at(str, char, 5));


    vector(char*, piska);
    vector_init(piska);
    vector_push_back(piska, "Pisk");
    printf("String: %s\n", vector_at(piska, char*, 0));
    printf("Error String: %s\n", vector_at(piska, char*, 1));

    return 0;
}