#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

vector<int> getAncestors(int x, const vector<int>& parent) {
    vector<int> anc;
    int steps = 0;
    int cur = x;
    while (steps < 3) {
        if (cur <= 0 || cur >= (int)parent.size() || parent[cur] <= 0) break;
        cur = parent[cur];
        anc.push_back(cur);
        steps++;
    }
    return anc;
}

bool canMarry(int a, int b, const vector<int>& parent) {
    vector<int> A = getAncestors(a, parent);
    vector<int> B = getAncestors(b, parent);
    unordered_set<int> s(A.begin(), A.end());
    for (int id : B) {
        if (s.count(id)) return false;
    }
    return true;
}

struct Node {
    int id;
    vector<Node*> children;
};

Node* buildTree(int root, const vector<vector<int>>& children) {
    Node* node = new Node{ root, {} };
    for (int c : children[root]) {
        node->children.push_back(buildTree(c, children));
    }
    return node;
}

int getWidth(Node* root) {
    if (!root) return 0;
    if (root->children.empty()) return to_string(root->id).size();
    int w = 0;
    for (Node* c : root->children) w += getWidth(c) + 2;
    return max(w, (int)to_string(root->id).size());
}

void printTree(Node* root, vector<string>& canvas, int left, int top) {
    if (!root) return;
    string label = to_string(root->id);
    int pos = left + getWidth(root) / 2 - label.size() / 2;
    if ((int)canvas.size() <= top) canvas.resize(top + 1);
    if ((int)canvas[top].size() < pos + label.size()) canvas[top].resize(pos + label.size(), ' ');
    for (int i = 0; i < (int)label.size(); i++) canvas[top][pos + i] = label[i];

    if (!root->children.empty()) {
        int childLeft = left;
        int mid = left + getWidth(root) / 2;
        if ((int)canvas.size() <= top + 1) canvas.resize(top + 2);
        for (Node* c : root->children) {
            int cw = getWidth(c);
            int cpos = childLeft + cw / 2;
            if ((int)canvas[top + 1].size() < max(mid, cpos) + 1) canvas[top + 1].resize(max(mid, cpos) + 1, ' ');
            if (cpos < mid) canvas[top + 1][cpos] = '/';
            else if (cpos > mid) canvas[top + 1][cpos] = '\\';
            printTree(c, canvas, childLeft, top + 2);
            childLeft += cw + 2;
        }
    }
}

void drawForest(ofstream& fout, const vector<int>& parent) {
    int N = parent.size() - 1;
    vector<vector<int>> children(N + 1);
    for (int i = 1; i <= N; i++) {
        if (parent[i] > 0) children[parent[i]].push_back(i);
    }
    vector<int> roots;
    for (int i = 1; i <= N; i++) if (parent[i] == 0) roots.push_back(i);

    fout << "FAMILY TREE:\n";
    for (int r : roots) {
        Node* root = buildTree(r, children);
        vector<string> canvas;
        printTree(root, canvas, 0, 0);
        for (string& line : canvas) fout << line << "\n";
        fout << "\n";
    }
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    if (!fin.is_open() || !fout.is_open()) {
        cerr << "Khong mo duoc file\n";
        return 0;
    }

    int N; fin >> N;
    vector<int> parent(N + 1, 0);
    for (int i = 1; i <= N; i++) fin >> parent[i];

    drawForest(fout, parent);

    int Q; if (!(fin >> Q)) Q = 0;
    while (Q--) {
        int a, b; fin >> a >> b;
        if (a<1 || a>N || b<1 || b>N) fout << "INVALID_ID\n";
        else fout << (canMarry(a, b, parent) ? "ALLOW (can marry)\n" : "FORBID (can't marry)\n");
    }
}
