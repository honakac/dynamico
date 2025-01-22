#include "dynamico.h"
#include <stdio.h>

int main()
{
    string(str);
    string_init(str);

    vector_fill(str, 'a', 100*1024*1024);
    printf("Data Increased: Size = %ld, Position = %ld\n", str.size, str.pos);

    size_t size = string_size(str);
    for (size_t i = 0; i < size; i++)
        string_pop(str);
    printf("Data Decreased: Size = %ld, Position = %ld\n", str.size, str.pos);

    return 0;
}