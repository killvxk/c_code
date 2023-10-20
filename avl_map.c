
typedef struct avl_node {
    void* key;
    void* value;
    int height;
    struct avl_node* left;
    struct avl_node* right;
    int (*compare)(const void*, const void*);
} avl_node_t;

avl_node_t* create_node(void* key, void* value, int (*compare)(const void*, const void*)) {
    avl_node_t* node = (avl_node_t*)malloc(sizeof(avl_node_t));
    node->key = key;
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->compare = compare;
    return node;
}

int get_height(avl_node_t* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int get_balance_factor(avl_node_t* node) {
    if (node == NULL) {
        return 0;
    }
    return get_height(node->left) - get_height(node->right);
}

avl_node_t* get_min_node(avl_node_t* node) {
    avl_node_t* current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}


void update_height(avl_node_t* node) {
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    node->height = (left_height > right_height ? left_height : right_height) + 1;
}

avl_node_t* rotate_left(avl_node_t* node) {
    avl_node_t* right_child = node->right;
    node->right = right_child->left;
    right_child->left = node;
    update_height(node);
    update_height(right_child);
    return right_child;
}

avl_node_t* rotate_right(avl_node_t* node) {
    avl_node_t* left_child = node->left;
    node->left = left_child->right;
    left_child->right = node;
    update_height(node);
    update_height(left_child);
    return left_child;
}

avl_node_t* balance_node(avl_node_t* node) {
    update_height(node);
    int balance_factor = get_balance_factor(node);

    if (balance_factor > 1) {
        if (get_balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if (balance_factor < -1) {
        if (get_balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

avl_node_t* insert_node(avl_node_t* root, void* key, void* value) {
    if (root == NULL) {
        return create_node(key, value, root->compare);
    }

    int cmp = root->compare(key, root->key);
    if (cmp < 0) {
        root->left = insert_node(root->left, key, value);
    } else if (cmp > 0) {
        root->right = insert_node(root->right, key, value);
    } else {
        root->value = value;
        return root;
    }

    return balance_node(root);
}

void* find_value(avl_node_t* root, void* key) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = root->compare(key, root->key);
    if (cmp < 0) {
        return find_value(root->left, key);
    } else if (cmp > 0) {
        return find_value(root->right, key);
    } else {
        return root->value;
    }
}

avl_node_t* delete_node(avl_node_t* root, void* key) {
    if (root == NULL) {
        return root;
    }

    int cmp = root->compare(key, root->key);
    if (cmp < 0) {
        root->left = delete_node(root->left, key);
    } else if (cmp > 0) {
        root->right = delete_node(root->right, key);
    } else {
        // Node found, perform deletion
        if (root->left == NULL || root->right == NULL) {
            avl_node_t* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            avl_node_t* temp = get_min_node(root->right);
            root->key = temp->key;
            root->value = temp->value;
            root->right = delete_node(root->right, temp->key);
        }
    }

    if (root == NULL) {
        return root;
    }

    return balance_node(root);
}



void destroy_node(avl_node_t* node) {
    if (node == NULL) {
        return;
    }

    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
    node = NULL;
}

