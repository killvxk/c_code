typedef struct {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t length;
    int is_wide;
} dynamic_string;

dynamic_string* create_dynamic_string(int is_wide) {
    dynamic_string* str = (dynamic_string*)malloc(sizeof(dynamic_string));
    str->data = NULL;
    str->element_size = is_wide ? sizeof(wchar_t) : sizeof(char);
    str->capacity = 0;
    str->length = 0;
    str->is_wide = is_wide;
    return str;
}

void destroy_dynamic_string(dynamic_string* str) {
    free(str->data);
    free(str);
}

void resize_dynamic_string(dynamic_string* str, size_t new_capacity) {
    str->data = realloc(str->data, new_capacity * str->element_size);
    str->capacity = new_capacity;
}

void append_dynamic_string(dynamic_string* str, const void* source) {
    size_t source_length = str->is_wide ? wcslen((const wchar_t*)source) : strlen((const char*)source);
    size_t new_length = str->length + source_length;

    if (new_length >= str->capacity) {
        size_t new_capacity = str->capacity == 0 ? 1 : str->capacity * 2;
        while (new_capacity <= new_length) {
            new_capacity *= 2;
        }
        resize_dynamic_string(str, new_capacity);
    }

    void* destination = (char*)str->data + str->length * str->element_size;
    memcpy(destination, source, source_length * str->element_size);
    str->length = new_length;
}

void append_dynamic_string_n(dynamic_string* str, const void* data, size_t length) {
    size_t new_length = str->length + length;
    if (new_length > str->capacity) {
        size_t new_capacity = (new_length + 1) * 2;
        void* new_data = realloc(str->data, new_capacity * str->element_size);
        if (new_data == NULL) {
            // 处理内存分配失败的情况
            return;
        }
        str->data = new_data;
        str->capacity = new_capacity;
    }

    memcpy((char*)str->data + str->length * str->element_size, data, length * str->element_size);
    str->length = new_length;
    ((char*)str->data)[str->length * str->element_size] = '\0';
}



const void* get_dynamic_string_data(dynamic_string* str) {
    return str->data;
}

int find_substring(dynamic_string* str, const void* substring) {
    const void* str_data = str->data;
    const void* sub_data = substring;
    size_t str_len = str->length;
    size_t sub_len = str->is_wide ? wcslen((const wchar_t*)sub_data) : strlen((const char*)sub_data);
    size_t sub_size = str->is_wide ? sizeof(wchar_t) : sizeof(char);

    for (size_t i = 0; i <= str_len - sub_len; i++) {
        if (memcmp((const char*)str_data + i * sub_size, (const char*)sub_data, sub_len * sub_size) == 0) {
            return i;  // Substring found at index i
        }
    }

    return -1;  // Substring not found
}

void split_string(dynamic_string* str, const void* delimiter, size_t* num_tokens, void*** tokens) {
    void* token;
    void** temp_tokens = NULL;
    size_t count = 0;
    size_t delimiter_len = str->is_wide ? wcslen((const wchar_t*)delimiter) : strlen((const char*)delimiter);
    size_t str_len = str->length;
    size_t sub_size = str->is_wide ? sizeof(wchar_t) : sizeof(char);
    const void* str_data = str->data;

    size_t start = 0;
    size_t end = 0;
    while (end <= str_len) {
        int is_token = 1;
        if (end < str_len) {
            if (str->is_wide) {
                const wchar_t* str_wide = (const wchar_t*)str_data;
                const wchar_t* delimiter_wide = (const wchar_t*)delimiter;
                for (size_t i = 0; i < delimiter_len; i++) {
                    if (str_wide[end + i] != delimiter_wide[i]) {
                        is_token = 0;
                        break;
                    }
                }
            } else {
                const char* str_narrow = (const char*)str_data;
                const char* delimiter_narrow = (const char*)delimiter;
                for (size_t i = 0; i < delimiter_len; i++) {
                    if (str_narrow[end + i] != delimiter_narrow[i]) {
                        is_token = 0;
                        break;
                    }
                }
            }
        } else {
            is_token = 0;
        }

        if (is_token) {
            size_t token_len = end - start;
            if (token_len > 0) {
                temp_tokens = realloc(temp_tokens, (count + 1) * sizeof(void*));
                temp_tokens[count] = malloc(token_len * sub_size);
                memcpy(temp_tokens[count], (const char*)str_data + start * sub_size, token_len * sub_size);
                count++;
            }
            start = end + delimiter_len;
            end += delimiter_len - 1;
        }

        end++;
    }

    *tokens = temp_tokens;
    *num_tokens = count;
}


void replace_substring(dynamic_string* str, const void* old_substring, const void* new_substring) {
    size_t old_substring_length = 0;
    size_t new_substring_length = 0;

    if (str->is_wide) {
        const wchar_t* old_wide = (const wchar_t*)old_substring;
        const wchar_t* new_wide = (const wchar_t*)new_substring;
        while (old_wide[old_substring_length] != L'\0') {
            old_substring_length++;
        }
        while (new_wide[new_substring_length] != L'\0') {
            new_substring_length++;
        }
    } else {
        const char* old_narrow = (const char*)old_substring;
        const char* new_narrow = (const char*)new_substring;
        while (old_narrow[old_substring_length] != '\0') {
            old_substring_length++;
        }
        while (new_narrow[new_substring_length] != '\0') {
            new_substring_length++;
        }
    }

    const void* result = NULL;
    const void* str_data = str->data;
    size_t str_len = str->length;

    dynamic_string* temp_str = create_dynamic_string(str->is_wide);
    size_t start = 0;
    size_t end = 0;

    while (end <= str_len) {
        int is_match = 1;
        if (end < str_len) {
            if (str->is_wide) {
                const wchar_t* str_wide = (const wchar_t*)str_data;
                const wchar_t* old_wide = (const wchar_t*)old_substring;
                for (size_t i = 0; i < old_substring_length; i++) {
                    if (str_wide[end + i] != old_wide[i]) {
                        is_match = 0;
                        break;
                    }
                }
            } else {
                const char* str_narrow = (const char*)str_data;
                const char* old_narrow = (const char*)old_substring;
                for (size_t i = 0; i < old_substring_length; i++) {
                    if (str_narrow[end + i] != old_narrow[i]) {
                        is_match = 0;
                        break;
                    }
                }
            }
        } else {
            is_match = 0;
        }

        if (is_match) {
            // 保存当前处理的位置之前的字符串
            append_dynamic_string_n(temp_str, str_data + start, end - start);

            // 追加新的子字符串
            append_dynamic_string(temp_str, new_substring);

            start = end + old_substring_length;
            end += old_substring_length - 1;
        }

        end++;
    }

    // 追加最后一个子字符串之后的部分
    append_dynamic_string_n(temp_str, str_data + start, str_len - start);

    // 更新原始字符串
    free(str->data);  // 释放原始字符串的内存
    str->data = temp_str->data;
    str->capacity = temp_str->capacity;
    str->length = temp_str->length;

    // 释放临时字符串
    free(temp_str);
}
