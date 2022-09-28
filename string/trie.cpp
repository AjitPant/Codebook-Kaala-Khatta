template <int ALPHABET = 2, int PAR = 0>
struct Trie {
  struct Node {
    int next[ALPHABET], par;
    bool ends;
    Node(int par = -1) : ends(false), par(par) {
      for (auto &x : next) x = -1;
    }
  };
  int root;
  vector<Node> nodes;
  Trie() : root(0), nodes(1) {}
  void insert(vector<int> &v) {
    int cur = root;
    for (auto &x : v) {
      if (nodes[cur].next[x] == -1)
        nodes.push_back(Node(cur)), nodes[cur].next[x] = (int)nodes.size() - 1;
      cur = nodes[cur].next[x];
    }
    nodes[cur].ends = true;
  }
  void insert(long long num, int bits = PAR) {  // for xor insertion
    vector<int> rem;
    while (num) {
      rem.push_back(num % 2);
      num /= 2;
    }
    while (rem.size() < bits) rem.push_back(0);
    reverse(all(rem));
    insert(rem);
  }
  void insert(string &s) {  // string insertion
    vector<int> v;
    for (auto c : s) v.push_back(c - PAR);
    insert(v);
  }
};
