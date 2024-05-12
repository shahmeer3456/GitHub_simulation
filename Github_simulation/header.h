#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class GithubEntity
{
private:
    string username, password, id;

public:
    GithubEntity() {}
    GithubEntity(string user, string pass);
    string getid() const;
    void setid(string id);
    string getusername() const;
    void setusername(string user);
    string getpassword();
    void setpassword(string pass);
    bool searchForFollower(string id);
    void menuforeverything();
};

class user : public GithubEntity
{
private:
    int maxFollowers = 10;
    string* followers;

public:
    static int friendcount;
    user();
    user(int max, int n, string* friendList, string* pagesliked);
    ~user();
    int getMaxFollowers();
    void input();
    void savetofile();
    void setmaxpages(int max);
    bool signin();
    void addFollower(string id, string friendname);
    int getFollowerIndCount();
    void setFollowerCount(int c);
    void incrementFollowerCount();
    int getFollowerIndCountFromFile();
    void logout();
};
