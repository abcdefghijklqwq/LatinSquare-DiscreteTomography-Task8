#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <map>
using namespace std;

#define all(x) (x).begin(), (x).end()

void printMatrix(vector<vector<int>>& M, vector<int>& Y, vector<int>& X) {
    for (int i = 0; i < (int)Y.size(); i++)
        for (int j = 0; j < (int)X.size(); j++) {
            cout << M[i][j];
            if (j == (int)X.size()-1) cout << "| " << Y[i] << "\n";
            else cout << " ";
        }
    for (int i = 0; i < (int)X.size(); i++)
        cout << "--" << (i == (int)X.size() - 1 ? "\n" : "");
    for (int i = 0; i < (int)X.size(); i++)
        cout << X[i] << " \n"[i == (int)X.size() - 1];
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
    for (int i = 1; i < (int)X.size(); i++)
        X[i] += X[i-1];
    for (int i = 1; i < (int)X_.size(); i++)
        X_[i] += X_[i-1];

    bool ok = true;
    for (int i = 0; i < (int)X.size(); i++)
        ok &= X_[i] >= X[i];
    return ok;
}

vector<vector<int>> constructM_(vector<int>& Y, vector<int>& X, vector<int>& X_) {
    int m = (int)Y.size(), n = (int)X.size();
    vector<vector<int>> M_(m, vector<int>(n));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            M_[i][j] = (Y[i]-1 >= j ? 1 : 0);

    for (int j = n-1; j > -1; j--)
        if (X[j] > X_[j])
            for (int k = j-1, cnt = X[j] - X_[j]; k > -1 && cnt > 0; k--)
                for (int i = m-1; i > -1 && cnt > 0; i--)
                    if (M_[i][k] == 1 && M_[i][j] == 0)
                        M_[i][j] = 1, M_[i][k] = 0, cnt--, X_[k]--;
    return M_;
} 

vector<vector<int>> constructM(vector<vector<int>> M_, vector<int>& Y, vector<int>& X, vector<int>& orderY, vector<int>& orderX) {
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
    for (int i = 0; i < n; i++)
        tX[i] = X[orderX[i]];
    return M1;
}

void preserve_order(vector<int>& Y, vector<int>& X, vector<int>& orderY, vector<int>& orderX) {
    vector<pair<int,int>> Y_((int)Y.size()), X_((int)X.size());
    for (int i = 0; i < (int)Y.size(); i++)
        Y_[i] = {Y[i], i};
    for (int i = 0; i < (int)X.size(); i++)
        X_[i] = {X[i], i};
    
    sort(all(Y)), sort(all(X)), sort(all(Y_)), sort(all(X_));
    reverse(all(Y)), reverse(all(X)), reverse(all(X_)), reverse(all(Y_));

    for (int i = 0; i < (int)Y_.size(); i++)
        orderY[Y_[i].second] = i;
    for (int i = 0; i < (int)X_.size(); i++)
        orderX[X_[i].second] = i;
}

bool isSolution(vector<vector<int>> M, vector<int> Y, vector<int> X) {
    int m = (int)Y.size(), n = (int)X.size();
    for (int i = 0; i < m; i++) {
        int cnt = 0;
        for (int j = 0; j < n; j++)
            cnt += (M[i][j]);
        if (cnt != Y[i])
            return false;
    }

    for (int j = 0; j < n; j++) {
        int cnt = 0;
        for (int i = 0; i < m; i++)
            cnt += (M[i][j]);
        if (cnt != X[j])
            return false;
    }
    return true;
}

int main() {
    int m, n; cin >> m >> n;
    vector<int> Y(m), X(n), X_(n), orderY(m), orderX(n), copyY, copyX;
    
    for (int i = 0; i < m; i++)
        cin >> Y[i];
    for (int i = 0; i < n; i++)
        cin >> X[i];
    
    copyY = Y, copyX = X;

    preserve_order(Y, X, orderY, orderX);

    for (int j = 0; j < n; j++) {
        int cnt = 0;
        for (int i = 0; i < m; i++)
            if (Y[i] >= j+1)
                cnt++;
        X_[j] = cnt;
    }

    if (C1(Y, X) && C2(Y, X) && C3(X, X_)) {
        vector<vector<int>> M;
        M = constructM(constructM_(Y, X, X_), Y, X, orderY, orderX);
        printMatrix(M, copyY, copyX);
    } else {
        cout << "There is no solution for the instance...\n";
    }
}