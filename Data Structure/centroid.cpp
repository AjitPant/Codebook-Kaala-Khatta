struct Graph {
	vector<vector<int>> adj;
	Graph(int n): adj(n+1) {}
	void add_edge(int a, int b, bool directed = false) {
		adj[a].pb(b);
		if(!directed) adj[b].pb(a);
	}
};

struct Centroid {
	vector<int> stree, parent;
	void _dfs(vector<vector<int>> &adj, ll x, ll par=-1) {
		stree[x] = 1, parent[x] = par;
		for(auto &p: adj[x]) {
			if(p != par) {
				_dfs(adj, p, x);
				stree[x] += stree[p];
			}
		}
	}
	int decompose(Graph &G, Graph &cd, ll root=1) {
		int n = G.adj.size()-1;
		stree.resize(n+1);
		parent.resize(n+1);
		_dfs(G.adj, root);
		vector<bool> done(n+1);
		return construct(G, cd, done, root);
	}
	int construct(Graph &G, Graph &cd, vector<bool> &done, ll root) {
		while(true) {
			ll maxm = 0, ind = -1;
			for(auto &x: G.adj[root]) {
				if(!done[x] && stree[x]>maxm) {
					maxm = stree[x];
					ind = x;
				}
			}
			if(maxm <= stree[root]/2) {
				done[root] = true;
				for(auto &p: G.adj[root]) {
					if(!done[p]) {
						ll x = construct(G, cd, done, p);
						cd.add_edge(x, root);
						// root is parent of x is centroid tree
						// cd.parent[x] = root;	
					}
				}
				return root;
			}
			else {
				ll temp = stree[root];
				stree[root] -= stree[ind];
				stree[ind] = temp;
				root = ind;
			} 
		}
	}
};
