// O(N) space complexity, O(NlogK) time to process a string of length N, K is
// alphabet size Can change to O(NK) space complexity, O(N) time to process a
// string of length N, replace map by array in node_SA

template <int MAXLEN = 1000000>
struct SuffixAutomaton {
  struct node_SA {
    int len, link, cnt;
    int next[26];  // map<char, int> next;
    node_SA() {
      for (int i = 0; i < 26; i++) next[i] = -2;
    }
  };
  vector<node_SA> v;
  int sz, last;
  SuffixAutomaton(int MAX_SIZE = MAXLEN) : sz(1), last(0), v(2 * MAX_SIZE + 5) {
    v[0].len = 0, v[0].link = -1;
  }
  int minlen(const int& idx) {
    return (v[idx].link == -1 ? 0 : v[v[idx].link].len + 1);
  }
  int minlen(const node_SA& n) {
    return (n.link == -1 ? 0 : v[n.link].len + 1);
  }
  void add_char(char c) {
    int cur = sz++;
    v[cur].len = v[last].len + 1;
    v[cur].cnt = 1;
    int temp = last;
    while (temp != -1 && v[temp].next[c - 'a'] == -2) {
      v[temp].next[c - 'a'] = cur;
      temp = v[temp].link;
    }
    if (temp == -1)
      v[cur].link = 0;
    else {
      int nx = v[temp].next[c - 'a'];
      if (v[temp].len + 1 == v[nx].len)
        v[cur].link = nx;
      else {
        int clone = sz++;
        v[clone].len = v[temp].len + 1;
        v[clone].link = v[nx].link;
        for (int i = 0; i < 26; i++) v[clone].next[i] = v[nx].next[i];
        while (temp != -1 && v[temp].next[c - 'a'] == nx) {
          v[temp].next[c - 'a'] = clone;
          temp = v[temp].link;
        }
        v[nx].link = v[cur].link = clone;
      }
    }
    last = cur;
  }
  void build(std::string& s) {
    for (char c : s) add_char(c);
  }
};
