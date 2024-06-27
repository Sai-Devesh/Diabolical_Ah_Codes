#include <bits/stdc++.h>
using namespace std;

#define infinity 100000

vector<vector<int>> getAdjacencyMatrix(string filename) {
    fstream myFile;
    myFile.open(filename, ios::in);
    if (myFile.is_open()) {
        string line;
        getline(myFile, line);
        int n;
        istringstream iss(line);
        iss >> n;
        vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
        int val;
        for (int i = 0; i < n; i++) {
            getline(myFile, line);
            istringstream iss(line);
            int j = 0;
            while (iss >> val) {
                adjMatrix[i][j] = val;
                j++;
            }
        }
        return adjMatrix;
    } else {
        cout << "Cannot open file while computing adjacency matrix from the file." << endl;
        vector<vector<int>> adjMatrix;
        return adjMatrix;
    }
}

void floydWarshall(vector<vector<int>> adjMatrix, int n, vector<vector<vector<int>>> &dist, vector<vector<vector<int>>> &dist_1) {
    int k = 0;
    while (k < n) {
        if (k == 0) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    dist[k][i][j] = adjMatrix[i][j];
                    if (dist[k][i][j] == infinity) dist_1[k][i][j] = -1;
                    else dist_1[k][i][j] = i;
                }
            }
        } else {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    dist[k][i][j] = dist[k - 1][i][j];
                    dist_1[k][i][j] = dist_1[k - 1][i][j];
                }
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[k][i][k] + dist[k][k][j] < dist[k][i][j]) {
                    dist[k][i][j] = dist[k][i][k] + dist[k][k][j];
                    dist_1[k][i][j] = k;
                }
            }
        }
        k++;
    }
}

void gsp(vector<vector<int>> predMatrix, int i, int j, int k, vector<int> &sp) {
    if (i == j) {
        sp.push_back(i);
    } else if (predMatrix[i][j] == -1) {
        sp.clear();
    } else {
        gsp(predMatrix, i, predMatrix[i][j], k, sp);
        sp.push_back(j);
    }
}

void printsp(vector<vector<vector<int>>> dist_1, vector<vector<vector<int>>> dist, int i, int j, int k) {
    vector<int> sp;
    gsp(dist_1[k], i, j, k, sp);
    if (sp.size() <= 0) {
        cout << "No shortest path between " << i << " and " << j << " with k as  " << k << endl;
    } else {
        cout << "Shortest path between " << i << " and " << j << " with k as " << k << ": ";
        int sum = 0;
        for (int x = 0; x < sp.size() - 1; x++) {
            sum += dist[k][sp[x]][sp[x + 1]];
        }
        for (int x = 0; x < sp.size(); x++) cout << sp[x] << " ";
        cout << ". PathLength = " << sum;
        cout << endl;
    }
}

void computeTripletPath(string filename, vector<vector<vector<int>>> dist_1, vector<vector<vector<int>>> dist, int n) {
    int skipLines = n + 1;
    fstream myFile;
    myFile.open(filename, ios::in);

    if (myFile.is_open()) {
        string line;
        while (skipLines > 0) {
            getline(myFile, line);
            skipLines--;
        }
        getline(myFile, line);
        istringstream iss(line);
        int numTriplets;
        iss >> numTriplets;
        for (int x = 0; x < numTriplets; x++) {
            getline(myFile, line);
            istringstream iss(line);
            int i, j, k;
            iss >> i >> j >> k;
            printsp(dist_1, dist, i, j, k);
        }
    }
}

int main() {
    string filename = "FW_input.txt";
    vector<vector<int>> adjMatrix = getAdjacencyMatrix(filename);
    int n = adjMatrix.size();
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(n, vector<int>(n, 0)));
    vector<vector<vector<int>>> dist_1(n, vector<vector<int>>(n, vector<int>(n, -1)));
    floydWarshall(adjMatrix, n, dist, dist_1);

    cout << "Weight Adjacency matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Distance matrix considering all vertices as intermediate vertices: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << dist[n - 1][i][j] << " ";
        }
        cout << endl;
    }

    computeTripletPath(filename, dist_1, dist, n);

    return 0;
}
