#pragma once
/*
copy and pasted (mostly) verbatim from geeks for geeks. Simple one class templated splay tree solution
   * https://www.geeksforgeeks.org/splay-tree-set-1-insert/ // rightRotate, leftRotate, splay, preOrder, and search functions
   * https://www.geeksforgeeks.org/splay-tree-set-2-insert-delete/ // insert function
   * https://www.geeksforgeeks.org/splay-tree-set-3-delete/ // remove/delete
@author GeeksForGeeks, David Wadsworth
*/

/* Splay tree is also self-balancing BST.
The main idea of splay tree is to bring the recently accessed item to root of the tree, this makes the recently searched item to be accessible in O(1) time if accessed again.
The idea is to use locality of reference (In a typical application, 80% of the access are to 20% of the items).
Imagine a situation where we have millions or billions of keys and only few of them are accessed frequently, which is very likely in many practical applications.
All splay tree operations run in O(log n) time on average, where n is the number of entries in the tree. Any single operation can take Theta(n) time in the worst case. */
#pragma once
#include <vector>
#include <memory>
#include <iostream>

template <typename T>
class SplayTree
{
    class Node
    {
    public:
        Node(std::size_t key, T* value)
            : key(key), value(value), left(nullptr), right(nullptr)
        {
        }

        ~Node()
        {
            delete value;
        }
        std::size_t key;
        T* value;
        Node* left, * right;
    };

    // A utility function to right rotate subtree Node with y
    Node* rightRotate(Node* x)
    {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    // A utility function to left rotate subtree Node with x
    Node* leftRotate(Node* x)
    {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // This function brings the key at node if key is present in tree.
    // If key is not present, then it brings the last accessed item at Node.
    // This function modifies the tree and returns the new Node
    Node* splay(Node* node, std::size_t key)
    {
        // Base cases: Node is NULL or
        // key is present at Node
        if (node == nullptr || node->key == key)
            return node;

        // Key lies in left subtree
        if (node->key > key) {
            // Key is not in tree, we are done
            if (node->left == nullptr) return node;

            // Zig-Zig (Left Left)
            if (node->left->key > key) {
                // First recursively bring the
                // key as node of left-left
                node->left->left = splay(node->left->left, key);

                // Do first rotation for node,
                // second rotation is done after else
                node = rightRotate(node);
            }
            else if (node->left->key < key) // Zig-Zag (Left Right)
            {
                // First recursively bring
                // the key as node of left-right
                node->left->right = splay(node->left->right, key);

                // Do first rotation for Node->left
                if (node->left->right != nullptr)
                    node->left = leftRotate(node->left);
            }

            // Do second rotation for node
            return (node->left == nullptr) ? node : rightRotate(node);
        }
        else // Key lies in right subtree
        {
            // Key is not in tree, we are done
            if (node->right == nullptr) return node;

            // Zig-Zag (Right Left)
            if (node->right->key > key) {
                // Bring the key as Node of right-left
                node->right->left = splay(node->right->left, key);

                // Do first rotation for node->right
                if (node->right->left != nullptr)
                    node->right = rightRotate(node->right);
            }
            else if (node->right->key < key)// Zag-Zag (Right Right)
            {
                // Bring the key as node of
                // right-right and do first rotation
                node->right->right = splay(node->right->right, key);
                node = leftRotate(node);
            }

            // Do second rotation for Node
            return (node->right == nullptr) ? node : leftRotate(node);
        }
    }

    // A utility function to print pre-order traversal of the tree.
    // The function also prints height of every node
    void preOrder(Node* node)
    {
        if (node != nullptr) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void in_order(Node* node, std::vector<T*>& list)
    {
        if (node != nullptr) {
            in_order(node->left, list);
            list.push_back(node->value);
            in_order(node->right, list);
        }
    }

    Node* m_root;
    std::vector<std::unique_ptr<Node>> m_nodes;
public:
    SplayTree()
        : m_root(nullptr)
    {
    }

    ~SplayTree()
    {
        clear();
    }

    SplayTree(SplayTree&&) = delete;
    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(SplayTree&&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;

    // Delete all objects and set the root back to nullptr
    void clear()
    {
        m_nodes.clear();
        m_root = nullptr;
    }


    // Function to insert a new key 'k' in splay tree with given root
    T* insert(std::size_t k, T* value)
    {
        // Simple Case: If tree is empty
        if (m_root == nullptr) {
            m_root = new Node(k, value);
            m_nodes.push_back(std::unique_ptr<Node>(m_root));
            return value;
        }

        // Bring the closest leaf node to root
        m_root = splay(m_root, k);

        // If key is already present, then return
        if (m_root->key == k) {
            delete value;
            return m_root->value;
        }
        // Otherwise allocate memory for new Node
        Node* new_node = new Node(k, value);

        // If root's key is greater, make root as right child of new node and copy the left child of root to new Node
        if (m_root->key > k) {
            new_node->right = m_root;
            new_node->left = m_root->left;
            m_root->left = nullptr;
        }

        // If root's key is smaller, make root as left child of new Node and copy the right child of root to new Node
        else {
            new_node->left = m_root;
            new_node->right = m_root->right;
            m_root->right = nullptr;
        }

        m_root = new_node;
        m_nodes.push_back(std::unique_ptr<Node>(new_node));
        return value; // new node becomes new root
    }

    T* insert(T* value)
    {
        return insert(m_nodes.size(), value);
    }

    void remove(std::size_t key)
    {
        Node* temp;
        if (!m_root)
            return;

        // Splay the given key    
        m_root = splay(m_root, key);

        // If key is not present, then return root
        if (key != m_root->key)
            return;

        // If key is present
        // If left child of root does not exist make root->right as root   
        if (!m_root->left) {
            temp = m_root;
            m_root = m_root->right;
        }

        // Else if left child exits
        else {
            temp = m_root;

            /*Note: Since key == root->key,
            so after Splay(key, root->leftChild),
            the tree we get will have no right child tree
            and maximum node in left subtree will get splayed*/
            // New root
            m_root = splay(m_root->left, key);

            // Make right child of previous root as new root's right child
            m_root->right = temp->right;
        }

        for (auto i = 0; i < m_nodes.size(); ++i)
            if (m_nodes[i]->key == key) {
                m_nodes.erase(m_nodes.begin() + i);
                break;
            }

        // return root of the new Splay Tree
        return;
    }

    T* search(std::size_t key)
    {
        m_root = splay(m_root, key);

        if (m_root)
            if (m_root->key == key)
                return m_root->value;

        return nullptr;
    }

    std::vector<T*> getOrderedList()
    {
        std::vector<T*> temp_list;
        in_order(m_root, temp_list);
        return temp_list;
    }

    std::size_t size()
    {
        return m_nodes.size();
    }

    void print()
    {
        preOrder(m_root);
        std::cout << "\n";
    }

};