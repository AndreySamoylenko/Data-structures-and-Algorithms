#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <stack>

// ------------------ двусвязный сортированный список ----------------------

// ------------------ красно-чёрное дерево ----------------------
// обход справа налево
// при удалении максимальный справа
// ключ — дата + фио
template <typename Key>
class RBtree
{
private:
    struct Node
    {
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        Key key;
        bool red = true; // новый узел — красный
    };

    using N = Node;
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

    N *BST_insert(const Key &key)
    {
        // std::cout << "inserting " << key << std::endl;

        N *y = nullptr;
        N *x = root;
        // доходим до места вставки
        while (x)
        {
            y = x;
            // std::cout << "traversing " << ((x->key < key) ? "right" : "left") << std::endl;
            if (key < x->key)
            {
                x = x->left;
            }
            else if (key > x->key)
            {
                x = x->right;
            }
            else if (x->key == key)
            { // если ключ уже есть, то просто добавляем индекс в список узла
                // std::cout << "adding index to existing key " << key << std::endl;
                return nullptr;
            }
        }
        // std::cout << "traversed to insert place succsesfully\n";

        // вставляем новый узел
        N *z = new N;
        z->key = key;

        // std::cout << z->data << std::endl;
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

    N *maximum(N *x) const
    {
        while (x->right)
            x = x->right;
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

    // обход дерева справа налево
    void print_ordered(N *node) const
    {
        if (!node)
        {
            // std::cout << "null\n";
            return;
        }
        else
        {
            print_ordered(node->right);
            std::cout << node->key << std::endl;
            print_ordered(node->left);
        }
    }

    // печать структуры дерева
    void print_structured(N *node, int indent = 0) const
    {
        if (node)
        {
            print_structured(node->right, indent + 4);
            std::cout << std::string(indent, ' ') << (node->red ? "R" : "B") << ": " << node->key << std::endl;
            print_structured(node->left, indent + 4);
        }
        else
        {
            std::cout << std::string(indent, ' ') << "null\n";
        }
    }

public:
    RBtree() = default;
    ~RBtree()
    {
        if (!root)
            return;

        std::stack<N *> s;
        s.push(root);
        while (!s.empty())
        {
            N *current = s.top();
            s.pop();
            if (current->left)
                s.push(current->left);
            if (current->right)
                s.push(current->right);
            delete current;
        }
    }

    bool empty() const { return root == nullptr; }

    void add(const Key &key) 
    {
        // std::cout << record << std::endl;
        N *z = BST_insert(key);
        if (z)
            insert_fixup(z);
    }


    void remove(const Key &key) 
    {

        N *z = find_node(key);
        // если узла с таким ключом нет, то удалять нечего
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
            y = maximum(z->left);
            yOriginalRed = y->red;
            x = y->left;
            if (y->parent == z)
            {
                if (x)
                    x->parent = y;
                xParent = y;
            }
            else
            {
                transplant(y, y->left);
                y->left = z->left;
                y->left->parent = y;
                xParent = y->parent;
            }
            transplant(z, y);
            y->right = z->right;
            y->right->parent = y;
            y->red = z->red;
        }

        delete z;

        if (!yOriginalRed)
        {
            delete_fixup(x, xParent);
        }
    }

    void print_in_order() const
    {
        print_ordered(root);
    }

    void print_structure() const
    {
        print_structured(root);
    }
};

