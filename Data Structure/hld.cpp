struct HLD {
  vector<int> sz, tin, tout, nxt, order, level, pars;
  int timer;
  // SegTree ST;
  void dfs(vector<vector<int>> &adj, int x, int par = -1) {
    sz[x] = 1;
    pars[x] = par;
    for (auto &p : adj[x])
      if (p != par) {
        level[p] = level[x] + 1;
        dfs(adj, p, x);
        if (adj[x][0] == par || sz[p] > sz[adj[x][0]]) swap(p, adj[x][0]);
      }
  }
  void dfs2(vector<vector<int>> &adj, int x) {
    tin[x] = timer++;
    order.push_back(x);
    for (auto &p : adj[x]) {
      if (p == pars[x]) continue;
      nxt[p] = (p == adj[x][0] ? nxt[x] : p);
      dfs2(adj, p);
    }
    tout[x] = timer;
  }
  HLD(vector<vector<int>> &adj, int N, int root = 1)
      : sz(N + 5),
        tin(N + 5),
        tout(N + 5),
        nxt(N + 5),
        level(N + 5),
        pars(N + 5),
        timer(0) {
    int n = adj.size() - 1;
    level[root] = 0;
    dfs(adj, root);
    dfs2(adj, root);
    // build segment tree on "order" here
    //  ST.resize(order.size());
    //  ST.build(0, 0, order.size()-1, order);
  }
  int path_query(int a, int b) {
    int N = order.size();
    // int answer = -INFINT;
    while (nxt[a] != nxt[b]) {
      if (level[nxt[a]] < level[nxt[b]]) swap(a, b);
      // answer = max(answer, ST.range_query(0, 0, N-1, tin[nxt[a]], tin[a]));
      a = pars[nxt[a]];
    }
    if (tin[a] > tin[b]) swap(a, b);
    // answer = max(answer, ST.range_query(0, 0, N-1, tin[a], tin[b]));
    return answer;
  }
  void point_update(int x, int val) {
    // ST.point_update(0, 0, order.size()-1, tin[x], val);
  }
};
