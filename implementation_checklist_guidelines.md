# Development Task Board

## 1. Graph Data Structure & I/O

* \[ \] **\[Silvana\]** Define and implement graph as **undirected** (mutual connections).

* \[ \] **\[Silvana\]** Implement graph using an **Adjacency List**.

* \[ \] **\[Silvana\]** Write input parsing logic (format: one connection per line, e.g., `A B`). Document format in README.

* \[ \] **\[Silvana\]** Implement debug function to print all parsed connections immediately after file load. Include destination-only vertices.

## 2. Core Algorithms

* \[ \] **\[Samuel\]** Implement base unweighted BFS.

* \[ \] **\[Samuel\]** Add predecessor/parent node tracking to BFS for route reconstruction.

* \[ \] **\[Samuel\]** Standardize distance logic (e.g., 4 vertices = 3 edges).

## 3. Required Operations

* \[ \] **\[Samuel\]** **Op 1 (Fewest Connections):** Implement BFS to verify reachability and find shortest route.

* \[ \] **\[Samuel\]** **Op 2 (Two Required Locations):** Implement logic to calculate and compare paths `Start -> Req1 -> Req2 -> End` and `Start -> Req2 -> Req1 -> End`. Return the shortest valid complete path.

* \[ \] **\[Silvana\]** **Op 3 (Visit All Reachable & Return):** Implement round-trip traversal. Terminate upon first return to start. Document any approximation algorithm limitations in README.

* \[ \] **\[Silvana\]** **Op 4 (Meeting Location):** Execute BFS from 3 start nodes. Intersect reachable sets (excluding start nodes). Calculate and output the meeting node with the lowest combined edge count. Retain paths for printing.

## 4. Test Datasets

* \[ \] **\[Samuel\]** Create manual test dataset (5-10 vertices) for verifying basic operations and edge cases.

* \[ \] **\[Silvana\]** Create scale test dataset (20-50 vertices) for benchmarking runtimes and stress-testing complex operations.

## 5. Edge Case Testing Suite

* \[ \] **\[Samuel\]** Test: Direct connection exists.

* \[ \] **\[Samuel\]** Test: Multi-connection route.

* \[ \] **\[Silvana\]** Test: Unreachable destination (disconnected components).

* \[ \] **\[Silvana\]** Test: Graph containing a cycle.

* \[ \] **\[Samuel\]** Test: Multiple routes with identical minimum lengths (ties).

* \[ \] **\[Silvana\]** Test: No valid round trip exists (Op 3).

* \[ \] **\[Silvana\]** Test: No valid meeting location exists (Op 4).

## 6. Resources

* BFS C++: [Runestone Academy](https://runestone.academy/ns/books/published/cppds/Graphs/ImplementingBreadthFirstSearch.html)

* Traversal/Routes: [Open Data Structures](https://www.opendatastructures.org/ods-cpp/12_3_Graph_Traversal.html)

* I/O Streams: [basic_ifstream](https://learn.microsoft.com/en-us/cpp/standard-library/basic-ifstream-class?view=msvc-170) | [basic_istringstream](https://learn.microsoft.com/en-us/cpp/standard-library/basic-istringstream-class?view=msvc-170)