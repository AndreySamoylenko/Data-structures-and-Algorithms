// ------------------ двусвязный сортированный список ----------------------
#include <cstddef> // Для std::size_t

struct list_node
{
    size_t data;
    list_node *next;
    list_node *previous;
};

class List
{
private:
    list_node *head;

public:
    List() {}
    ~List()
    {
        if (!head)
            return;

        head->previous->next = nullptr; // разорвали закольцованность
        while (head->next)
        {
            head->previous = nullptr;
            head = head->next;
            delete head->previous;
        }
        head->previous = nullptr;

        delete head;
    }

    void add(size_t data)
    {
        list_node *new_Node = new list_node;
        new_Node->data = data;

        if (!head) // если добавленный элемент первый в списке
        {
            new_Node->next = new_Node;
            new_Node->previous = new_Node;
            head = new_Node;
            return;
        }

        list_node *current = head;
        if (data >= current->data)
            head = new_Node;

        do
            current = current->next;
        while (data < current->data and current != head);

        current->previous->next = new_Node;
        new_Node->previous = current->previous;
        current->previous = new_Node;
        new_Node->next = current;
    }
    void remove(size_t data)
    {
        if (!head)
            return;

        list_node *true_first = head;

        do
        {
            if (head->data == data && head != head->next)
            {
                head->previous->next = head->next;
                head = head->next;
                list_node *buffer = head->previous->previous;
                delete head->previous;
                head->previous = buffer;
            }
            else
                head = head->next;
        } while (head != true_first);

        if (head == head->next and head->data == data)
        {
            true_first = nullptr;
            delete head;
        }

        head = true_first;
    }
    bool find_value(size_t value)
    {
        if (!head) // если список пуст
            return 0;
        if (head->data < value) // если все значения списка меньше искомого
            return 0;
        if (head->previous->data > value) // если все значения списка больше искомого
            return 0;

        list_node *current = head;
        do
        {
            if (current->data == value)
                return 1;
            current = current->next;
        } while (current != head);

        return 0;
    }
};

#include <DataManager.hpp>

void DataManager::remove(const Key &key)
{
    size_t index = indexed_struct.remove(key);
}

class RBtree : public IndexedStructure
{
private:
    template <class T>
    struct Node
    {
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        Key key;
        T data;
        bool red = true; // новый узел — красный
    };

    using N = Node<List *>;
    N *root = nullptr;

    bool is_red(N *x) const { return x && x->red; }
    bool is_black(N *x) const { return !is_red(x); }

    void rotate_left(N *x)
    {
        N *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }
    void rotate_right(N *y)
    {
        N *x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;

        x->parent = y->parent;
        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void insert_fixup(N *z)
    {
        // пока родитель красный — нарушено свойство
        while (z->parent && z->parent->red)
        {
            if (z->parent == z->parent->parent->left)
            {
                N *y = z->parent->parent->right; // дядя
                if (is_red(y))
                {
                    // Case 1: дядя красный
                    z->parent->red = false;
                    y->red = false;
                    z->parent->parent->red = true;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        // Case 2: треугольник
                        z = z->parent;
                        rotate_left(z);
                    }
                    // Case 3: линия
                    z->parent->red = false;
                    z->parent->parent->red = true;
                    rotate_right(z->parent->parent);
                }
            }
            else
            {
                // симметрично
                N *y = z->parent->parent->left;
                if (is_red(y))
                {
                    z->parent->red = false;
                    y->red = false;
                    z->parent->parent->red = true;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rotate_right(z);
                    }
                    z->parent->red = false;
                    z->parent->parent->red = true;
                    rotate_left(z->parent->parent);
                }
            }
        }
        root->red = false; // корень всегда чёрный
    }

    N *BST_insert(const PersonalData &record)
    {
        Key key = record.key;

        N *y = nullptr;
        N *x = root;

        while (x)
        {
            y = x;
            if (key < x->key)
                x = x->left;
            else if (key < x->key)
                x = x->right;
            else
                break;
        }

        if (y == x)
        {
            x->data->add(record.array_index);
            return root;
        }

        N *z = new N{};
        z->key = record.key;
        z->data->add(record.array_index);

        z->parent = y;
        if (!y)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        return z;
    }

    N *find_node(const Key &key) const
    {
        N *cur = root;
        while (cur)
        {
            if (key < cur->key)
                cur = cur->left;
            else if (cur->key < key)
                cur = cur->right;
            else
                return cur; // равны
        }
        return nullptr;
    }

    void transplant(N *u, N *v)
    {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    N *minimum(N *x) const
    {
        while (x->left)
            x = x->left;
        return x;
    }

    void delete_fixup(N *x, N *xParent)
    {
        while ((x != root) && is_black(x))
        {
            if (x == (xParent ? xParent->left : nullptr))
            {
                N *w = xParent->right;
                if (is_red(w))
                {
                    w->red = false;
                    xParent->red = true;
                    rotate_left(xParent);
                    w = xParent->right;
                }
                if (is_black(w->left) && is_black(w->right))
                {
                    w->red = true;
                    x = xParent;
                    xParent = x->parent;
                }
                else
                {
                    if (is_black(w->right))
                    {
                        if (w->left)
                            w->left->red = false;
                        w->red = true;
                        rotate_right(w);
                        w = xParent->right;
                    }
                    w->red = xParent->red;
                    xParent->red = false;
                    if (w->right)
                        w->right->red = false;
                    rotate_left(xParent);
                    x = root;
                    break;
                }
            }
            else
            {
                N *w = xParent->left;
                if (is_red(w))
                {
                    w->red = false;
                    xParent->red = true;
                    rotate_right(xParent);
                    w = xParent->left;
                }
                if (is_black(w->right) && is_black(w->left))
                {
                    w->red = true;
                    x = xParent;
                    xParent = x->parent;
                }
                else
                {
                    if (is_black(w->left))
                    {
                        if (w->right)
                            w->right->red = false;
                        w->red = true;
                        rotate_left(w);
                        w = xParent->left;
                    }
                    w->red = xParent->red;
                    xParent->red = false;
                    if (w->left)
                        w->left->red = false;
                    rotate_right(xParent);
                    x = root;
                    break;
                }
            }
        }
        if (x)
            x->red = false;
    }

public:
    RBtree() = default;

    void add(const PersonalData &record) override
    {
        N *z = BST_insert(record);
        if (z)
            insert_fixup(z);
    }

    List *find(const Key &key) const
    {
        return (find_node(key)->data);
    }

    void remove(const Key &key) override
    {
        N *z = find_node(key);
        if (!z)
            return;

        N *y = z;
        bool yOriginalRed = y->red;
        N *x = nullptr;
        N *xParent = nullptr;

        if (!z->left)
        {
            x = z->right;
            xParent = z->parent;
            transplant(z, z->right);
        }
        else if (!z->right)
        {
            x = z->left;
            xParent = z->parent;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            yOriginalRed = y->red;
            x = y->right;
            if (y->parent == z)
            {
                if (x)
                    x->parent = y;
                xParent = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
                xParent = y->parent;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->red = z->red;
        }

        delete z;

        if (!yOriginalRed)
        {
            delete_fixup(x, xParent);
        }
    }

    void update(const PersonalData &old_data, const PersonalData &new_data) override
    {
        Key old_key = old_data.key;
        N *n = find_node(old_key);
        if (!n)
            return;
        if (!(n->data->find_value(old_data.array_index)))
            return;

        remove(old_key);
        add(new_data);
    }
};

class Array : public Repository
{
private:
    PersonalData array[1000];
    size_t a_size = 0;
    size_t a_capacity = 1000;

public:
    void add(PersonalData &record) override
    {
        if (a_size == a_capacity)
            return;

        array[a_size] = record;
        record.array_index = a_size;
        a_size++;
    }
    void remove(const Key &key)
    {
        for (int index = 0; index < a_size; index++)
        {
            if (array[index].key == key)
            {
                array[index] = array[a_size - 1];
                a_size--;
                return;
            }
        }
    }
    void update(const PersonalData &old_data, const PersonalData &new_data)
    {
        for (int index = 0; index < a_size; index++)
        {
            if (array[index] == old_data)
            {
                array[index] = new_data;
                return;
            }
        }
    }
};