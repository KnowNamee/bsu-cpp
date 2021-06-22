#include <bits/stdc++.h>

using namespace std;

#define int long long
#define vec vector

signed main(){
    std::ios_base::sync_with_stdio(0);
    int n1, m1, n2, m2;
    cin >> n1 >> m1 >> n2 >> m2;
    vec<vec<int>> a(n1, vec<int>(m1));
    vec<vec<int>> b(n2, vec<int>(m2));
    for(int i=0; i<n1; i++){
        for(int j=0; j<m1; j++){
            cin>>a[i][j];
        }
    }
    for(int i=0; i<n2; i++){
        for(int j=0; j<m2; j++){
            cin>>b[i][j];
        }
    }
    vec<vec<int>> r(n1, vector<int>(m2));
    for(int i=0; i<n1; i++){
        for(int k=0; k<m1; k++){
            int aik = a[i][k];
            for(int j=0; j<m2; j++){
                r[i][j] += (aik*b[k][j]);
            }
        }
    }
    for(int i=0; i<n1; i++){
        for(int j=0; j<m2; j++){
            cout << r[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
