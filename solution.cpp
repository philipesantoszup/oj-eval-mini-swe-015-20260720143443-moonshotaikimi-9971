#include <bits/stdc++.h>
using namespace std;

const char* DB_FILE = "db.dat";

struct Entry {
    string key;
    int value;
};

bool operator<(const Entry& a, const Entry& b) {
    if (a.key != b.key) return a.key < b.key;
    return a.value < b.value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<Entry> entries;
    entries.reserve(100000);
    
    // Load existing data
    ifstream in(DB_FILE, ios::binary);
    if (in) {
        int n; in.read((char*)&n, 4);
        entries.resize(n);
        for (int i = 0; i < n; i++) {
            int kl; in.read((char*)&kl, 4);
            entries[i].key.resize(kl);
            in.read(&entries[i].key[0], kl);
            in.read((char*)&entries[i].value, 4);
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
            auto it = lower_bound(entries.begin(), entries.end(), Entry{key, val});
            if (it == entries.end() || it->key != key || it->value != val) {
                entries.insert(it, Entry{key, val});
            }
            
        } else if (cmd == "delete") {
            iss >> key >> val;
            auto it = lower_bound(entries.begin(), entries.end(), Entry{key, val});
            if (it != entries.end() && it->key == key && it->value == val) {
                entries.erase(it);
            }
            
        } else if (cmd == "find") {
            iss >> key;
            // Find first occurrence with binary search
            auto it = lower_bound(entries.begin(), entries.end(), Entry{key, 0},
                [](const Entry& a, const Entry& b) { return a.key < b.key; });
            
            bool first = true;
            bool found = false;
            while (it != entries.end() && it->key == key) {
                if (!first) cout << ' ';
                cout << it->value;
                first = false;
                found = true;
                ++it;
            }
            if (!found) cout << "null";
            cout << '\n';
        }
    }
    
    // Save
    ofstream out(DB_FILE, ios::binary | ios::trunc);
    int cnt = entries.size();
    out.write((char*)&cnt, 4);
    for (auto& e : entries) {
        int kl = e.key.length();
        out.write((char*)&kl, 4);
        out.write(e.key.data(), kl);
        out.write((char*)&e.value, 4);
    }
    
    return 0;
}
