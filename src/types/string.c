#include "string.h"

void str_append(string_t* str, const char* text) {
    size_t i = 0;
    while (text[i] != '\0' && str->length < str->capacity - 1) {
        str->data[str->length] = text[i];
        str->length++;
        i++;
    }
    str->data[str->length] = '\0';
}