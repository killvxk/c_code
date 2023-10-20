typedef struct {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
} dynamic_array;

dynamic_array* create_dynamic_array(size_t element_size) {
    dynamic_array* array = (dynamic_array*)malloc(sizeof(dynamic_array));
    array->data = NULL;
    array->element_size = element_size;
    array->capacity = 0;
    array->size = 0;
    return array;
}

void destroy_dynamic_array(dynamic_array* array) {
    free(array->data);
    free(array);
}

void resize_dynamic_array(dynamic_array* array, size_t new_capacity) {
    array->data = realloc(array->data, new_capacity * array->element_size);
    array->capacity = new_capacity;
}

void push_back_dynamic_array(dynamic_array* array, void* element) {
    if (array->size == array->capacity) {
        size_t new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        resize_dynamic_array(array, new_capacity);
    }
    void* destination = (char*)array->data + array->size * array->element_size;
    memcpy(destination, element, array->element_size);
    array->size++;
}

void* get_dynamic_array_element(dynamic_array* array, size_t index) {
    if (index >= array->size) {
        return NULL;
    }
    return (char*)array->data + index * array->element_size;
}
#if defined(TEST)
int main() {
    // 创建一个存储整数的动态数组
    dynamic_array* int_array = create_dynamic_array(sizeof(int));

    // 添加元素到动态数组
    int element1 = 10;
    push_back_dynamic_array(int_array, &amp;element1);

    int element2 = 20;
    push_back_dynamic_array(int_array, &amp;element2);

    // 获取动态数组的元素
    int* retrieved_element1 = (int*)get_dynamic_array_element(int_array, 0);
    if (retrieved_element1 != NULL) {
        printf(“Element at index 0: %d\n“, *retrieved_element1);
    }

    int* retrieved_element2 = (int*)get_dynamic_array_element(int_array, 1);
    if (retrieved_element2 != NULL) {
        printf(“Element at index 1: %d\n“, *retrieved_element2);
    }

    // 销毁动态数组
    destroy_dynamic_array(int_array);

    return 0;
}
#endif

