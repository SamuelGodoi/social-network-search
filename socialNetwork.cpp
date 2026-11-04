#include "socialNetwork.h"
#include <iostream>
#include <fstream>
#include <queue>

// Returns the ID for a name. If they don't exist yet, it adds them.
int SocialNetwork::getUser(const string& name) {
    // If user already exists, return their ID
    if (userToID.count(name) > 0) {
        return userToID[name];
    }
    
    // Otherwise, create a new user
    int newID = idToUser.size();
    userToID[name] = newID;
    idToUser.push_back(name);
    adjList.push_back(vector<int>()); // Add an empty friend list for them
    
    return newID;
}

// Reads friendships from a text file
bool SocialNetwork::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string user1, user2;
    // Simple way to read two strings per line
    while (file >> user1 >> user2) {
        if (user1 == user2) continue; // Skip if someone is friends with themselves

        int id1 = getUser(user1);
        int id2 = getUser(user2);

        // Check if they are already friends to avoid duplicates
        bool alreadyFriends = false;
        for (int i = 0; i < adjList[id1].size(); i++) {
            if (adjList[id1][i] == id2) {
                alreadyFriends = true;
                break;
            }
        }
        
        // Add the friendship 
        if (!alreadyFriends) {
            adjList[id1].push_back(id2);
            adjList[id2].push_back(id1);
        }
    }
    return true;
}

bool SocialNetwork::hasUser(const string& name) {
    return userToID.count(name) > 0;
}

int SocialNetwork::getNumUsers() {
    return idToUser.size();
}

int SocialNetwork::getNumFriendships() {
    int totalEdges = 0;
    for (int i = 0; i < adjList.size(); i++) {
        totalEdges += adjList[i].size();
    }
    return totalEdges / 2; // Divide by 2 because A->B and B->A is one friendship
}

// Breadth-First Search
void SocialNetwork::bfs(int start, vector<int>& distances, vector<int>& parents) {
    distances.assign(adjList.size(), -1);
    parents.assign(adjList.size(), -1);
    
    queue<int> q;
    
    distances[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // Check all friends of the current user
        for (int i = 0; i < adjList[current].size(); i++) {
            int neighbor = adjList[current][i];
            
            // If we haven't visited this neighbor yet
            if (distances[neighbor] == -1) {
                distances[neighbor] = distances[current] + 1;
                parents[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
}

// Uses BFS to find the path, then traces backward using the parent array
vector<string> SocialNetwork::shortestPath(const string& from, const string& to) {
    vector<string> path;
    
    if (!hasUser(from) || !hasUser(to)) {
        return path; // Return empty if either user doesn't exist
    }
    
    int startID = userToID[from];
    int targetID = userToID[to];
    
    vector<int> distances;
    vector<int> parents;
    
    bfs(startID, distances, parents);
    
    // If the target wasn't reached, they aren't connected
    if (distances[targetID] == -1) {
        return path; 
    }
    
    // Trace back from target to start
    int current = targetID;
    while (current != -1) {
        path.push_back(idToUser[current]);
        current = parents[current];
    }
    
    // The path is currently backwards (target -> start), so flip it
    vector<string> correctPath;
    for (int i = path.size() - 1; i >= 0; i--) {
        correctPath.push_back(path[i]);
    }

    return correctPath;
}

// Prints the list of the social network
void SocialNetwork::printPath() {
    for (int i=0; i < adjList.size(); i++) {
        cout << "[" << i << "] " << idToUser[i] << " --> ";
        for (int j=0; j< adjList[i].size(); j++) {
            int friendID = adjList[i][j];
            cout << idToUser[friendID] << " ";
        }
            cout << endl;
    }
}

// Multi-stop route method
vector<string> SocialNetwork::shortestRoute(const string& start, const string& stop, const string& end) {
    vector<string>route;

    vector<string> path1 = shortestPath(start,stop); //Finds the shortest path from the start to the stop
    vector<string> path2 = shortestPath(stop,end); // Fiinds the shortest path from the stop to the end

    if (path1.empty() || path2.empty()) { // returns an empty vector if any of the paths are empty
        return route;
    }

    // adds the first path to the route and then adds the second one starting at 1 to avoid printing stop twice
    for (int i=0; i<path1.size();i++) {
        route.push_back(path1[i]);
    }
    for (int i=1; i<path2.size();i++) {
        route.push_back(path2[i]);
    }

    return route;
}

// Visit reachable nodes and return to first node (DFS) : OP 3
vector<string> SocialNetwork::round_trip(const string& start) {
    vector<string>path;
    // it returns empty if start does not exist 
    if (!hasUser(start)) {
        return path;
    }
    int startID = userToID[start];
    vector <bool> visited(adjList.size(), false);

    visited[startID]= true;
    path.push_back(idToUser[startID]);

    int current = startID;
    bool keepGoing = true;

    // loop to contunue visiting until it's done
    while (keepGoing) { 
        keepGoing = false;

        for (int i=0; i<adjList[current].size(); i++) {
            int friendID = adjList[current][i];
            // move to the friend if it's not visited
            if (visited[friendID] ==false) {
                visited[friendID]=true;
                path.push_back(idToUser[friendID]);
                current = friendID;
                keepGoing = true;
                break;
            }
        }
        // if there are not more unvisited friends, return to the start
        if (!keepGoing && current != startID) {
            vector<string> returnPath = shortestPath(idToUser[current],start);

            // starts at 1 to avoid duplicating the current node
            for (int i=1; i<returnPath.size();i++) {
                path.push_back(returnPath[i]);
            }
        }
    }
    return path;
}

// OP 4: Meeting location

// Verifies that the three users exist
string SocialNetwork::findMeetingPoint(const string& user1, const string& user2, const string& user3) {
    if (!hasUser(user1) || (!hasUser(user2) || (!hasUser(user3)))) {
        return "";
    }
    // Get IDs for the three users
    int ID1 = userToID[user1];
    int ID2 = userToID[user2];
    int ID3 = userToID[user3];

    // create empty vectors to store distances and paths for each user
    vector<int>dist1,dist2,dist3;
    vector<int>p1,p2,p3;

    // To fill the vectors with distances from start node
    bfs(ID1,dist1,p1);
    bfs(ID2,dist2,p2);
    bfs(ID3,dist3,p3);

    // declared variables to track the best meeting point
    // initialized in -1 because we don't have a valid point yet
    int best = -1;
    int minimumDist = 99999999;

    // Started a loop throught every user to see if they are a good meeting point
    for (int i=0;i<adjList.size(); i++) {
        // skip the three users 
        if (i== ID1 || i==ID2 || i==ID3) {
            continue;
        }
        // return -1 if there is no path 
        if (dist1[i]==-1 || dist2[i]==-1 || dist3[i]==-1) {
            continue;
        }
        // do a sum of the distances of the three users
        int totalDistance = dist1[i]+dist2[i]+dist3[i];

        if (totalDistance < minimumDist) {
            minimumDist = totalDistance;
            best =i; 
        }
    }
    // return the name and not the ID if there is a valid meeting point 
    if (best!= -1) {
        return idToUser[best];
    }
    return "";
}
