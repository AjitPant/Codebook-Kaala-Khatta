
template<int ALPHABET = 26, int LOW = 'a'>
struct AhoCorasick {
	struct Node {
		int next[ALPHABET], link, parent;
		char ch; bool ends;
		Node(int par = -1, char c = LOW - 1): parent(par), ch(c), link(-1), ends(false) {
			for(int i=0; i<ALPHABET; i++)
				next[i] = -1;
		}
	};
	vector<Node> nodes;
	int root;
	AhoCorasick(): root(0), nodes(1) {}
	void add_string(string &s, int idx) {
		int cur = root;
		for(auto c: s) {
			if(nodes[cur].next[c - LOW] == -1)
				nodes.push_back(Node(cur, c)), nodes[cur].next[c - LOW] = (int)nodes.size()-1;
			cur = nodes[cur].next[c - LOW];
		}
		nodes[cur].leaves.push_back(idx), nodes[cur].ends = true;
	}
	void build_links() {
		queue<int> q; q.push(0);
		while(!q.empty()) {
			int fr = q.front(); q.pop();
			if(nodes[fr].parent <= 0) {
				nodes[fr].link = 0;
				for(int i=0; i<ALPHABET; i++)
					if(nodes[fr].next[i] == -1)
						if(nodes[fr].parent == -1)
							nodes[fr].next[i] = 0;
						else
							nodes[fr].next[i] = nodes[nodes[fr].link].next[i];
					else
						q.push(nodes[fr].next[i]);
			}
			else {
				nodes[fr].link = nodes[nodes[nodes[fr].parent].link].next[nodes[fr].ch - LOW];
				for(int i=0; i<ALPHABET; i++)
					if(nodes[fr].next[i] == -1)
						nodes[fr].next[i] = nodes[nodes[fr].link].next[i];
					else
						q.push(nodes[fr].next[i]);
			}
		}
	}
};
