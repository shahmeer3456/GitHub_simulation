#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
using namespace std;

class HashTable
{
private:
    struct Node
    {
        string key;
        string value;
        Node* next;

        Node(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
    };

    vector<Node*> table;
    int size;

    int hash(const string& key)
    {
        int hashValue = 0;
        for (char c : key)
        {
            hashValue += c;
        }
        return hashValue % table.size();
    }

public:
    HashTable(int initialSize) : size(0)
    {
        table.resize(initialSize, nullptr);
    }

    HashTable() {}

    ~HashTable()
    {
        for (Node* node : table)
        {
            while (node)
            {
                Node* prev = node;
                node = node->next;
                delete prev;
            }
        }
    }

    void insert(const string& key, const string& value)
    {
        int index = hash(key);
        Node* newNode = new Node(key, value);
        if (!table[index])
        {
            table[index] = newNode;
        }
        else
        {
            Node* current = table[index];
            while (current->next)
            {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    string search(const string& key)
    {
        int index = hash(key);
        Node* current = table[index];
        while (current)
        {
            if (current->key == key)
            {
                return current->value;
            }
            current = current->next;
        }
        return "Not found";
    }

    void remove(const string& key)
    {
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current)
        {
            if (current->key == key)
            {
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    table[index] = current->next;
                }
                delete current;
                size--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    int getSize() const
    {
        return size;
    }
};

#endif
