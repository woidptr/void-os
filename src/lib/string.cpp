#include "string.hpp"

string::string(struct heap_ctx* heap, const char* text) {
    if (!text) {
        this->length = 0;
        this->capacity = 1;
        this->buffer = (char*)heap_alloc(heap, this->capacity);
        if (this->buffer) {
            this->buffer[0] = '\0';
        }
        return;
    }

    this->capacity = this->length + 1;
    this->buffer = new char[capacity];

    for (size_t i = 0; i < length; i++) {
        buffer[i] = text[i];
    }

    this->buffer[this->length] = '\0';
}

string::~string() {
    // if (this->buffer != nullptr) {
    //     delete[] this->buffer;

    //     this->buffer = nullptr;
    //     this->length = 0;
    //     this->capacity = 0;
    // }
}

const char* string::c_str() {

}