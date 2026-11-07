#include "socialNetwork.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <sstream>

AdjacencyList::AdjacencyList(): heads(nullptr), vertexCount(0), capacity(0), friendshipCount(0) {}

AdjacencyList::~AdjacencyList() {
    for (int user = 0; user < vertexCount; ++user) {
        Node* current = heads[user];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] heads;
}

void AdjacencyList::ensureCapacity(int requiredCapacity) {
    if (requiredCapacity <= capacity) {
        return;
    }

    int newCapacity = capacity == 0 ? 4 : capacity;
    while (newCapacity < requiredCapacity) {
        newCapacity *= 2;
    }

    Node** newHeads = new Node*[newCapacity];
    for (int i = 0; i < newCapacity; ++i) {
        newHeads[i] = i < vertexCount ? heads[i] : nullptr;
    }
    delete[] heads;
    heads = newHeads;
    capacity = newCapacity;
}

int AdjacencyList::addVertex() {
    ensureCapacity(vertexCount + 1);
    heads[vertexCount] = nullptr;
    return vertexCount++;
}

bool AdjacencyList::hasEdge(int from, int to) const {
    if (from < 0 || from >= vertexCount) {
        return false;
    }

    for (const Node* current = heads[from]; current != nullptr; current = current->next) {
        if (current->userID == to) {
            return true;
        }
    }
    return false;
}

void AdjacencyList::appendNeighbor(int userID, int neighborID) {
    Node* newNode = new Node;
    newNode->userID = neighborID;
    newNode->next = nullptr;

    if (heads[userID] == nullptr) {
        heads[userID] = newNode;
        return;
    }

    Node* last = heads[userID];
    while (last->next != nullptr) {
        last = last->next;
    }
    last->next = newNode;
}

bool AdjacencyList::addUndirectedEdge(int first, int second) {
    if (first < 0 || first >= vertexCount || second < 0 ||
        second >= vertexCount || first == second || hasEdge(first, second)) {
        return false;
    }

    appendNeighbor(first, second);
    appendNeighbor(second, first);
    ++friendshipCount;
    return true;
}

int AdjacencyList::size() const {
    return vertexCount;
}

int AdjacencyList::edgeCount() const {
    return friendshipCount;
}

const AdjacencyList::Node* AdjacencyList::neighbors(int userID) const {
    if (userID < 0 || userID >= vertexCount) {
        return nullptr;
    }
    return heads[userID];
}

// Returns the ID for a name. If they don't exist yet, it adds them.
int SocialNetwork::getUser(const string& name) {
    // If user already exists, return their ID
    if (userToID.count(name) > 0) {
        return userToID[name];
    }
    
    // Otherwise, create a new user
    int newID = adjList.addVertex();
    userToID[name] = newID;
    idToUser.push_back(name);
    
    return newID;
}

// Reads friendships from a text file
bool SocialNetwork::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        ++lineNumber;
        istringstream lineStream(line);
        string user1, user2, extra;

        if (!(lineStream >> user1)) {
            continue;
        }
        if (!(lineStream >> user2) || (lineStream >> extra)) {
            cerr << "Error: malformed input in '" << filename << "' at line "
                 << lineNumber << "; expected exactly two usernames.\n";
            return false;
        }

        if (user1 == user2) {
            continue;
        }

        int id1 = getUser(user1);
        int id2 = getUser(user2);
        adjList.addUndirectedEdge(id1, id2);
    }

    if (file.bad()) {
        cerr << "Error: failed while reading '" << filename << "'.\n";
        return false;
    }
    return true;
}

bool SocialNetwork::hasUser(const string& name) {
    return userToID.count(name) > 0;
}

int SocialNetwork::getNumUsers() {
    return adjList.size();
}

int SocialNetwork::getNumFriendships() {
    return adjList.edgeCount();
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
        for (const AdjacencyList::Node* edge = adjList.neighbors(current);
             edge != nullptr; edge = edge->next) {
            int neighbor = edge->userID;
            
            // If we haven't visited this neighbor yet
            if (distances[neighbor] == -1) {
                distances[neighbor] = distances[current] + 1;
                parents[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
}

vector<string> SocialNetwork::reconstructPath(int target, const vector<int>& parents) {
    vector<string> reversedPath;
    for (int current = target; current != -1; current = parents[current]) {
        reversedPath.push_back(idToUser[current]);
    }

    vector<string> path;
    for (vector<string>::size_type i = reversedPath.size(); i > 0; --i) {
        path.push_back(reversedPath[i - 1]);
    }
    return path;
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
    
    return reconstructPath(targetID, parents);
}

vector<string> SocialNetwork::shortestPathUnderLimit(
    const string& from, const string& to, int connectionLimitExclusive) {
    if (connectionLimitExclusive <= 0) {
        return vector<string>();
    }

    vector<string> path = shortestPath(from, to);
    if (path.empty() ||
        path.size() - 1 >= static_cast<vector<string>::size_type>(connectionLimitExclusive)) {
        return vector<string>();
    }

    return path;
}

// Prints the list of the social network
void SocialNetwork::printPath() {
    for (int i = 0; i < adjList.size(); i++) {
        cout << "[" << i << "] " << idToUser[i] << " --> ";
        for (const AdjacencyList::Node* edge = adjList.neighbors(i);
             edge != nullptr; edge = edge->next) {
            cout << idToUser[edge->userID] << " ";
        }
        cout << endl;
    }
}

// Finds the shortest route through both required users in either order.
vector<string> SocialNetwork::shortestRoute(const string& start,
                                           const string& required1,
                                           const string& required2,
                                           const string& end) {
    auto buildRoute = [this, &start, &end](
                          const string& firstRequired,
                          const string& secondRequired) {
        vector<vector<string>> segments;
        segments.push_back(shortestPath(start, firstRequired));
        segments.push_back(shortestPath(firstRequired, secondRequired));
        segments.push_back(shortestPath(secondRequired, end));

        vector<string> route;
        for (vector<vector<string>>::size_type i = 0; i < segments.size(); ++i) {
            if (segments[i].empty()) {
                return vector<string>();
            }

            vector<string>::size_type firstUser = route.empty() ? 0 : 1;
            route.insert(route.end(), segments[i].begin() + firstUser,
                         segments[i].end());
        }
        return route;
    };

    vector<string> route1 = buildRoute(required1, required2);
    vector<string> route2 = buildRoute(required2, required1);

    if (route1.empty()) {
        return route2;
    }
    if (route2.empty()) {
        return route1;
    }

    // Both routes have the same endpoints, so fewer users means fewer edges.
    // On a tie, keep the order in which the required users were provided.
    return route1.size() <= route2.size() ? route1 : route2;
}

// Walk the BFS spanning tree out and back, visiting every reachable user.
vector<string> SocialNetwork::round_trip(const string& start) {
    if (!hasUser(start)) {
        return vector<string>();
    }

    int startID = userToID[start];
    vector<int> distances;
    vector<int> parents;
    bfs(startID, distances, parents);

    vector<string> path(1, start);
    vector<int> stack(1, startID);
    vector<const AdjacencyList::Node*> nextNeighbor(1, adjList.neighbors(startID));

    while (!stack.empty()) {
        const AdjacencyList::Node* edge = nextNeighbor.back();
        while (edge != nullptr && parents[edge->userID] != stack.back()) {
            edge = edge->next;
        }

        if (edge != nullptr) {
            nextNeighbor.back() = edge->next;
            int child = edge->userID;
            path.push_back(idToUser[child]);
            stack.push_back(child);
            nextNeighbor.push_back(adjList.neighbors(child));
        } else {
            stack.pop_back();
            nextNeighbor.pop_back();
            if (!stack.empty()) {
                path.push_back(idToUser[stack.back()]);
            }
        }
    }
    return path;
}

MeetingResult SocialNetwork::findMeetingPoint(
    const string& user1, const string& user2, const string& user3) {
    MeetingResult result;
    if (!hasUser(user1) || (!hasUser(user2) || (!hasUser(user3)))) {
        return result;
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
    int minimumDist = numeric_limits<int>::max();

    // Started a loop throught every user to see if they are a good meeting point
    for (int i = 0; i < adjList.size(); ++i) {
        // skip the three users 
        if (i == ID1 || i == ID2 || i == ID3) {
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
            best = i;
        }
    }
    if (best != -1) {
        result.meetingPoint = idToUser[best];
        result.pathFromUser1 = reconstructPath(best, p1);
        result.pathFromUser2 = reconstructPath(best, p2);
        result.pathFromUser3 = reconstructPath(best, p3);
    }
    return result;
}
