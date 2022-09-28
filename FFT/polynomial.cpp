namespace algebra {
const int inf = 1e9;
const int magic = 500; // threshold for sizes to run the naive algo
namespace fft {
const int maxn = 1 << 18;
typedef double ftype;
typedef complex<ftype> point;
const ftype pi = acos(-1);
template<typename T>
void mul(vector<T> &a, const vector<T> &b) {
    static const int shift = 15, mask = (1 << shift) - 1;
    size_t n = a.size() + b.size() - 1;
    while (__builtin_popcount(n) != 1) {
        n++;
    }
    a.resize(n);
    static point* A = new point[maxn], *B = new point[maxn];
    static point* C = new point[maxn], *D = new point[maxn];
    for (size_t i = 0; i < n; i++) {
        A[i] = point(a[i] & mask, a[i] >> shift);
        if (i < b.size()) {
            B[i] = point(b[i] & mask, b[i] >> shift);
        } else {
            B[i] = 0;
        }
    }
    fft(A, C, n); fft(B, D, n);
    for (size_t i = 0; i < n; i++) {
        point c0 = C[i] + conj(C[(n - i) % n]);
        point c1 = C[i] - conj(C[(n - i) % n]);
        point d0 = D[i] + conj(D[(n - i) % n]);
        point d1 = D[i] - conj(D[(n - i) % n]);
        A[i] = c0 * d0 - point(0, 1) * c1 * d1;
        B[i] = c0 * d1 + d0 * c1;
    }
    fft(A, C, n); fft(B, D, n);
    reverse(C + 1, C + n);
    reverse(D + 1, D + n);
    int t = 4 *  n;
    for (size_t i = 0; i < n; i++) {
        int64_t A0 = llround(real(C[i]) / t);
        T A1 = llround(imag(D[i]) / t);
        T A2 = llround(imag(C[i]) / t);
        a[i] = A0 + (A1 << shift) + (A2 << 2 * shift);
    }
    return;
}
}
template<typename T>
struct poly {
    vector<T> a;

    void normalize() { // get rid of leading zeroes
        while (!a.empty() && a.back() == T(0)) {
            a.pop_back();
        }
    }

    poly() {}
    poly(T a0) : a{a0} {normalize();}
    poly(vector<T> t) : a(t) {normalize();}

    poly inv(size_t n) const { // get inverse series mod x^n
        assert(!is_zero());
        poly ans = a[0].inv();
        size_t a = 1;
        while (a < n) {
            poly C = (ans * mod_xk(2 * a)).substr(a, 2 * a);
            ans -= (ans * C).mod_xk(a).mul_xk(a);
            a *= 2;
        }
        return ans.mod_xk(n);
    }
    pair<poly, poly> divmod_slow(const poly &b) const { // when divisor or quotient is small
        vector<T> A(a);
        vector<T> res;
        while (A.size() >= b.a.size()) {
            res.push_back(A.back() / b.a.back());
            if (res.back() != T(0)) {
                for (size_t i = 0; i < b.a.size(); i++) {
                    A[A.size() - i - 1] -= res.back() * b.a[b.a.size() - i - 1];
                }
            }
            A.pop_back();
        }
        std::reverse(begin(res), end(res));
        return {res, A};
    }

    pair<poly, poly> divmod(const poly &b) const { // returns quotiend and remainder of a mod b
        if (deg() < b.deg()) {
            return {poly{0}, *this};
        }
        int d = deg() - b.deg();
        if (min(d, b.deg()) < magic) {
            return divmod_slow(b);
        }
        poly D = (reverse(d + 1) * b.reverse(d + 1).inv(d + 1)).mod_xk(d + 1).reverse(d + 1, 1);
        return {D, *this - D * b};
    }

    T eval(T x) const { // evaluates in single point x
        T res(0);
        for (int i = int(a.size()) - 1; i >= 0; i--) {
            res *= x;
            res += a[i];
        }
        return res;
    }
    T& coef(size_t idx) { // mutable reference at coefficient
        return a[idx];
    }
    poly deriv() { // calculate derivative
        vector<T> res;
        for (int i = 1; i <= deg(); i++) {
            res.push_back(T(i) * a[i]);
        }
        return res;
    }
    poly integr() { // calculate integral with C = 0
        vector<T> res = {0};
        for (int i = 0; i <= deg(); i++) {
            res.push_back(a[i] / T(i + 1));
        }
        return res;
    }
    poly log(size_t n) { // calculate log p(x) mod x^n
        assert(a[0] == T(1));
        return (deriv().mod_xk(n) * inv(n)).integr().mod_xk(n);
    }
    poly exp(size_t n) { // calculate exp p(x) mod x^n
        if (is_zero()) {
            return T(1);
        }
        assert(a[0] == T(0));
        poly ans = T(1);
        size_t a = 1;
        while (a < n) {
            poly C = ans.log(2 * a).div_xk(a) - substr(a, 2 * a);
            ans -= (ans * C).mod_xk(a).mul_xk(a);
            a *= 2;
        }
        return ans.mod_xk(n);

    }

    poly pow_slow(size_t k, size_t n) { // if k is small
        return k ? k % 2 ? (*this * pow_slow(k - 1, n)).mod_xk(n) : (*this * *this).mod_xk(n).pow_slow(k / 2, n) : T(1);
    }
    poly pow(size_t k, size_t n) { // calculate p^k(n) mod x^n
        if (is_zero()) {
            return *this;
        }
        if (k < magic) {
            return pow_slow(k, n);
        }
        int i = leading_xk();
        T j = a[i];
        poly t = div_xk(i) / j;
        return bpow(j, k) * (t.log(n) * T(k)).exp(n).mul_xk(i * k).mod_xk(n);
    }
    vector<T> chirpz_even(T z, int n) { // P(1), P(z^2), P(z^4), ..., P(z^2(n-1))
        int m = deg();
        if (is_zero()) {
            return vector<T>(n, 0);
        }
        vector<T> vv(m + n);
        T zi = z.inv();
        T zz = zi * zi;
        T cur = zi;
        T total = 1;
        for (int i = 0; i <= max(n - 1, m); i++) {
            if (i <= m) {vv[m - i] = total;}
            if (i < n) {vv[m + i] = total;}
            total *= cur;
            cur *= zz;
        }
        poly w = (mulx_sq(z) * vv).substr(m, m + n).mulx_sq(z);
        vector<T> res(n);
        for (int i = 0; i < n; i++) {
            res[i] = w[i];
        }
        return res;
    }
    vector<T> chirpz(T z, int n) { // P(1), P(z), P(z^2), ..., P(z^(n-1))
        auto even = chirpz_even(z, (n + 1) / 2);
        auto odd = mulx(z).chirpz_even(z, n / 2);
        vector<T> ans(n);
        for (int i = 0; i < n / 2; i++) {
            ans[2 * i] = even[i];
            ans[2 * i + 1] = odd[i];
        }
        if (n % 2 == 1) {
            ans[n - 1] = even.back();
        }
        return ans;
    }
    template<typename iter>
    vector<T> eval(vector<poly> &tree, int v, iter l, iter r) { // auxiliary evaluation function
        if (r - l == 1) {
            return {eval(*l)};
        } else {
            auto m = l + (r - l) / 2;
            auto A = (*this % tree[2 * v]).eval(tree, 2 * v, l, m);
            auto B = (*this % tree[2 * v + 1]).eval(tree, 2 * v + 1, m, r);
            A.insert(end(A), begin(B), end(B));
            return A;
        }
    }
    vector<T> eval(vector<T> x) { // evaluate polynomial in (x1, ..., xn)
        int n = x.size();
        if (is_zero()) {
            return vector<T>(n, T(0));
        }
        vector<poly> tree(4 * n);
        build(tree, 1, begin(x), end(x));
        return eval(tree, 1, begin(x), end(x));
    }
    template<typename iter>
    poly inter(vector<poly> &tree, int v, iter l, iter r, iter ly, iter ry) { // auxiliary interpolation function
        if (r - l == 1) {
            return {*ly / a[0]};
        } else {
            auto m = l + (r - l) / 2;
            auto my = ly + (ry - ly) / 2;
            auto A = (*this % tree[2 * v]).inter(tree, 2 * v, l, m, ly, my);
            auto B = (*this % tree[2 * v + 1]).inter(tree, 2 * v + 1, m, r, my, ry);
            return A * tree[2 * v + 1] + B * tree[2 * v];
        }
    }
};

template<typename T>
T resultant(poly<T> a, poly<T> b) { // computes resultant of a and b
    if (b.is_zero()) {
        return 0;
    } else if (b.deg() == 0) {
        return bpow(b.lead(), a.deg());
    } else {
        int pw = a.deg();
        a %= b;
        pw -= a.deg();
        T mul = bpow(b.lead(), pw) * T((b.deg() & a.deg() & 1) ? -1 : 1);
        T ans = resultant(b, a);
        return ans * mul;
    }
}
template<typename iter>
poly<typename iter::value_type> kmul(iter L, iter R) { // computes (x-a1)(x-a2)...(x-an) without building tree
    if (R - L == 1) {
        return vector<typename iter::value_type> { -*L, 1};
    } else {
        iter M = L + (R - L) / 2;
        return kmul(L, M) * kmul(M, R);
    }
}
template<typename T, typename iter>
poly<T> build(vector<poly<T>> &res, int v, iter L, iter R) { // builds evaluation tree for (x-a1)(x-a2)...(x-an)
    if (R - L == 1) {
        return res[v] = vector<T> { -*L, 1};
    } else {
        iter M = L + (R - L) / 2;
        return res[v] = build(res, 2 * v, L, M) * build(res, 2 * v + 1, M, R);
    }
}
template<typename T>
poly<T> inter(vector<T> x, vector<T> y) { // interpolates minimum polynomial from (xi, yi) pairs
    int n = x.size();
    vector<poly<T>> tree(4 * n);
    return build(tree, 1, begin(x), end(x)).deriv().inter(tree, 1, begin(x), end(x), begin(y), end(y));
}
};

using namespace algebra;

typedef poly<base> polyn;

using namespace algebra;
