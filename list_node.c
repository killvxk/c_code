typedef struct list_node {
    void* data;
    struct list_node* next;
} list_node_t;

typedef struct {
    list_node_t* head;
    list_node_t* tail;
    size_t size;
} linked_list_t;

linked_list_t* create_linked_list() {
    linked_list_t* list = malloc(sizeof(linked_list_t));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void destroy_linked_list(linked_list_t* list) {
    list_node_t* current = list->head;
    while (current != NULL) {
        list_node_t* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

void linked_list_add(linked_list_t* list, void* data) {
    list_node_t* new_node = malloc(sizeof(list_node_t));
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
}

void* linked_list_get(linked_list_t* list, size_t index) {
    if (index >= list->size) {
        return NULL;
    }

    list_node_t* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

void linked_list_remove(linked_list_t* list, size_t index) {
    if (index >= list->size) {
        return;
    }

    list_node_t* current = list->head;
    list_node_t* previous = NULL;

    for (size_t i = 0; i < index; i++) {
        previous = current;
        current = current->next;
    }

    if (previous != NULL) {
        previous->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current == list->tail) {
        list->tail = previous;
    }

    free(current->data);
    free(current);
    list->size--;
}

void linked_list_insert_after(linked_list_t* list, size_t index, void* data) {
    if (index >= list->size) {
        return;
    }

    list_node_t* current = list->head;

    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    list_node_t* new_node = malloc(sizeof(list_node_t));
    new_node->data = data;
    new_node->next = current->next;

    current->next = new_node;

    if (current == list->tail) {
        list->tail = new_node;
    }

    list->size++;
}

void linked_list_traverse(linked_list_t* list, void (*callback)(void*)) {
    list_node_t* current = list->head;
    while (current != NULL) {
        callback(current->data);
        current = current->next;
    }
}
#if defined(TEST)
// 自定义回调函数示例：打印整数
void print_int(void* data) {
    printf("%d ", *(int*)data);
}

int main() {
    linked_list_t* list = create_linked_list();

    int* data1 = malloc(sizeof(int));
    *data1 = 10;
    linked_list_add(list, data1);

    char* data2 = malloc(sizeof(char));
    *data2 = 'A';
    linked_list_add(list, data2);

    float* data3 = malloc(sizeof(float));
    *data3 = 3.14;
    linked_list_add(list, data3);

    linked_list_traverse(list, print_int);
    printf("\n");

    linked_list_remove(list, 1);

    int* data4 = malloc(sizeof(int));
    *data4 = 20;
    linked_list_insert_after(list, 0, data4);

    linked_list_traverse(list, print_int);
    printf("\n");

    destroy_linked_list(list);

    return 0;
}
#endif

