struct LCA_euler {
	vector<int> tin, level;
	vector<pair<int, int>> tour;
	vector<vector<int>> sparse;
	void _dfs(vector<vector<int>> &adj, int x, int par = -1) {
		tin[x] = tour.size(), tour.pb({level[x], x});
		for(auto &p : adj[x])
			if(p != par) {
				level[p] = level[x] + 1;
				_dfs(adj, p, x);
				tour.pb({level[x], x});
			}
	}
	void build_sparse() {
		int cur = 1, N = tour.size();
		int sz = log2(N);
		sparse.resize(sz+1);
		for(int i=0; i<N; i++) sparse[0].pb(i);
		for(int i=1, x=2; i<=sz; i++, x*=2) 
			for(int j=0; j<N-x+1; j++)
				if(tour[sparse[i-1][j]].first < tour[sparse[i-1][j+x/2]].first) 
					sparse[i].pb(sparse[i-1][j]);
				else
					sparse[i].pb(sparse[i-1][j+x/2]);
	}
	void build(vector<vector<int>> &adj, int root = 1) {
		int n = adj.size()-1;
		tin.resize(n+1); level.resize(n+1); 
		_dfs(adj, root);
		build_sparse();
	}
	int query(int a, int b) {
		int which = log2(b-a+1), x = sparse[which][a];
		int y = sparse[which][b + 1 - (1 << which)];
		if(tour[x].first < tour[y].first) return tour[x].second;
		else return tour[y].second;
	}
	int find_lca(int a, int b) {
		int st = tin[a], en = tin[b];
		if(st > en) swap(st, en);
		return query(st, en);
	}
};
