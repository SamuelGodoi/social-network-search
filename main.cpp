#include "socialNetwork.h"
#include <iostream>

using namespace std;

void printPath(const vector<string>& path) {
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << path[i];
    }
    cout << "  (" << path.size() - 1 << " connections)\n";
}

int main(int argc, char* argv[]) {
    string filename = "friends.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    SocialNetwork net;
    
    if (!net.loadFromFile(filename)) {
        cout << "Error: could not open file '" << filename << "'\n";
        return 1;
    }
    
    cout << "Loaded " << net.getNumUsers() << " users and " 
         << net.getNumFriendships() << " friendships.\n\n";

         net.printPath();
         cout << endl;

    cout << "Commands:\n";
    cout << "  path <user1> <user2>\n";
    cout << " route <user1> <user1> <stop> <user2>\n";
    cout << "  quit\n\n";

    string cmd;
    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "quit") {
            break;
        } 
        else if (cmd == "path") {
            string user1, user2;
            cin >> user1 >> user2;

            if (!net.hasUser(user1) || !net.hasUser(user2)) {
                cout << "Error: one or both users do not exist.\n";
                continue;
            }

            vector<string> path = net.shortestPath(user1, user2);
            
            if (path.empty()) {
                cout << "No connection between " << user1 << " and " << user2 << ".\n";
            } else {
                printPath(path);
            }
        } 

        else if (cmd == "route") {
            string user1, stop, user2;
            cin >> user1 >> stop >> user2;

            if (!net.hasUser(user1) || !net.hasUser(stop) || !net.hasUser(user2)) {
                cout <<"Error: one or both users do not exist.\n";
                continue;
            }
            vector<string> route = net.shortestRoute(user1,stop,user2);
            if (route.empty()) {
                cout << "No connection between " << user1 << stop << ", and " << user2 << ".\n";
            } else {
                printPath(route);
            }
        }

        else {
            cout << "Unknown command.\n";
            // Clear the rest of the line to prevent infinite loop on bad input
            string junk;
            getline(cin, junk); 
        }
    }
    
    return 0;
}