#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "header.h"
#include <ctime>
#include <chrono>
#include "HashTable.h"
#include "repository.h"
#include<algorithm>
#include<string>
#include<Windows.h>
#include "Graph.h"
using namespace std;

HashTable userHashTable(100);

GithubEntity::GithubEntity(string u, string p) : username(u), password(p) {}

string GithubEntity::getid() const
{
    return id;
}

void GithubEntity::setid(string id)
{
    this->id = id;
}

string GithubEntity::getusername() const
{
    return username;
}

void GithubEntity::setusername(string user)
{
    username = user;
}

string GithubEntity::getpassword()
{
    return password;
}

void GithubEntity::setpassword(string pass)
{
    password = pass;
}

string trim(const string& str)
{
    int first = str.find_first_not_of(' ');
    if (first == -1)
        return "";
    int last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool GithubEntity::searchForFollower(string userid)
{
    ifstream fin("user.txt");
    if (!fin)
    {
        cout << "Unable to open the file." << endl;
        return false;
    }
    string line;
    while (getline(fin, line))
    {
        if (line.find("Username: ") != -1)
        {
            string username = line.substr(10);
            if (username == userid)
            {
                fin.close();
                return true;
            }
        }
    }

    fin.close();
    return false;
}

user::user() : followers(nullptr), maxFollowers(0) {}

user::user(int max, int n, string* friendList, string* pagesliked) : maxFollowers(max)
{
    followers = new string[max];
    for (int i = 0; i < max; ++i)
    {
        followers[i] = friendList[i];
    }
    this->friendcount = 0;
}

user::~user()
{
    delete[] followers;
}

int user::getMaxFollowers()
{
    return maxFollowers;
}

void user::input()
{
    string username, password;
    cout << "Enter your username\n";
    getline(cin, username);
    cout << "Enter your password\n";
    getline(cin, password);
    this->setusername(username);
    this->setpassword(password);

    userHashTable.insert(username, password);
    cout << "User data saved to hash table\n";
}

void user::savetofile()
{
    ofstream fout("user.txt", ios::app);
    if (fout)
    {
        fout << "Username: " << getusername() << endl;
        fout << "Password: " << getpassword() << endl;
        fout << "--------------------------------" << endl;
        cout << "User data saved to file\n";
        fout.close();
    }
    else
    {
        cout << "Can not open file\n";
    }
}

bool user::signin()
{
    ifstream fin("user.txt");
    string username, password;

    cout << "Enter username: ";
    getline(cin.ignore(), username);
    this->setusername(username);

    cout << "Enter password: ";
    getline(cin, password);

    bool isuername = false, ispassword = false;

    // search and compare with hash table
    bool search = userHashTable.search(username) == password;
    if (search)
    {
        cout << "User found in hash table" << endl;
        return true;
    }
    else
    {
        cout << "User not found in hash table" << endl;
        return false;
    }
    return false;
}

int user::friendcount = 0;

void user::incrementFollowerCount()
{
    ++friendcount;
}
void user::setFollowerCount(int count)
{
    friendcount = getFollowerIndCount();
    this->friendcount = count;
}
int user::getFollowerIndCount()
{
    ifstream fin("user.txt");
    int count = 1;
    string line;
    if (fin)
    {
        while (getline(fin, line))
        {
            if (line.find("Freind Count ") != -1)
            {
                count++;
            }
        }
    }
    fin.close();
    return count;
}

int user::getFollowerIndCountFromFile()
{
    ifstream fin("user.txt");
    string line;
    int c = 0;
    if (fin)
    {
        while (getline(fin, line))
        {
            if (line.find("Freind Count ") != -1)
            {
                line.substr(12);
                // line=line.substr(0, 14);
                c++;
                // cout << line << endl;
            }
        }
        return c;
        // cout << "total friend count from file " << c << endl;
    }
    else
    {
        cout << "can not open file\n";
    }
}

void user::addFollower(string username, string friendusername)
{
    if (searchForFollower(friendusername))
    {
        ofstream fout("user.txt", ios::app);
        ifstream fin("user.txt"); // Open the file for reading
        if (fout && fin)
        {
            if (getFollowerIndCountFromFile() < 10)
            {
                fout << "Freind Count " << getFollowerIndCount() << endl;
                fout << "Friend of " << getusername() << ": " << friendusername << endl;
                fout << "--------------------------------" << endl;

                // Add the reverse friendship
                string line;
                while (getline(fin, line))
                {
                    if (line.find("Username: " + friendusername) != -1)
                    {
                        fout << "Friend of " << friendusername << ": " << getusername() << endl;
                        fout << "--------------------------------" << endl;
                    }
                }

                cout << "Friend added successfully." << endl;
                fout.close();
                fin.close();
            }
            else
            {
                cout << "Cannot add friend. The maximum limit is 10 friends.\n";
            }
        }
        else
        {
            cout << "Unable to open the file." << endl;
        }
    }
    else
    {
        cout << "User not found." << endl;
    }
}

void user::logout()
{
    setusername("");
    setpassword("");
}

void Graph::addUser(const string& username) {
    // Check if the user already exists
    for (const auto& node : nodes) {
        if (node.username == username) {
            return; // User already exists
        }
    }
    // Add a new user
    Node newNode;
    newNode.username = username;
    nodes.push_back(newNode);
}

void Graph::addFollower(const string& user, const string& follower) {
    for (auto& node : nodes) {
        if (node.username == user) {
            node.followers.push_back(follower);
            return;
        }
    }
}

void Graph::removeFollower(const string& user, const string& follower) {
    for (auto& node : nodes) {
        if (node.username == user) {
            auto it = std::find(node.followers.begin(), node.followers.end(), follower);
            if (it != node.followers.end()) {
                node.followers.erase(it);
            }
            return;
        }
    }
}

void Graph::displayFollowers(const string& user) {
    for (const auto& node : nodes) {
        if (node.username == user) {
            cout << "Followers of " << user << ":" << endl;
            for (const auto& follower : node.followers) {
                cout << follower << endl;
            }
            return;
        }
    }
    cout << "User not found." << endl;
}

void Graph::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        return;
    }
    for (const auto& node : nodes) {
        for (const auto& follower : node.followers) {
            file << node.username << " follows " << follower << endl;
        }
    }
    cout << "Data saved to file." << endl;
}

void Graph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        string user, follows, follower;
        stringstream ss(line);
        ss >> user >> follows >> follower;
        if (follows == "follows") {
            addFollower(user, follower);
        }
    }
    cout << "Data loaded from file." << endl;
}

User::User(Graph* g) : graph(g) {}

string User::getUsername() const {
    return username;
}

void User::setUsername(const string& name) {
    username = name;
    graph->addUser(name);
}

void User::follow(User& otherUser) {
    graph->addFollower(otherUser.getUsername(), username);
}

void User::unfollow(User& otherUser) {
    graph->removeFollower(otherUser.getUsername(), username);
}

void User::displayFollowers() {
    graph->displayFollowers(username);
}

void manageFollowers(Graph& graph) {
    string username1, username2;

    cout << "Enter the username for user1: ";
    cin >> username1;

    cout << "Enter the username for user2: ";
    cin >> username2;

    int choice;
    cout << "Enter 1 to follow, 2 to unfollow: ";
    cin >> choice;

    User user1(&graph);
    User user2(&graph);
    user1.setUsername(username1);
    user2.setUsername(username2);

    if (choice == 1) {
        user1.follow(user2);
        cout << user1.getUsername() << " is now following " << user2.getUsername() << endl;
    }
    else if (choice == 2) {
        user1.unfollow(user2);
        cout << user1.getUsername() << " has unfollowed " << user2.getUsername() << endl;
    }
    else {
        cout << "Invalid choice." << endl;
    }

    // Display followers of user2
    user2.displayFollowers();

    // Save data to file
    graph.saveToFile("followers.txt");

    // Load data from file
    graph.loadFromFile("followers.txt");
}
void loading()							//loading screen
{
    system("color F0");//colors of main menu
    system("cls");
    int d;
    d = 0;
    while (!(d == 1))
    {
        

        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t ";
        Sleep(0700);
        cout << "L";
        Sleep(0700);
        cout << "O";
        Sleep(0700);
        cout << "A";
        Sleep(0700);
        cout << "D";
        Sleep(0700);
        cout << "I";
        Sleep(0700);
        cout << "N";
        Sleep(0700);
        cout << "G";
        Sleep(0700);
        cout << ".";
        system("cls");
        d++;
    }
}

void GithubEntity::menuforeverything()
{
    loading();
    bool continueLoop = true;

    while (continueLoop)
    {
        try
        {
            user u;
            string choice, comment, post;
            int opt;
            bool login = true;
            RepositoryManager repoManager;


            system("color 5f");

            cout << "\t\t\t(-------Welcome to Github-------)\n";
            cout << "1. Press 1 If you are a User:" << endl;
            cout << "4. Press 2 for exiting\n";
            cin >> opt;

            switch (opt)
            {
            case 1:
            {
                while (true)
                {
                    try
                    {

                        cout << "Press 1 for signin" << endl;
                        cout << "Press 2 for repository menu" << endl;
                        cout << "Press 3 to add a follower" << endl;
                        cout << "Press 4 to View your profile" << endl;
                        cout << "Press 5 to follow/unfollow" << endl; // Added option for logout
                        cout << "Press 6 to logout" << endl;

                        cin >> opt;

                        if (opt == 1)
                        {
                            // singin the user first
                            if (!u.signin())
                            {
                                cout << "ID not found, please sign up\n";
                                u.input();
                                u.savetofile();
                            }
                            else

                            {
                                loading();

                                cout << "---------------------------Welcome " << u.getusername() << " ---------------------------" << endl;
                            }
                        }

                        if (opt == 2)
                        {
                            if (u.getusername() != "")
                            {
                                char repoChoice;
                                do
                                {
                                    

                                    cout << "\nRepository Menu:\n";
                                    cout << "1. Create Repository\n";
                                    cout << "2. Add Commit\n";
                                    cout << "3. Add File\n";
                                    cout << "4. Display Commits\n";
                                    cout << "5. Delete Repository\n";
                                    cout << "6. Back to main menu\n";
                                    cout << "Enter your choice: ";
                                    cin >> repoChoice;

                                    switch (repoChoice)
                                    {
                                    case '1':

                                    {
                                        loading();

                                        string repoName;
                                        bool isPrivate;
                                        cout << "Enter repository name: ";
                                        cin >> repoName;
                                        cout << "Is the repository private? (1 for Yes, 0 for No): ";
                                        cin >> isPrivate;
                                        repoManager.createRepository(repoName, isPrivate);
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
                                        repoManager.commitToRepository(repoName, commitMsg);
                                        break;
                                    }
                                    case '3':
                                    {
                                        string repoName, filename;
                                        cout << "Enter repository name: ";
                                        cin >> repoName;
                                        cout << "Enter filename: ";
                                        cin >> filename;
                                        repoManager.addFileToRepository(repoName, filename);
                                        break;
                                    }
                                    case '4':
                                    {
                                        string repoName;
                                        cout << "Enter repository name: ";
                                        cin >> repoName;
                                        repoManager.displayCommits(repoName);
                                        break;
                                    }
                                    case '5':
                                    {
                                        string repoName;
                                        cout << "Enter repository name to delete: ";
                                        cin >> repoName;
                                        repoManager.deleteRepository(repoName);
                                        break;
                                    }
                                    case '6':
                                    {
                                        cout << "Exiting repository menu.\n";
                                        break;
                                    }
                                    default:
                                    {
                                        cout << "Invalid choice!\n";
                                        break;
                                    }
                                    }
                                } while (repoChoice != '6');
                            }
                            else
                            {
                                cout << "ID not found, please sign up\n";
                                cin.clear();
                                cin.ignore();
                                u.input();
                                u.savetofile();
                            }
                        }
                        else if (opt == 3)
                        {
                            string friendusername;
                            cout << "Enter the username of the friend you want to add: ";
                            cin >> friendusername;
                            u.addFollower(u.getusername(), friendusername);
                        }
                        else if (opt == 4)
                        {
                            if (u.getusername() != "")
                            {
                                cout << "Username: " << u.getusername() << endl;
                                cout << "Password: " << u.getpassword() << endl;
                                cout << "Follower Count: " << u.getFollowerIndCountFromFile() << endl;
                            }
                            else
                            {
                                cout << "Please sign in first.\n";
                            }
                        }
                        else if (opt == 5)
                        {
                            Graph graph;
                            manageFollowers(graph);
                        }
                        else if (opt == 6)
                        {
                            // Logout
                            u.logout();
                            cout << "Logout successful." << endl;
                        }
                        else
                        {
                            cout << "Invalid choice\n";
                            cin.clear();
                            cin.ignore();
                        }
                    }
                    catch (const exception& e)
                    {
                        cout << "Exception: " << e.what() << endl;
                    }
                    catch (...)
                    {
                        cout << "An unexpected exception occurred." << endl;
                    }
                }
                break;
            }


            case 2:
            {
                continueLoop = false;
                break;
            }

            default:
            {
                cout << "Invalid choice\n";
                cin.clear();
                cin.ignore();
                break;
            }
            }
        }
        catch (const exception& e)
        {
            cout << "Exception: " << e.what() << endl;
        }
        catch (...)
        {
            cout << "An unexpected exception occurred." << endl;
        }

        cout << "Do you want to continue? Press 1 for yes and any other number for no: ";
        cin >> continueLoop;

        if (!cin)
        {
            cin.clear();
            cin.ignore();
        }
    }
}
