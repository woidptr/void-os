#include "string.h"
#include "../memory.h"

string_t strnew(const char* text) {
    string_t str;

    str.length = strlen(text);

    if (str.length < 15) {
        str.capacity = 16;
    } else {
        str.capacity = str.length + 1;
    }

    str.data = (char*)malloc(str.capacity);

    if (str.data == nullptr) {
        str.length = 0;
        str.capacity = 0;
        return str;
    }

    memcpy(str.data, text, str.length);

    str.data[str.length] = '\0';

    return str;
}

size_t strlen(const char* text) {
    size_t length = 0;

    while (text[length] != '\0') {
        length++;
    }

    return length;
}

void str_append(string_t* str, const char* text) {
    size_t i = 0;
    while (text[i] != '\0' && str->length < str->capacity - 1) {
        str->data[str->length] = text[i];
        str->length++;
        i++;
    }
    str->data[str->length] = '\0';
}