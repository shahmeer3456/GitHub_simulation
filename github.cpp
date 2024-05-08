#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

struct CommitNode 
{
    string message;
    CommitNode* next;
    CommitNode(string msg) : message(msg), next(nullptr) {}
};

struct FileNode 
{
    string filename;
    FileNode* next;
    FileNode(string name) : filename(name), next(nullptr) {}
};


struct RepositoryNode
 {
    string name;
    bool isPrivate;
    RepositoryNode* left;
    RepositoryNode* right;
    CommitNode* commitsHead;
    FileNode* filesHead;
    RepositoryNode(string repoName, bool privateRepo) : name(repoName), isPrivate(privateRepo), left(nullptr), right(nullptr), commitsHead(nullptr), filesHead(nullptr) {}
};

class RepositoryManager 
{
private:
    RepositoryNode* root;

    void addCommit(RepositoryNode* repo, string commitMsg)
     {
        CommitNode* newCommit = new CommitNode(commitMsg);
        newCommit->next = repo->commitsHead;
        repo->commitsHead = newCommit;
    }

   
    void addFile(RepositoryNode* repo, string filename) 
    {
        FileNode* newFile = new FileNode(filename);
        newFile->next = repo->filesHead;
        repo->filesHead = newFile;

        // Update file list in repository file
        ofstream repoFile(repo->name + ".txt", ios::app);
        if (repoFile.is_open()) {
            repoFile << filename << endl;
            repoFile.close();
        }
        else {
            cout << "Unable to open repository file for writing." << endl;
        }
    }

    // Helper function to delete a repository node and its associated linked lists
    void deleteRepositoryNode(RepositoryNode* node)
     {
        if (node == nullptr) return;
        deleteRepositoryNode(node->left);
        deleteRepositoryNode(node->right);
        CommitNode* commitPtr = node->commitsHead;
        while (commitPtr != nullptr) 
        {
            CommitNode* temp = commitPtr;
            commitPtr = commitPtr->next;
            delete temp;
        }
        FileNode* filePtr = node->filesHead;
        while (filePtr != nullptr) 
        {
            FileNode* temp = filePtr;
            filePtr = filePtr->next;
            delete temp;
        }
        delete node;
    }

public:
    RepositoryManager() : root(nullptr) {}

    
    void createRepository(string repoName, bool isPrivate) 
    {
        RepositoryNode* newNode = new RepositoryNode(repoName, isPrivate);
        if (root == nullptr) 
        {
            root = newNode;
        }
        else {
            RepositoryNode* current = root;
            RepositoryNode* parent = nullptr;
            while (current != nullptr) 
            {
                parent = current;
                if (repoName < current->name) 
                {
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }
            if (repoName < parent->name)
             {
                parent->left = newNode;
            }
            else 
            {
                parent->right = newNode;
            }
        }
        // Create a file for the repository
        ofstream repoFile(repoName + ".txt");
        if (repoFile.is_open()) {
            repoFile << "Files in repository '" << repoName << "':" << endl;
            repoFile.close();
            cout << "Repository '" << repoName << "' created successfully." << endl;
        }
        else {
            cout << "Unable to create repository file." << endl;
        }
    }

    // Function to delete a repository
    void deleteRepository(string repoName) 
    {
        root = deleteRepositoryHelper(root, repoName);
    }

    // Helper function to delete a repository
    RepositoryNode* deleteRepositoryHelper(RepositoryNode* node, string repoName)
     {
        if (node == nullptr) return nullptr;
        if (repoName < node->name) 
        {
            node->left = deleteRepositoryHelper(node->left, repoName);
        }
        else if (repoName > node->name) 
        {
            node->right = deleteRepositoryHelper(node->right, repoName);
        }
        else {
            if (node->left == nullptr) 
            {
                RepositoryNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) 
            {
                RepositoryNode* temp = node->left;
                delete node;
                return temp;
            }
            RepositoryNode* temp = minValueNode(node->right);
            node->name = temp->name;
            node->right = deleteRepositoryHelper(node->right, temp->name);
        }
        return node;
    }

    // Function to find the repository node with the minimum value
    RepositoryNode* minValueNode(RepositoryNode* node) {
        RepositoryNode* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Function to add a commit to a repository
    void commitToRepository(string repoName, string commitMsg) 
    {
        RepositoryNode* repo = searchRepository(root, repoName);
        if (repo != nullptr) 
        {
            addCommit(repo, commitMsg);
            cout << "Commit added to repository '" << repoName << "'." << endl;
        }
        else 
        {
            cout << "Repository not found!" << endl;
        }
    }

    // Function to add a file to a repository
    void addFileToRepository(string repoName, string filename) 
    {
        RepositoryNode* repo = searchRepository(root, repoName);
        if (repo != nullptr) 
        {
            addFile(repo, filename);
            cout << "File '" << filename << "' added to repository '" << repoName << "'." << endl;
        }
        else
         {
            cout << "Repository not found!" << endl;
        }
    }

    // Function to display all commits of a repository
    void displayCommits(string repoName) 
    {
        RepositoryNode* repo = searchRepository(root, repoName);
        if (repo != nullptr) {
            cout << "Commits for repository '" << repoName << "':" << endl;
            CommitNode* commitPtr = repo->commitsHead;
            while (commitPtr != nullptr) {
                cout << "- " << commitPtr->message << endl;
                commitPtr = commitPtr->next;
            }
        }
        else {
            cout << "Repository not found!" << endl;
        }
    }

    // Function to search for a repository node by name
    RepositoryNode* searchRepository(RepositoryNode* node, string repoName) 
    {
        if (node == nullptr || node->name == repoName) 
        {
            return node;
        }
        if (repoName < node->name) 
        {
            return searchRepository(node->left, repoName);
        }
        else 
        {
            return searchRepository(node->right, repoName);
        }
    }
};
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
        for (char c : key) {
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

    ~HashTable() {
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
        while (current) {
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

void runRepositoryMenu(RepositoryManager& manager)
 {
    char choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Create Repository\n";
        cout << "2. Add Commit\n";
        cout << "3. Add File\n";
        cout << "4. Display Commits\n";
        cout << "5. Delete Repository\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1': 
        {
            string repoName;
            bool isPrivate;
            cout << "Enter repository name: ";
            cin >> repoName;
            cout << "Is the repository private? (1 for Yes, 0 for No): ";
            cin >> isPrivate;
            manager.createRepository(repoName, isPrivate);
            break;
        }
        case '2': 
        {
            string repoName, commitMsg;
            cout << "Enter repository name: ";
            cin >> repoName;
            cout << "Enter commit message: ";
            cin.ignore();
            getline(cin, commitMsg);
            manager.commitToRepository(repoName, commitMsg);
            break;
        }
        case '3': 
        {
            string repoName, filename;
            cout << "Enter repository name: ";
            cin >> repoName;
            cout << "Enter filename: ";
            cin >> filename;
            manager.addFileToRepository(repoName, filename);
            break;
        }
        case '4': 
        {
            string repoName;
            cout << "Enter repository name: ";
            cin >> repoName;
            manager.displayCommits(repoName);
            break;
        }
        case '5': 
        {
            string repoName;
            cout << "Enter repository name to delete: ";
            cin >> repoName;
            manager.deleteRepository(repoName);
            break;
        }
        case '6':
         {
            cout << "Exiting program.\n";
            break;
        }
        default: 
        {
            cout << "Invalid choice!\n";
            break;
        }
        }
    } 
    while (choice != '6');
}

class GithubEntity 
{
private:
    RepositoryManager manager;
    HashTable table;

public:
    void menuforeverything() {
        runRepositoryMenu(manager);
    }
};

int main() 
{
    GithubEntity s;
    s.menuforeverything();
}