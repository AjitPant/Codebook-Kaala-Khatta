// Monge condition : ∀a<b≤c<d,
// Convex Monge condition : f(a,c)+f(b,d)≤f(a,d)+f(b,c)
// Concave Monge condition : f(a,c)+f(b,d)≥f(a,d)+f(b,c)
// Totally monotone : ∀ a<b≤c<d,
// Convex totally monotone : f(a,c)≥f(b,c)⟹f(a,d)≥f(b,d)
// Concave totally monotone : f(a,c)≤f(b,c)⟹f(a,d)≤f(b,d)
// Usually f(i,j) is something like dpi+cost(i+1,j) or cost(i,j).

struct Node {
  ll p, l, r;  // p is the best transition point for dp[l], dp[l+1], ..., dp[r]
};

deque<Node> dq;
dp[0] = 0;
dq.push_back({0, 1, n});
for (int i = 1; i <= n; ++i) {
  dp[i] = f(dq.front().p, i)
          // r == i implies that this Node is useless later, so pop it
          if (dq.front().r == i) dq.pop_front();
  // else update l
  else dq.front().l++;

  // find l, r for i
  // f(i, dq.back().l) < f(dq.back().p, dq.back().l) implies the last Node in
  // deque is useless
  while (!dq.empty() && f(i, dq.back().l) < f(dq.back().p, dq.back().l))
    dq.pop_back();
  // we know that r=n, now we need to find l
  // l=i+1 as deque is empty
  if (dq.empty()) dq.push_back({i, i + 1, n});
  // find l by binary search
  else {
    int l = dq.back().l, r = dq.back().r;
    while (l < r) {
      int mid = r - (r - l) / 2;
      if (f(i, mid) < f(dq.back().p, mid))
        r = mid - 1;
      else
        l = mid;
    }
    dq.back().r = l;
    // l == n means that i is useless
    if (l != n) dq.push_back({i, l + 1, n});
  }
}
