// CSCE 2110 - Social Network Connection Search
// Increment 1: Graph loading and BFS shortest path
// Authors: Silvana Velazquez Vivanco and Samuel Lucena Godoi

#ifndef SOCIAL_NETWORK_H
#define SOCIAL_NETWORK_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SocialNetwork {
private:
    unordered_map<string, int> userToID;  // Maps username to an integer ID
    vector<string> idToUser;              // Maps ID back to username
    vector<vector<int>> adjList;          // Adjacency list for friendships

    // Helper functions
    int getUser(const string& name);
    void bfs(int start, vector<int>& distances, vector<int>& parents);

public:
    // Core functionalities
    bool loadFromFile(const string& filename);
    bool hasUser(const string& name);
    int getNumUsers();
    int getNumFriendships();
    
    // Finds the shortest path between two users
    vector<string> shortestPath(const string& from, const string& to);
};

#endif