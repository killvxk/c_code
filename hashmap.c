#define HASHMAP_INITIAL_CAPACITY 16

typedef struct {
    void* key;
    void* value;
} hashmap_entry;

typedef struct {
    hashmap_entry** entries;
    size_t capacity;
    size_t size;
    int (*match)(const void*, const void*);
    unsigned long (*hash)(const void*);
} hashmap;

hashmap* create_hashmap(int (*match)(const void*, const void*),
                        unsigned long (*hash)(const void*))
{
    hashmap* hm = malloc(sizeof(hashmap));
    hm->entries = malloc(sizeof(hashmap_entry*) * HASHMAP_INITIAL_CAPACITY);
    hm->capacity = HASHMAP_INITIAL_CAPACITY;
    hm->size = 0;
    hm->match = match;
    hm->hash = hash;

    for (size_t i = 0; i < hm->capacity; i++) {
        hm->entries[i] = NULL;
    }

    return hm;
}

void destroy_hashmap(hashmap* hm)
{
    for (size_t i = 0; i < hm->capacity; i++) {
        hashmap_entry* entry = hm->entries[i];
        if (entry != NULL) {
            free(entry->key);
            free(entry->value);
            free(entry);
        }
    }

    free(hm->entries);
    free(hm);
}

void hashmap_put(hashmap* hm, const void* key, const void* value)
{
    unsigned long hash = hm->hash(key);
    size_t index = hash % hm->capacity;

    hashmap_entry* entry = hm->entries[index];
    while (entry != NULL) {
        if (hm->match(entry->key, key)) {
            free(entry->value);
            entry->value = malloc(sizeof(value));
            memcpy(entry->value, value, sizeof(value));
            return;
        }
        index = (index + 1) % hm->capacity;
        entry = hm->entries[index];
    }

    entry = malloc(sizeof(hashmap_entry));
    entry->key = malloc(sizeof(key));
    memcpy(entry->key, key, sizeof(key));
    entry->value = malloc(sizeof(value));
    memcpy(entry->value, value, sizeof(value));

    hm->entries[index] = entry;
    hm->size++;

    if (hm->size >= hm->capacity / 2) {
        size_t new_capacity = hm->capacity * 2;
        hm->entries = realloc(hm->entries, sizeof(hashmap_entry*) * new_capacity);
        memset(hm->entries + hm->capacity, 0, sizeof(hashmap_entry*) * (new_capacity - hm->capacity));
        hm->capacity = new_capacity;
    }
}

void* hashmap_get(hashmap* hm, const void* key)
{
    unsigned long hash = hm->hash(key);
    size_t index = hash % hm->capacity;

    hashmap_entry* entry = hm->entries[index];
    while (entry != NULL) {
        if (hm->match(entry->key, key)) {
            return entry->value;
        }
        index = (index + 1) % hm->capacity;
        entry = hm->entries[index];
    }

    return NULL; // Key not found
}

#if defined(TEST)
// 自定义匹配函数示例：比较两个整数是否相等
int int_match(const void* key1, const void* key2)
{
    return *(int*)key1 == *(int*)key2;
}

// 自定义哈希函数示例：将整数转换为哈希值
unsigned long int_hash(const void* key)
{
    return *(int*)key;
}

int main()
{
    hashmap* hm = create_hashmap(int_match, int_hash);

    int key1 = 10;
    int value1 = 100;
    hashmap_put(hm, &key1, &value1);

    int key2 = 20;
    int value2 = 200;
    hashmap_put(hm, &key2, &value2);

    int* retrieved_value1 = hashmap_get(hm, &key1);
    if (retrieved_value1 != NULL) {
        printf(“Value for key1: %d\n“, *retrieved_value1);
    }

    int* retrieved_value2 = hashmap_get(hm, &key2);
    if (retrieved_value2 != NULL) {
        printf(“Value for key2: %d\n“, *retrieved_value2);
    }

    destroy_hashmap(hm);

    return 0;
}
#endif
