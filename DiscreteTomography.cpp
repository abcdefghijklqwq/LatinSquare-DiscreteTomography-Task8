#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <map>
using namespace std;

#define all(x) (x).begin(), (x).end()

void dv(vector<int>& V) {
    for (int i = 0; i < (int)V.size(); i++)
        cout << V[i] << " \n"[i == (int)V.size() - 1];
    cout << "======\n";
}

void dv1(vector<vector<int>>& M, vector<int>& Y, vector<int>& X) {
    for (int i = 0; i < (int)Y.size(); i++)
        for (int j = 0; j < (int)X.size(); j++) {
            cout << M[i][j];
            if (j == (int)X.size()-1) cout << "|" << Y[i] << "\n";
            else cout << " ";
        }
    for (int i : X)
        cout << "- ";
    cout << "\n";
    for (int i : X)
        cout << i << " ";
    cout << "\n=======\n";
}

bool C1(vector<int>& Y, vector<int>& X) {
    return accumulate(all(Y), 0) == accumulate(all(X), 0);
}

bool C2(vector<int>& Y, vector<int>& X) {
    for (int y : Y)
        if (y > (int)X.size())
            return false;
    for (int x : X)
        if (x > (int)Y.size())
            return false;
    return true;
}

bool C3(vector<int> X, vector<int> X_) {
    //bagian ini masih un-sure
    while ((int)X.size() < (int)X_.size())
        X.push_back(0);
    while ((int)X_.size() < (int)X.size())
        X_.push_back(0);
    
    for (int i = 1; i < (int)X.size(); i++)
        X[i] += X[i-1];
    for (int i = 1; i < (int)X_.size(); i++)
        X_[i] += X_[i-1];

    bool ok = true;
    for (int i = 0; i < (int)X.size(); i++)
        ok &= X_[i] >= X[i];
    return ok;
}

void constructM_(vector<vector<int>>& M_, vector<int>& Y, vector<int>& X, vector<int>& X_) {
    int m = (int)Y.size(), n = (int)X.size();

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            M_[i][j] = (Y[i]-1 >= j ? 1 : 0);
    
    dv1(M_, Y, X_);

    queue<pair<int,int>> available_cells;

    for (int j = 0; j < n; j++)
        if (X[j] > X_[j])
            for (int cnt = X[j] - X_[j]; cnt > 0; cnt--)
                M_[available_cells.front().first][j] = 1, available_cells.pop();
        else if (X[j] < X_[j])
            for (int i = m-1, cnt = X_[j] - X[j]; i > -1 && cnt > 0; i--)
                if (M_[i][j] == 1)
                    available_cells.push({i, j}), M_[i][j] = 0, cnt--;
                
    dv1(M_, Y, X);
} 

vector<vector<int>> constructM(vector<vector<int>>& M_, vector<int>& Y, vector<int>& X, vector<int>& orderY, vector<int>& orderX) {
    int m = (int)Y.size(), n = (int)X.size();
    vector<vector<int>> M(m, vector<int>(n)), M1(m, vector<int>(n));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            M[i][j] = M_[orderY[i]][j];
    
    for (int j = 0; j < n; j++)
        for (int i = 0; i < m; i++)
            M1[i][j] = M[i][orderX[j]];
    
    vector<int> tX(n), tY(m);
    for (int i = 0; i < m; i++)
        tY[i] = Y[orderY[i]];
    for (int i = 0; i < m; i++)
        tX[i] = X[orderX[i]];
    
    dv1(M1, tY, tX);

    return M1;
}

void preserve_order(vector<int>& Y, vector<int>& X, vector<int>& orderY, vector<int>& orderX) {
    vector<pair<int,int>> Y_((int)Y.size()), X_((int)X.size());
    for (int i = 0; i < (int)Y.size(); i++)
        Y_[i] = {Y[i], i};
    for (int i = 0; i < (int)X.size(); i++)
        X_[i] = {X[i], i};
    
    dv(X), dv(Y);
    
    sort(all(Y)), sort(all(X)), sort(all(Y_)), sort(all(X_));
    reverse(all(Y)), reverse(all(X)), reverse(all(X_)), reverse(all(Y_));

    for (int i = 0; i < (int)Y_.size(); i++)
        orderY[i] = Y_[i].second;
    for (int i = 0; i < (int)X_.size(); i++)
        orderX[i] = X_[i].second;
}

int main() {
    int m, n; cin >> m >> n;
    vector<int> Y(m), X(n), X_(n), orderY(m), orderX(n);
    
    for (int i = 0; i < m; i++)
        cin >> Y[i];
    for (int i = 0; i < n; i++)
        cin >> X[i];

    preserve_order(Y, X, orderY, orderX);

    for (int j = 0; j < n; j++) {
        int cnt = 0;
        for (int i = 0; i < m; i++)
            if (Y[i] >= j+1)
                cnt++;
        X_[j] = cnt;
    }

    dv(Y), dv(X), dv(X_);
    if (C1(Y, X) && C2(Y, X) && C3(X, X_)) {
        cout << "Yes\n";
        vector<vector<int>> M_(m, vector<int>(n)), M;
        constructM_(M_, Y, X, X_);
        M = constructM(M_, Y, X, orderY, orderX);
    }
}