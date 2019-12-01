#include <vector>

using namespace std;

// Node
struct Node
{
  int no;                  //< A unique number for this node
  int mem;                 //< memory consumption for this node
  vector<Node*> dependers; //< Nodes that will run after this node
  vector<Node*> dependees; //< Nodes that are already run before this node
};

// The function that you should implement!
//
// Param nodes : A vector of node pointers and each `Node` contains
//               all data for the DAG graph. See also `node.h`.
// Return : A vector of integers of in the order of execution.
//          Integer values are number of Node(`Node::no`).
vector<int> solve(const vector<Node*>& nodes); // User code
