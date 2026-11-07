# Social Network Connection Search

**Class:** CSCE 2110 - Foundations of Data Structures

**Team:** Silvana Velazquez Vivanco and Samuel Lucena Godoi


## What the program does

The program reads a file of friendships and lets you search the network. Since
the graph is unweighted, it uses BFS to find paths with the fewest connections.
You can also ask for a route through two people, a round trip through everyone
reachable from one person, or a meeting point for three people.

## How to compile and run

You need a C++11 compiler. The commands below use `g++` and only the standard
C++ library. Run them from the project folder:

```sh
g++ socialNetwork.cpp main.cpp -o main
./main friends.txt
```

To use the other data file, run:

```sh
./main friends2.txt
```

If you leave out the file name, the program tries to open `friends.txt` in the
folder where it was started:

```sh
./main
```

## Input files

The project has two data files:

- `friends.txt`: 11 users and 11 friendships.
- `friends2.txt`: 26 users and 31 friendships, with cycles and separate groups.

Each line should have two names separated by spaces or tabs. For example:

```text
Alice Bob
Alice Carlos
```

Friendships go both ways. If a pair is repeated or written in reverse, it is
counted once. Self-friendships are ignored. A line with anything other than two
names is treated as an error, and the program prints the line number.

## Trying the commands

After starting the program, type commands at the prompt. For example, with
`friends.txt`:

```text
path Alice Diana
within Alice Diana 3
route Alice Bob Carlos Diana
roundtrip Alice
meet Alice Bob Carlos
quit
```

- `path A B` finds a shortest route from A to B.
- `within A B 3` only prints a route if it has fewer than 3 connections. The
  limit is exclusive.
- `route Start Person1 Person2 End` tries both orders for the two required
  people and prints the shorter complete route.
- `roundtrip Start` visits everyone reachable from Start, then returns to Start.
- `meet A B C` finds a common meeting person with the lowest total distance and
  prints each person's path there.
- `quit` ends the program. It also exits if the input ends.

The number after a printed path counts connections, not people. For example,
`A -> B -> C` is 3 people but 2 connections.

## How the graph is stored

The graph is stored in a adjacency list. Each person gets an integer
ID. An array holds the first linked-list node for each ID, and the nodes
hold the IDs of that person's friends. We use a map to look up a person's ID and
a vector to get their name back when printing.

The adjacency list takes O(V + E) space, where V is the number
of people and E is the number of friendships. One tradeoff is that looking for a
duplicate friendship scans the person's friend list. The code also has to
allocate and free the linked-list nodes itself.

## Testing

We tried the operations using both supplied data files and tested extra cases such as direct
paths, disconnected users, cycles, duplicate friendships, and no possible
meeting point. 


