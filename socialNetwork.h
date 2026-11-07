// CSCE 2110 - Social Network Connection Search
// Increment 1: Graph loading and BFS shortest path
// Authors: Silvana Velazquez Vivanco and Samuel Lucena Godoi

#ifndef SOCIAL_NETWORK_H
#define SOCIAL_NETWORK_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct MeetingResult {
    string meetingPoint;
    vector<string> pathFromUser1;
    vector<string> pathFromUser2;
    vector<string> pathFromUser3;
};

class AdjacencyList {
public:
    struct Node {
        int userID;
        Node* next;
    };

private:
    Node** heads;
    int vertexCount;
    int capacity;
    int friendshipCount;

    void ensureCapacity(int requiredCapacity);
    bool hasEdge(int from, int to) const;
    void appendNeighbor(int userID, int neighborID);

public:
    AdjacencyList();
    ~AdjacencyList();

    AdjacencyList(const AdjacencyList&) = delete;
    AdjacencyList& operator=(const AdjacencyList&) = delete;

    int addVertex();
    bool addUndirectedEdge(int first, int second);
    int size() const;
    int edgeCount() const;
    const Node* neighbors(int userID) const;
};

class SocialNetwork {
private:
    unordered_map<string, int> userToID;  // Maps username to an integer ID
    vector<string> idToUser;              // Maps ID back to username
    AdjacencyList adjList;                // Hand-built linked-list adjacency storage

    // Helper functions
    int getUser(const string& name);
    void bfs(int start, vector<int>& distances, vector<int>& parents);
    vector<string> reconstructPath(int target, const vector<int>& parents);

public:
    // Core functionalities
    bool loadFromFile(const string& filename);
    bool hasUser(const string& name);
    int getNumUsers();
    int getNumFriendships();
    
    // Finds the shortest path between two users
    vector<string> shortestPath(const string& from, const string& to);

    // Finds the shortest path only when its edge count is below the exclusive limit
    vector<string> shortestPathUnderLimit(const string& from, const string& to, int connectionLimitExclusive);

    void printPath();
    
    vector<string> shortestRoute(const string& start, const string& required1,
                                 const string& required2, const string& end);

    vector<string>round_trip(const string& start); // OP 3: Visit all reachable nodes and return to the first one

    MeetingResult findMeetingPoint(const string& user1, const string& user2,
                                   const string& user3);

};

#endif