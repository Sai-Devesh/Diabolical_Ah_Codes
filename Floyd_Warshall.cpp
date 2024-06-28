/*
Author: Annepu Sai Devesh
Date: 1 April 2024
Given a text consisting the weighted adjacency 
matrix for a graph G, computing the shortest path between every pair of vertices in G.
Implement Floyd-Warshall algorithm to the above task. Also storing the distance and predecessor
matrix to compute the shortest path i− > v1 . . . − > j where each interme-
diate vertex has an index at most k.
*/

#include <bits/stdc++.h>
using namespace std;

// Constant for infinity
#define infinity 99999

// Function to read the adjacency matrix from a file
vector<vector<int>> getAdjacencyMatrix(string filename) {
    fstream myFile;
    myFile.open(filename, ios::in);
    if (myFile.is_open()) {
        string line;
        // Read the first line to get the size of the matrix
        getline(myFile, line);
        int n;
        istringstream iss(line);
        iss >> n;
        // Initialize the adjacency matrix
        vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
        int val;
        // Read the matrix values from the file
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
        // Error handling if the file cannot be opened
        cout << "Cannot open file while computing adjacency matrix from the file." << endl;
        vector<vector<int>> adjMatrix;
        return adjMatrix;
    }
}

// Function to execute the Floyd-Warshall algorithm
void floydWarshall(vector<vector<int>> adjMatrix, int n, vector<vector<vector<int>>> &dist, vector<vector<vector<int>>> &dist_1) {
    int k = 0;
    while (k < n) {
        if (k == 0) {
            // Initialize the dist and dist_1 matrices for k = 0
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    dist[k][i][j] = adjMatrix[i][j];
                    if (dist[k][i][j] == infinity) dist_1[k][i][j] = -1;
                    else dist_1[k][i][j] = i;
                }
            }
        } else {
            // Copy the previous distance and predecessor matrices
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    dist[k][i][j] = dist[k - 1][i][j];
                    dist_1[k][i][j] = dist_1[k - 1][i][j];
                }
            }
        }
        // Update the distance and predecessor matrices
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

// Function to get the shortest path using the predecessor matrix
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

// Function to print the shortest path
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

// Function to compute and print the shortest paths for triplets from the file
void computeTripletPath(string filename, vector<vector<vector<int>>> dist_1, vector<vector<vector<int>>> dist, int n) {
    int skipLines = n + 1;
    fstream myFile;
    myFile.open(filename, ios::in);

    if (myFile.is_open()) {
        string line;
        // Skip the lines containing the adjacency matrix
        while (skipLines > 0) {
            getline(myFile, line);
            skipLines--;
        }
        // Read the number of triplets
        getline(myFile, line);
        istringstream iss(line);
        int numTriplets;
        iss >> numTriplets;
        // Process each triplet
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
    // Read the adjacency matrix from the file
    vector<vector<int>> adjMatrix = getAdjacencyMatrix(filename);
    int n = adjMatrix.size();
    // Initialize the distance and predecessor matrices
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(n, vector<int>(n, 0)));
    vector<vector<vector<int>>> dist_1(n, vector<vector<int>>(n, vector<int>(n, -1)));
    // Execute the Floyd-Warshall algorithm
    floydWarshall(adjMatrix, n, dist, dist_1);

    // Print the weight adjacency matrix
    cout << "Weight Adjacency matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    // Print the distance matrix considering all vertices as intermediate vertices
    cout << "Distance matrix considering all vertices as intermediate vertices: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << dist[n - 1][i][j] << " ";
        }
        cout << endl;
    }

    // Compute and print the shortest paths for the triplets
    computeTripletPath(filename, dist_1, dist, n);

    return 0;
}
