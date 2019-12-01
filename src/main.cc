#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <unordered_set>

#include "common.h"

using namespace std;

int eval(vector<Node*> nodes, const vector<int> &sol)
{
  struct EvalNode
  {
    Node* node = nullptr;
    unordered_set<EvalNode*> dependers;
    unordered_set<EvalNode*> dependees;

    int alloc_cnt = 0;
    int free_cnt = 0;
    bool executed = false;
  };

  vector<EvalNode*> enodes(nodes.size(), nullptr);
  for (auto node : nodes) {
    auto enode = new EvalNode;
    enode->node = node;
    enodes[node->no] = enode;
  }
  for (auto enode : enodes) {
    auto node = enode->node;
    for (auto dependee : node->dependees)
      enode->dependees.insert(enodes[dependee->no]);
    for (auto depender : node->dependers)
      enode->dependers.insert(enodes[depender->no]);
    enode->alloc_cnt = node->dependees.size();
    enode->free_cnt = node->dependers.size();
  }

  int peak_mem = 0;
  int cur_mem = 0;

  auto update_mem = [&] (int amount) {
    cur_mem += amount;
    peak_mem = max(peak_mem, cur_mem);
    cerr << "amount: " << setw(8) << amount
         << " | cur: " << setw(8) << cur_mem
         << " | peak: " << setw(8) << peak_mem << endl;
  };

  string exec_order;

  auto throw_with_msg = [&](const std::string &msg) {
    string throw_msg = msg + "\nExecution Order : { " + exec_order + "}";
    throw throw_msg;
  };

  if (sol.size() != nodes.size())
    throw_with_msg("Number of nodes in your solution does not match the number of all nodes");

  for (auto node_no : sol)
  {
    exec_order += to_string(node_no) + " ";
    cerr << exec_order << endl;
    if (node_no > nodes.size())
      throw_with_msg("Invalid index " + std::to_string(node_no));

    auto enode = enodes[node_no];

    if (enode->alloc_cnt != 0)
      throw_with_msg("Invalid order of execution, please check if your order is topological");
    if (enode->executed)
      throw_with_msg("Node " + std::to_string(node_no) + " is executed twice");

    enode->executed = true;
    update_mem(enode->node->mem);

    // Notify finished to dependers (for alloc)
    for (auto depender : enode->dependers) {
      assert(depender->alloc_cnt > 0);
      depender->alloc_cnt--;
    }

    // Notify finished to dependees (for free)
    for (auto dependee : enode->dependees) {
      assert(dependee->free_cnt > 0);
      dependee->free_cnt--;
      // Free if no more nodes are waiting
      if (dependee->free_cnt == 0) {
        update_mem(-dependee->node->mem);
      }
    }
    // Free the output nodes
    // if (enode->free_cnt == 0) {
    //   update_mem(-enode->node->mem);
    // }
  }

  // assert(cur_mem == 0);

  for (auto e : enodes)
    delete e;

  return peak_mem;
}

int main()
{
  cerr << "<Start>" << endl;

  // Input processing
  int num_nodes, deps;
  cin >> num_nodes >> deps;
  vector<Node*> nodes(num_nodes);
  for (int i = 0; i < nodes.size(); i++) {
    int mem;
    cin >> mem;
    nodes[i] = new Node{i, mem, {}, {}};
  }
  while (deps--) {
    int dependee_no, depender_no;
    cin >> dependee_no >> depender_no;
    assert(dependee_no < num_nodes);
    assert(depender_no < num_nodes);
    auto depender = nodes[depender_no];
    auto dependee = nodes[dependee_no];
    depender->dependees.push_back(dependee);
    dependee->dependers.push_back(depender);
  }

  // Solving
  auto ret = solve(nodes);

  // Evaluation
  try {
    int ans = eval(nodes, ret);
    cout << ans << endl;
    for (auto e : ret)
      cout << e << " ";
    cout << endl;
  } catch (const std::string &msg) {
    cout << -1 << endl;
    cerr << "==== SOLUTION ERROR ====" << endl;
    cerr << msg << endl;
  }

  // Cleanup
  for (auto e : nodes) {
    delete e;
  }
}
