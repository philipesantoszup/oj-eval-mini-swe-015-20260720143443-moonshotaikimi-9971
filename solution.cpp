#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>

using namespace std;

const char* DATA_FILE = "db.dat";

// Maps key -> set of sorted values
map<string, set<int>> db;

void loadData() {
    db.clear();
    ifstream in(DATA_FILE, ios::binary);
    if (!in) return;
    
    int numKeys;
    if (!in.read((char*)&numKeys, sizeof(int))) return;
    
    for (int i = 0; i < numKeys; i++) {
        int keyLen;
        in.read((char*)&keyLen, sizeof(int));
        string key;
        key.resize(keyLen);
        in.read(&key[0], keyLen);
        
        int numVals;
        in.read((char*)&numVals, sizeof(int));
        for (int j = 0; j < numVals; j++) {
            int val;
            in.read((char*)&val, sizeof(int));
            db[key].insert(val);
        }
    }
}

void saveData() {
    ofstream out(DATA_FILE, ios::binary | ios::trunc);
    
    // Remove empty entries
    for (auto it = db.begin(); it != db.end(); ) {
        if (it->second.empty()) {
            it = db.erase(it);
        } else {
            ++it;
        }
    }
    
    int numKeys = db.size();
    out.write((char*)&numKeys, sizeof(int));
    
    for (auto& p : db) {
        int keyLen = p.first.length();
        out.write((char*)&keyLen, sizeof(int));
        out.write(p.first.data(), keyLen);
        
        int numVals = p.second.size();
        out.write((char*)&numVals, sizeof(int));
        for (int val : p.second) {
            out.write((char*)&val, sizeof(int));
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Load existing data
    ifstream test(DATA_FILE);
    if (test.good()) {
        test.close();
        loadData();
    }
    
    int n;
    cin >> n;
    cin.ignore();
    
    string line, cmd, index;
    int value;
    
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        istringstream iss(line);
        iss >> cmd;
        
        if (cmd == "insert") {
            iss >> index >> value;
            db[index].insert(value);
            
        } else if (cmd == "delete") {
            iss >> index >> value;
            auto it = db.find(index);
            if (it != db.end()) {
                it->second.erase(value);
            }
            
        } else if (cmd == "find") {
            iss >> index;
            auto it = db.find(index);
            if (it == db.end() || it->second.empty()) {
                cout << "null\n";
                continue;
            }
            
            bool first = true;
            for (int val : it->second) {
                if (!first) cout << ' ';
                cout << val;
                first = false;
            }
            cout << '\n';
        }
    }
    
    saveData();
    
    return 0;
}
