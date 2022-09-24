struct Node { 
  ll p, l, r; // p is the best transition point for dp[l], dp[l+1], ..., dp[r]
}; 

vector<Node> v;
dp[0] = 0;
v.push_back({0, 1, n});
for(int i = 1 ; i <= n ; ++i) {
  dp[i] = f(v.back().p, i)
  // r == i implies that this Node is useless later, so pop it
  if(v.back().r == i) v.pop_back(); 
  // else update l
  else v.back().l++;

  // find l, r for i
  // f(i, v.back().r) < f(v.back().p, v.back().r) implies the last Node in vector is useless
  while(!v.empty() && f(i, v.back().r) < f(v.back().p, v.back().r)) v.pop_back();
  // we know that l=i+1, now we need to find r
  // r=n as vector is empty
  if(v.empty()) v.push_back({i, i + 1, n});
  // find r by binary search
  else {
    int l = v.back().l, r = v.back().r;
    while(l < r) {
      int mid = l + (r - l) / 2;
      if(f(i, mid) < f(v.back().p, mid)) l = mid + 1;
      else r = mid;
    }
    v.back().l = l;
    // l == i + 1 means that i is useless
    if(l != i + 1) v.push_back({i, i + 1, l - 1});
  }
}
