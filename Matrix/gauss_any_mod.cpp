
int gauss (vector < vector<int> > &a, vector<int> & ans) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    vector<int> where (m, -1);
    for (int col=0, row=0; col<m && row<n; ++col) {
        int sel = row;
        for (int i=row; i<n; ++i)
            if (a[i][col] > a[sel][col])
                sel = i;
        if (a[sel][col]  == 0)
            continue;
        for (int i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;

        for (int i=0; i<n; ++i)
            if (i != row) {
                int c = a[i][col] *mod_inv( a[row][col], mod)%mod;
                for (int j=col; j<=m; ++j)
                {
                    a[i][j] =(a[i][j]- a[row][j] * c%mod + mod)%mod;

                }
            }
        ++row;
    }

    ans.assign (m, 0);
    vi out(1);
    for (int i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] * mod_inv( a[where[i]][i], mod)%mod;

    for (int i=0; i<n; ++i) {
        int sum = 0;
        for (int j=0; j<m; ++j)
            sum =(sum +  ans[j] * a[i][j])%mod;

        if (sum != a[i][m])
            return -1;
    }

    for (int i=0; i<m; ++i)
        if (where[i] == -1)
            return 2;
    return 1;
}
