int cnt[maxn];
void dfs(int v, int p, bool keep) {
  int mx = -1, bigChild = -1;
  for (auto u : g[v])
    if (u != p && sz[u] > mx) mx = sz[u], bigChild = u;
  for (auto u : g[v])
    if (u != p && u != bigChild)
      dfs(u, v, 0);
  if (bigChild != -1)
    dfs(bigChild, v, 1);
  for (auto u : g[v])
    if (u != p && u != bigChild)
      for (int p = st[u]; p < ft[u]; p++) cnt[col[ver[p]]]++;
  cnt[col[v]]++;
  if (keep == 0)
    for (int p = st[v]; p < ft[v]; p++) cnt[col[ver[p]]]--;
}
