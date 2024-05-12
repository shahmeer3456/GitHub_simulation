#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Graph {
public:
    struct Node {
        string username;
        vector<string> followers;
    };

    vector<Node> nodes;

public:
    void addUser(const string& username);
    void addFollower(const string& user, const string& follower);
    void removeFollower(const string& user, const string& follower);
    void displayFollowers(const string& user);
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
};

class User {
public:
    string username;
    Graph* graph;

public:
    User(Graph* g);

    string getUsername() const;
    void setUsername(const string& name);
    void follow(User& otherUser);
    void unfollow(User& otherUser);
    void displayFollowers();
};

void manageFollowers(Graph& graph);
