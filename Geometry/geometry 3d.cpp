namespace geometry_3d {
#undef x
#undef y
typedef double T;
struct p3 {
    T x, y, z;
    // Basic vector operations
    p3 operator+(p3 p) { return {x + p.x, y + p.y, z + p.z}; }
    p3 operator-(p3 p) { return {x - p.x, y - p.y, z - p.z}; }
    p3 operator*(T d) { return {x * d, y * d, z * d}; }
    p3 operator/(T d) { return {x / d, y / d, z / d}; } // only for floating-point
    // Some comparators
    bool operator==(p3 p) { return tie(x, y, z) == tie(p.x, p.y, p.z); }
    bool operator!=(p3 p) { return !operator==(p); }
};
p3 zero{0, 0, 0};
T operator|(p3 v, p3 w) { return v.x * w.x + v.y * w.y + v.z * w.z; }
T sq(p3 v) { return v | v; }
double abs(p3 v) { return sqrt(sq(v)); }
p3 unit(p3 v) { return v / abs(v); }
double angle(p3 v, p3 w) {
    double cosTheta = (v | w) / abs(v) / abs(w);
    return acos(max(-1.0, min(1.0, cosTheta)));
}
p3 operator*(p3 v, p3 w) {
    return {v.y * w.z - v.z * w.y,
            v.z * w.x - v.x * w.z,
            v.x * w.y - v.y * w.x};
}
T orient(p3 p, p3 q, p3 r, p3 s) { return (q - p) * (r - p) | (s - p); }
T orientByNormal(p3 p, p3 q, p3 r, p3 n) { return (q - p) * (r - p) | n; }
struct plane {
    p3 n;
    T d;
    // From normal n and offset d
    plane(p3 n, T d) : n(n), d(d) {}
    // From normal n and point P
    plane(p3 n, p3 p) : n(n), d(n | p) {}
    // From three non-collinear points P,Q,R
    plane(p3 p, p3 q, p3 r) : plane((q - p) * (r - p), p) {}
    // Will be defined later:
    // - these work with T = int
    T side(p3 p) { return (n | p) - d; }
    double dist(p3 p) { return std::abs(side(p)) / T(abs(n)); }
    plane translate(p3 t) { return {n, d + (n | t)}; }
    plane shiftUp(double dist) { return {n, d + dist * abs(n)}; }
    p3 proj(p3 p) { return p - n * side(p) / sq(n); }
    p3 refl(p3 p) { return p - n * 2 * side(p) / sq(n); }
};
struct coords {
    p3 o, dx, dy, dz;
    // From three points P,Q,R on the plane:
    // build an orthonormal 3D basis
    coords(p3 p, p3 q, p3 r) : o(p) {
        dx = unit(q - p);
        dz = unit(dx * (r - p));
        dy = dz * dx;
    }
    // From four points P,Q,R,S:
    // take directions PQ, PR, PS as is
    coords(p3 p, p3 q, p3 r, p3 s) : o(p), dx(q - p), dy(r - p), dz(s - p) {}
    geometry_2d::pt pos2d(p3 p) { return {(p - o) | dx, (p - o) | dy}; }
    p3 pos3d(p3 p) { return {(p - o) | dx, (p - o) | dy, (p - o) | dz}; }
};
struct line3d {
    p3 d, o;
    // From two points P, Q
    line3d(p3 p, p3 q) : d(q - p), o(p) {}
    // From two planes p1, p2 (requires T = double)
    // Will be defined later:
    // - these work with T = int
    double sqDist(p3 p) { return sq(d * (p - o)) / sq(d); }
    double dist(p3 p) { return sqrt(sqDist(p)); }
    bool cmpProj(p3 p, p3 q) { return (d | p) < (d | q); }
    p3 proj(p3 p) { return o + d * (d | (p - o)) / sq(d); }
    p3 refl(p3 p) { return proj(p) * 2 - p; }
    p3 inter(plane p) { return o - d * p.side(o) / (p.n | d); }
    line3d(plane p1, plane p2) {
        d = p1.n * p2.n;
        o = (p2.n * p1.d - p1.n * p2.d) * d / sq(d);
    }
};
double dist(line3d l1, line3d l2) {
    p3 n = l1.d * l2.d;
    if (n == zero) // parallel
        return l1.dist(l2.o);
    return std::abs((l2.o - l1.o) | n) / abs(n);
}
p3 closestOnL1(line3d l1, line3d l2) {
    p3 n2 = l2.d * (l1.d * l2.d);
    return l1.o + l1.d * ((l2.o - l1.o) | n2) / (l1.d | n2);
}
double smallAngle(p3 v, p3 w) {
    return acos(min(std::abs(v | w) / abs(v) / abs(w), 1.0));
}
double angle(plane p1, plane p2) {
    return smallAngle(p1.n, p2.n);
}
bool isParallel(plane p1, plane p2) {
    return p1.n * p2.n == zero;
}
bool isPerpendicular(plane p1, plane p2) {
    return (p1.n | p2.n) == 0;
}
double angle(line3d l1, line3d l2) {
    return smallAngle(l1.d, l2.d);
}
bool isParallel(line3d l1, line3d l2) {
    return l1.d * l2.d == zero;
}
bool isPerpendicular(line3d l1, line3d l2) {
    return (l1.d | l2.d) == 0;
}
double angle(plane p, line3d l) {
    return M_PI / 2 - smallAngle(p.n, l.d);
}
bool isParallel(plane p, line3d l) {
    return (p.n | l.d) == 0;
}
bool isPerpendicular(plane p, line3d l) {
    return p.n * l.d == zero;
}
line3d perpThrough(plane p, p3 o) { return line3d(o, o + p.n); }
plane perpThrough(line3d l, p3 o) { return plane(l.d, o); }
p3 vectorArea2(vector<p3> p) {
    // vector area * 2 (to avoiddivisions)
    p3 S = zero;
    for (int i = 0, n = p.size(); i < n; i++)
        S = S + p[i] * p[(i + 1) % n];
    return S;
}
double area(vector<p3> p) {
    return abs(vectorArea2(p)) / 2.0;
}
// Create arbitrary comparator for map<>
bool operator<(p3 p, p3 q) {
    return tie(p.x, p.y, p.z) < tie(q.x, q.y, q.z);
}
struct edge {
    int v;
    bool same; // = is the common edge in the same order?
};
// Given a series of faces (lists of points), reverse some of them
// so that their orientations are consistent
void reorient(vector<vector<p3>> &fs) {
    int n = fs.size();
    // Find the common edges and create the resulting graph
    vector<vector<edge>> g(n);
    map<pair<p3, p3>, int> es;
    for (int u = 0; u < n; u++) {
        for (int i = 0, m = fs[u].size(); i < m; i++) {
            p3 a = fs[u][i], b = fs[u][(i + 1) % m];
            // Let’s look at edge [AB]
            if (es.count({a, b})) {
                // seen in same order
                int v = es[ {a, b}];
                g[u].push_back({v, true});
                g[v].push_back({u, true});
            }
            else if (es.count({b, a})) {
                // seen in different order
                int v = es[ {b, a}];
                g[u].push_back({v, false});
                g[v].push_back({u, false});
            }
            else {
                // not seen yet
                es[ {a, b}] = u;
            }
        }
    }
    // Perform BFS to find which faces should be flipped
    vector<bool> vis(n, false), flip(n);
    flip[0] = false;
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (edge e : g[u]) {
            if (!vis[e.v]) {
                vis[e.v] = true;
                // If the edge was in the same order,
                // exactly one of the two should be flipped
                flip[e.v] = (flip[u] ^ e.same);
                q.push(e.v);
            }
        }
    }
    // Actually perform the flips
    for (int u = 0; u < n; u++)
        if (flip[u])
            reverse(fs[u].begin(), fs[u].end());
}
double volume(vector<vector<p3>> fs) {
    double vol6 = 0.0;
    for (vector<p3> f : fs)
        vol6 += (vectorArea2(f) | f[0]);
    return std::abs(vol6) / 6.0;
}
p3 sph(double r, double lat, double lon) {
    lat *= M_PI / 180, lon *= M_PI / 180;
    return {r * cos(lat) * cos(lon), r * cos(lat) * sin(lon), r * sin(lat)};
}
int sphereLine(p3 o, double r, line3d l, pair<p3, p3> &out) {
    double h2 = r * r - l.sqDist(o);
    if (h2 < 0)
        return 0;                     // the line doesn’t touch the sphere
    p3 p = l.proj(o);                 // point P
    p3 h = l.d * sqrt(h2) / abs(l.d); // vector parallel to l, of length h
    out = {p - h, p + h};
    return 1 + (h2 > 0);
}
double greatCircleDist(p3 o, double r, p3 a, p3 b) {
    return r * angle(a - o, b - o);
}
bool validSegment(p3 a, p3 b) {
    return a * b != zero || (a | b) > 0;
}
bool properInter(p3 a, p3 b, p3 c, p3 d, p3 &out) {
    p3 ab = a * b, cd = c * d; // normals of planes OAB and OCD
    int oa = geometry_2d::sgn(cd | a),
        ob = geometry_2d::sgn(cd | b),
        oc = geometry_2d::sgn(ab | c),
        od = geometry_2d::sgn(ab | d);
    out = ab * cd * od; // four multiplications => careful with overflow!
    return (oa != ob && oc != od && oa != oc);
}
bool onSphSegment(p3 a, p3 b, p3 p) {
    p3 n = a * b;
    if (n == zero)
        return a * p == zero && (a | p) > 0;
    return (n | p) == 0 && (n | a * p) >= 0 && (n | b * p) <= 0;
}
struct directionSet : vector<p3> {
    using vector::vector; // import constructors
    void insert(p3 p) {
        for (p3 q : *this)
            if (p * q == zero)
                return;
        push_back(p);
    }
};
directionSet intersSph(p3 a, p3 b, p3 c, p3 d) {
    assert(validSegment(a, b) && validSegment(c, d));
    p3 out;
    if (properInter(a, b, c, d, out))
        return {out};
    directionSet s;
    if (onSphSegment(c, d, a))
        s.insert(a);
    if (onSphSegment(c, d, b))
        s.insert(b);
    if (onSphSegment(a, b, c))
        s.insert(c);
    if (onSphSegment(a, b, d))
        s.insert(d);
    return s;
}
double angleSph(p3 a, p3 b, p3 c) {
    return angle(a * b, a * c);
}
double orientedAngleSph(p3 a, p3 b, p3 c) {
    if ((a * b | c) >= 0)
        return angleSph(a, b, c);
    else
        return 2 * M_PI - angleSph(a, b, c);
}
double areaOnSphere(double r, vector<p3> p) {
    int n = p.size();
    double sum = -(n - 2) * M_PI;
    for (int i = 0; i < n; i++)
        sum += orientedAngleSph(p[(i + 1) % n], p[(i + 2) % n], p[i]);
    return r * r * sum;
}
int windingNumber3D(vector<vector<p3>> fs) {
    double sum = 0;
    for (vector<p3> f : fs)
        sum += remainder(areaOnSphere(1, f), 4 * M_PI);
    return round(sum / (4 * M_PI));
}
}
