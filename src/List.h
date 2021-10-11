#ifndef INCLUDE_LIST
#define INCLUDE_LIST

template <class T>
class List {
private:
    struct Node {
        T data;
        bool is_valid = true;
        Node* left = nullptr;
        Node* right = nullptr;
        Node(T object) {
            data = object;
        }
        Node* getNext() {
            return right;
        }
        Node* getPrev() {
            return left;
        }
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;
    
public:
    struct Iterator
    {
    private:
        Node* node;
    public:
        Iterator(){};
        Iterator(Node* node) : node(node) {
            // printf("created iterator with node %p\n", node);
        }
        const Iterator& operator ++ () {
            // printf("Itertor operation++: node is %p\n", node);
            // printf("Itertor operation++: right is %p\n", node->right);
            node = node->right;
            return *this;
        }
        const Iterator& operator -- () {
            node = node->left;
            return *this;
        }
        bool isValid() {
            // printf("valid check: node %p\n", node);
            if (node == nullptr){
                return false;
            }
            return node->is_valid;
        }
        Node* getNode() {
            return node;
        }
        Iterator operator = (const Iterator& it) {
            node = it.node;
            return *this;
        }

        Iterator operator + (int cnt) {
            Iterator new_it = Iterator(node);
            for (int i = 0; i < cnt; ++i) {
                if (new_it.node->right != nullptr) {
                    new_it.node = new_it.node->right;
                } else {
                    new_it.node = nullptr;
                    break;
                }
            }
            return new_it;
        }
    };
    
    void add(const T& elem) {
        if (size == 0) {
            // printf("bruh\n");
            head = new Node(elem);
            tail = head;
            // printf("%p\n", tail);
            size++;
            return;
        }
        size++;
        tail->right = new Node(elem);
        // printf("%p\n", tail->right);
        tail->right->left = tail;
        tail = tail->right;
    }

    void erase(Iterator& it) {
        it.getNode()->is_valid = false;
    }

    Iterator begin() {
        return Iterator{head};
    }


    void clear() {
        Node* cur_node = head;
        while (cur_node != nullptr) {
            if (!(cur_node->is_valid)) {
                if (cur_node->left != nullptr) {
                    cur_node->left->right = cur_node->right;
                } else {
                    head = cur_node->right;
                }
                if (cur_node->right != nullptr) {
                    cur_node->right->left = cur_node->left;
                } else {
                    tail = tail->left;
                }
                Node* old_node = cur_node;
                cur_node = cur_node->right;
                delete old_node;
                --size;
            } else {
                cur_node = cur_node->right;
            }
        }
    }

    int getSize() const {
        return size;
    }
};

#endif