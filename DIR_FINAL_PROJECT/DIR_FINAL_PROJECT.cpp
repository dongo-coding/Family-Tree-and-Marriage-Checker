#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>
using namespace std;


vector<int> getAncestors(int x, const vector<int>& parent) {
    vector<int> anc;
    int steps = 0;
    int cur = x;
    while (steps < 3) {
        if (cur <= 0 || parent[cur] <= 0) break;
        cur = parent[cur];
        anc.push_back(cur);
        steps++;
    }
    return anc;
}


bool canMarry(int a, int b, const vector<int>& parent) {
    vector<int> A = getAncestors(a, parent);
    vector<int> B = getAncestors(b, parent);
    unordered_set<int> s;
    for (int id : A) s.insert(id);
    for (int id : B) {
        if (s.find(id) != s.end()) {
            return false; 
        }
    }
    return true;
}

int main() {

    ifstream fin("input.txt");
    ofstream fout("output.txt");

    if (!fin.is_open()) {
        cerr << "Can't be opened input.txt\n";
        return 0;
    }
    if (!fout.is_open()) {
        cerr << "Can't be opened output.txt\n";
        return 0;
    }


    int N;
    if (!(fin >> N)) return 0;

    vector<int> parent(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        int p;
        fin >> p;
        parent[i] = p;
    }

    int Q;
    fin >> Q;
    while (Q--) {
        int a, b;
        fin >> a >> b;
        if (a < 1 || a > N || b < 1 || b > N) {
            fout << "INVALID_ID\n";
            continue;
        }
        if (canMarry(a, b, parent)) fout << "ALLOW\n";
        else fout << "FORBID\n";
    }

    fin.close();
    fout.close();
    return 0;
}
