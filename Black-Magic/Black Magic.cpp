#pragma GCC optimize("O3,unroll-loops,no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC target("popcnt")

#include <bits/extc++.h>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    set_t;
#include <ext/pb_ds/assoc_container.hpp>
typedef cc_hash_table<int, int> umap_t;
typedef priority_queue<int> heap;
#include <ext/rope>
using namespace __gnu_cxx;
int main() {
  set_t s;
  s.insert(12);
  s.insert(505);
  assert(*s.find_by_order(0) == 12);
  assert(*s.find_by_order(3) == 505);
  assert(s.order_of_key(12) == 0);
  assert(s.order_of_key(505) == 1);
  s.erase(12);
  assert(*s.find_by_order(0) == 505);
  assert(s.order_of_key(505) == 0);
  heap h1, h2;
  h1.join(h2);
  rope<char> r[2];
  r[1] = r[0];  // persistenet
  string t = "abc";
  r[1].insert(0, t.c_str());
  r[1].erase(1, 1);
  cout << r[1].substr(0, 2);
}
