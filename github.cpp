#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

struct CommitNode {
    string message;
    CommitNode* next;
    CommitNode(string msg) : message(msg), next(nullptr) {}
};

struct FileNode {
    string filename;
    FileNode* next;
    FileNode(string name) : filename(name), next(nullptr) {}
};


struct RepositoryNode {
    string name;
    bool isPrivate;
    RepositoryNode* left;
    RepositoryNode* right;
    CommitNode* commitsHead;
    FileNode* filesHead;
    RepositoryNode(string repoName, bool privateRepo) : name(repoName), isPrivate(privateRepo), left(nullptr), right(nullptr), commitsHead(nullptr), filesHead(nullptr) {}
};

class RepositoryManager {
private:
    RepositoryNode* root;

    void addCommit(RepositoryNode* repo, string commitMsg) {
        CommitNode* newCommit = new CommitNode(commitMsg);
        newCommit->next = repo->commitsHead;
        repo->commitsHead = newCommit;
    }

   
    void addFile(RepositoryNode* repo, string filename) {
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
    void deleteRepositoryNode(RepositoryNode* node) {
        if (node == nullptr) return;
        deleteRepositoryNode(node->left);
        deleteRepositoryNode(node->right);
        CommitNode* commitPtr = node->commitsHead;
        while (commitPtr != nullptr) {
            CommitNode* temp = commitPtr;
            commitPtr = commitPtr->next;
            delete temp;
        }
        FileNode* filePtr = node->filesHead;
        while (filePtr != nullptr) {
            FileNode* temp = filePtr;
            filePtr = filePtr->next;
            delete temp;
        }
        delete node;
    }

public:
    RepositoryManager() : root(nullptr) {}

    
    void createRepository(string repoName, bool isPrivate) {
        RepositoryNode* newNode = new RepositoryNode(repoName, isPrivate);
        if (root == nullptr) {
            root = newNode;
        }
        else {
            RepositoryNode* current = root;
            RepositoryNode* parent = nullptr;
            while (current != nullptr) {
                parent = current;
                if (repoName < current->name) {
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }
            if (repoName < parent->name) {
                parent->left = newNode;
            }
            else {
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
    void deleteRepository(string repoName) {
        root = deleteRepositoryHelper(root, repoName);
    }

    // Helper function to delete a repository
    RepositoryNode* deleteRepositoryHelper(RepositoryNode* node, string repoName) {
        if (node == nullptr) return nullptr;
        if (repoName < node->name) {
            node->left = deleteRepositoryHelper(node->left, repoName);
        }
        else if (repoName > node->name) {
            node->right = deleteRepositoryHelper(node->right, repoName);
        }
        else {
            if (node->left == nullptr) {
                RepositoryNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
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
    void commitToRepository(string repoName, string commitMsg) {
        RepositoryNode* repo = searchRepository(root, repoName);
        if (repo != nullptr) {
            addCommit(repo, commitMsg);
            cout << "Commit added to repository '" << repoName << "'." << endl;
        }
        else {
            cout << "Repository not found!" << endl;
        }
    }

    // Function to add a file to a repository
    void addFileToRepository(string repoName, string filename) {
        RepositoryNode* repo = searchRepository(root, repoName);
        if (repo != nullptr) {
            addFile(repo, filename);
            cout << "File '" << filename << "' added to repository '" << repoName << "'." << endl;
        }
        else {
            cout << "Repository not found!" << endl;
        }
    }

    // Function to display all commits of a repository
    void displayCommits(string repoName) {
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
    RepositoryNode* searchRepository(RepositoryNode* node, string repoName) {
        if (node == nullptr || node->name == repoName) {
            return node;
        }
        if (repoName < node->name) {
            return searchRepository(node->left, repoName);
        }
        else {
            return searchRepository(node->right, repoName);
        }
    }
};



int main()
{

}