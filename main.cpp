#include "socialNetwork.h"
#include <iostream>
#include <sstream>

using namespace std;

void printPath(const vector<string>& path) {
    for (vector<string>::size_type i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << path[i];
    }
    cout << "  (" << path.size() - 1 << " connections)\n";
}

int main(int argc, char* argv[]) {
    string filename = argc > 1 ? argv[1] : "friends.txt";

    SocialNetwork net;
    if (!net.loadFromFile(filename)) {
        cerr << "Error: could not load file '" << filename << "'.\n";
        return 1;
    }

    cout << "Loaded " << net.getNumUsers() << " users and "
         << net.getNumFriendships() << " friendships.\n\n";
    net.printPath();
    cout << "\nCommands:\n";
    cout << "  path <user1> <user2>\n";
    cout << "  within <user1> <user2> <max_connections_exclusive>\n";
    cout << "  route <start> <required1> <required2> <end>\n";
    cout << "  roundtrip <user>\n";
    cout << "  meet <user1> <user2> <user3>\n";
    cout << "  quit\n";

    string line;
    while (cout << "> " && getline(cin, line)) {
        istringstream input(line);
        string command;
        if (!(input >> command)) {
            continue;
        }
        if (command == "quit") {
            string extra;
            if (input >> extra) {
                cout << "Error: quit takes no arguments.\n";
            } else {
                break;
            }
        } else if (command == "path") {
            string user1, user2, extra;
            if (!(input >> user1 >> user2) || (input >> extra)) {
                cout << "Usage: path <user1> <user2>\n";
                continue;
            }
            if (!net.hasUser(user1) || !net.hasUser(user2)) {
                cout << "Error: One or both users do not exist.\n";
                continue;
            }

            vector<string> path = net.shortestPath(user1, user2);
            if (path.empty()) {
                cout << "No connection between " << user1 << " and " << user2 << ".\n";
            } else {
                printPath(path);
            }
        } else if (command == "within") {
            string user1, user2, extra;
            int connectionLimitExclusive;
            if (!(input >> user1 >> user2)) {
                cout << "Usage: within <user1> <user2> <max_connections_exclusive>\n";
                continue;
            }
            if (!(input >> connectionLimitExclusive) || (input >> extra)) {
                cout << "Error: within requires an integer limit and no extra arguments.\n";
                continue;
            }
            if (!net.hasUser(user1) || !net.hasUser(user2)) {
                cout << "Error: One or both users do not exist.\n";
                continue;
            }

            vector<string> path = net.shortestPathUnderLimit(
                user1, user2, connectionLimitExclusive);
            if (path.empty()) {
                cout << "No path with fewer than " << connectionLimitExclusive
                     << " connections between " << user1 << " and " << user2 << ".\n";
            } else {
                printPath(path);
            }
        } else if (command == "route") {
            string start, required1, required2, end, extra;
            if (!(input >> start >> required1 >> required2 >> end) || (input >> extra)) {
                cout << "Usage: route <start> <required1> <required2> <end>\n";
                continue;
            }
            if (!net.hasUser(start) || !net.hasUser(required1) ||
                !net.hasUser(required2) || !net.hasUser(end)) {
                cout << "Error: One or more users do not exist.\n";
                continue;
            }

            vector<string> route = net.shortestRoute(start, required1, required2, end);
            if (route.empty()) {
                cout << "No complete route exists through " << required1 << " and "
                     << required2 << " from " << start << " to " << end << ".\n";
            } else {
                printPath(route);
            }
        } else if (command == "roundtrip") {
            string user, extra;
            if (!(input >> user) || (input >> extra)) {
                cout << "Usage: roundtrip <user>\n";
                continue;
            }
            if (!net.hasUser(user)) {
                cout << "Error: User does not exist.\n";
                continue;
            }

            vector<string> roundTrip = net.round_trip(user);
            if (roundTrip.empty()) {
                cout << "Error generating roundtrip.\n";
            } else {
                printPath(roundTrip);
            }
        } else if (command == "meet") {
            string user1, user2, user3, extra;
            if (!(input >> user1 >> user2 >> user3) || (input >> extra)) {
                cout << "Usage: meet <user1> <user2> <user3>\n";
                continue;
            }
            if (!net.hasUser(user1) || !net.hasUser(user2) || !net.hasUser(user3)) {
                cout << "Error: Not all three users exist.\n";
                continue;
            }

            MeetingResult result = net.findMeetingPoint(user1, user2, user3);
            if (result.meetingPoint.empty()) {
                cout << "Error: No meeting location found for the three users.\n";
            } else {
                cout << "Best meeting location is: " << result.meetingPoint << "\n";
                cout << user1 << "'s path: ";
                printPath(result.pathFromUser1);
                cout << user2 << "'s path: ";
                printPath(result.pathFromUser2);
                cout << user3 << "'s path: ";
                printPath(result.pathFromUser3);
            }
        } else {
            cout << "Unknown command.\n";
        }
    }

    return 0;
}
