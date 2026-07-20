#include <bits/stdc++.h>
using namespace std;

const char* DB_FILE = "db.dat";

// Use flat storage in memory with custom hash
// Memory: storing (key, value) pairs in a vector per key
// File stores: [num_keys][for each key: key_len, key, num_vals, vals...]

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Sparse hash table with robin_hood hashing would be ideal
    // but let's use standard with reserve
    unordered_map<string, vector<int>> db;
    db.reserve(50000);
    
    // Load existing data
    ifstream in(DB_FILE, ios::binary);
    if (in) {
        int nk;
        in.read((char*)&nk, 4);
        for (int i = 0; i < nk; i++) {
            int kl; in.read((char*)&kl, 4);
            string k; k.resize(kl);
            in.read(&k[0], kl);
            int nv; in.read((char*)&nv, 4);
            db[k].resize(nv);
            for (int j = 0; j < nv; j++) {
                in.read((char*)&db[k][j], 4);
            }
        }
    }
    
    int n;
    string line, cmd, key;
    int val;
    
    cin >> n;
    cin.ignore();
    
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        istringstream iss(line);
        iss >> cmd;
        
        if (cmd == "insert") {
            iss >> key >> val;
            auto& vec = db[key];
            auto it = lower_bound(vec.begin(), vec.end(), val);
            if (it == vec.end() || *it != val) {
                vec.insert(it, val);
            }
        } else if (cmd == "delete") {
            iss >> key >> val;
            auto it = db.find(key);
            if (it != db.end()) {
                auto& vec = it->second;
                auto vit = lower_bound(vec.begin(), vec.end(), val);
                if (vit != vec.end() && *vit == val) {
                    vec.erase(vit);
                }
            }
        } else if (cmd == "find") {
            iss >> key;
            auto it = db.find(key);
            if (it == db.end() || it->second.empty()) {
                cout << "null\n";
            } else {
                bool first = true;
                for (int v : it->second) {
                    if (!first) cout << ' ';
                    cout << v;
                    first = false;
                }
                cout << '\n';
            }
        }
    }
    
    // Save - remove empty keys first
    ofstream out(DB_FILE, ios::binary | ios::trunc);
    // Count non-empty
    int nk = 0;
    for (auto& p : db) if (!p.second.empty()) nk++;
    out.write((char*)&nk, 4);
    
    for (auto& p : db) {
        if (p.second.empty()) continue;
        int kl = p.first.length();
        out.write((char*)&kl, 4);
        out.write(p.first.data(), kl);
        int nv = p.second.size();
        out.write((char*)&nv, 4);
        for (int v : p.second) out.write((char*)&v, 4);
    }
    
    return 0;
}
