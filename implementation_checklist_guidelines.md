# Development Task Board

## 1. Graph Data Structure & I/O

* \[x\] **\[Silvana\]** Define and implement graph as **undirected** (mutual connections).

* \[x\] **\[Silvana\]** Implement graph using an **Adjacency List**.

* \[x\] **\[Silvana\]** Write input parsing logic (format: one connection per line, e.g., `A B`). Document format in README.

* \[x\] **\[Silvana\]** Implement debug function to print all parsed connections immediately after file load. Include destination-only vertices.

## 2. Core Algorithms

* \[x\] **\[Samuel\]** Implement base unweighted BFS.

* \[x\] **\[Samuel\]** Add predecessor/parent node tracking to BFS for route reconstruction.

* \[x\] **\[Samuel\]** Standardize distance logic (e.g., 4 vertices = 3 edges).

## 3. Required Operations

* \[x\] **\[Samuel\]** **Op 1 (Fewest Connections):** Implement BFS to verify reachability and find shortest route.

* \[x\] **\[Samuel\]** **Op 2 (Two Required Locations):** Implement logic to calculate and compare paths `Start -> Req1 -> Req2 -> End` and `Start -> Req2 -> Req1 -> End`. Return the shortest valid complete path.

* \[x\] **\[Silvana\]** **Op 3 (Visit All Reachable & Return):** Implement round-trip traversal. Terminate upon first return to start. Document any approximation algorithm limitations in README.

* \[x\] **\[Silvana\]** **Op 4 (Meeting Location):** Execute BFS from 3 start nodes. Intersect reachable sets (excluding start nodes). Calculate and output the meeting node with the lowest combined edge count. Retain paths for printing.

## 4. Test Datasets

## 5. Edge Case Testing Suite

* \[x\] **\[Samuel\]** Test: Direct connection exists.

* \[x\] **\[Samuel\]** Test: Multi-connection route.

* \[x\] **\[Silvana\]** Test: Unreachable destination (disconnected components).

* \[x\] **\[Silvana\]** Test: Graph containing a cycle.

* \[x\] **\[Samuel\]** Test: Multiple routes with identical minimum lengths (ties).

* \[x\] **\[Silvana\]** Test: Round trip visits all reachable users and returns to its start (Op 3).

* \[x\] **\[Silvana\]** Test: No valid meeting location exists (Op 4).

## 6. Resources

* BFS C++: [Runestone Academy](https://runestone.academy/ns/books/published/cppds/Graphs/ImplementingBreadthFirstSearch.html)

* Traversal/Routes: [Open Data Structures](https://www.opendatastructures.org/ods-cpp/12_3_Graph_Traversal.html)

* I/O Streams: [basic_ifstream](https://learn.microsoft.com/en-us/cpp/standard-library/basic-ifstream-class?view=msvc-170) | [basic_istringstream](https://learn.microsoft.com/en-us/cpp/standard-library/basic-istringstream-class?view=msvc-170)