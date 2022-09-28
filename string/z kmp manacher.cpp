vector<int> kmp(const string &s) {
	int n = (int)s.size(); vector<int> ans(n, 0);
	for(int i=1; i<n; i++) {
		int k = ans[i-1]; 
		while(k && s[k] != s[i]) 
			k = ans[k-1];
		ans[i] = k + (s[k] == s[i]);
	}
	return ans;
}
vector<int> zfunc(const string &s) {
	int n = (int)s.size(); vector<int> z(n, 0); z[0] = n;
	for(int i=1, l=0, r=0; i<n; i++) {
		z[i] = max(0, min(r - i + 1, z[i - l])); 
		while(s[i+z[i]] == s[z[i]]) ++z[i];
		if(i + z[i] - 1 > r)
			l = i, r = i + z[i] - 1;
	}
	return z;
}
pair<vector<int>, vector<int>> manacher(const string &s) {
	string t = "$"; 
	for(auto c: s) t += c, t += '^';			// Only odd manacher will do the trick now
	int N = (int)t.size();  vector<int> ans(N, 1);
	int l = 1, r = 1;
	for(int i=1; i<N; i++) {
		ans[i] = max(0, min(r - i, ans[l + (r - i)]));
		while(t[i - ans[i]] == t[i + ans[i]]) 
			++ans[i];
		if(i + ans[i] > r)
			l = i - ans[i], r = i + ans[i]; 
	}
	vector<int> odd, even;
	for(int i=1; i<N-1; i++) {
		if(i&1) odd.push_back(1 + 2*((ans[i]-1)/2));
		else even.push_back(2 * (ans[i]/2));
	}
	return {odd, even};     // odd[i] : length of palindrome centred at ith character
}                           // even[i]: length of palindrome centred after ith character (0-indexed)
