const int MAXLEN = 4e5+5;
template<int ALPHABET = 26, int LOW = 'a'>
struct SuffixArray {
	vector<int> sa, order, lcp, locate;
	vector<vector<int>> sparse;
	string _s;
	SuffixArray() {} 
	void build(string s) {
		s += (char)(LOW-1);	
		int n = s.size();
		_s = s;
		sa.resize(n); order.resize(n);
		vector<vector<int>> pos(ALPHABET+1);
		for(int i=0; i<n; i++) 
			pos[s[i] - LOW + 1].push_back(i);

		int idx = -1, o_idx = -1;
		for(int i=0; i<ALPHABET+1; i++) {
			o_idx += (pos[i].size() > 0);
			for(auto& x: pos[i]) 
				order[x] = o_idx, sa[++idx] = x;
		}
		int cur = 1;
		while(cur < n) {
			cur *= 2;
			vector<pair<pair<int, int>, int>> w(n);
			vector<int> cnt(n), st(n), where(n);
			for(int i=0; i<n; i++) {
				int from = sa[i] - cur/2 + n; if(from >= n) from -= n;
				w[i] = {{order[from], order[sa[i]]}, from}; 
				cnt[order[from]]++;       where[from] = i;
			}
			for(int i=1; i<n; i++) 
				st[i] = st[i-1] + cnt[i-1];	
			for(int i=0; i<n; i++) 
				sa[st[w[i].first.first]++] = w[i].second; 

			order[sa[0]] = 0;
			for(int i=1; i<n; i++)
				order[sa[i]] = order[sa[i-1]] + (w[where[sa[i]]].first != w[where[sa[i-1]]].first);		
		}
	}
	void build_lcp() {
		int n = sa.size();
		lcp.resize(n); locate.resize(n);
		for(int i=0; i<n; i++)
			locate[sa[i]] = i;
		for(int i=0; i<n-1; i++) {
			int wh = locate[i], up = sa[wh-1];
			if(i > 0)
				lcp[wh] = max(lcp[wh], lcp[locate[i-1]]-1);
			while(_s[i+lcp[wh]] == _s[up+lcp[wh]])
				++lcp[wh];
		}
	}
	void build_sparse() {
		int n = _s.size();
		sparse.resize(20, vector<int>(n)); 
		for(int i=0; i<n; i++)
			sparse[0][i] = lcp[i];
		for(int i=1, len=2; i<20; i++, len *= 2)
			for(int j=0; j+len <=n; j++)
				sparse[i][j] = min(sparse[i-1][j], sparse[i-1][j+len/2]);
	}
	int find_lcp(int a, int b) {
		if(a == b) return _s.size()-1-a;				//-1 because sentinel is added to string 
		a = locate[a]; b = locate[b];
		if(a > b) { swap(a, b); } a++;
		int which = log2(b-a+1);
		return min(sparse[which][a], sparse[which][b-(1<<which)+1]);
	}
};
