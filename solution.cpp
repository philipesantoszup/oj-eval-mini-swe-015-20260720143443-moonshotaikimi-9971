#include <bits/stdc++.h>
using namespace std;

const int NUM_BUCKETS = 64;

int getBucket(const string& s) {
    unsigned h = 5381;
    for (char c : s) h = h * 33 + c;
    return h % NUM_BUCKETS;
}

string getFname(int b) {
    return "b" + to_string(b) + ".dat";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n; 
    cin.ignore();
    
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        istringstream iss(line);
        
        string cmd;
        iss >> cmd;
        
        if (cmd == "insert") {
            string key; 
            int val;
            iss >> key >> val;
            int b = getBucket(key);
            string fname = getFname(b);
            
            vector<pair<string, int>> data;
            ifstream in(fname, ios::binary);
            if (in) {
                uint16_t cnt;
                while (in.read((char*)&cnt, 2)) {
                    for (int j = 0; j < cnt; j++) {
                        uint8_t klen;
                        in.read((char*)&klen, 1);
                        string k; k.resize(klen);
                        in.read(&k[0], klen);
                        int v; in.read((char*)&v, 4);
                        data.push_back({k, v});
                    }
                }
            }
            
            bool dup = false;
            for (auto& p : data) {
                if (p.first == key && p.second == val) { dup = true; break; }
            }
            if (!dup) {
                data.push_back({key, val});
                sort(data.begin(), data.end());
                
                ofstream out(fname, ios::binary | ios::trunc);
                uint16_t cnt = data.size();
                out.write((char*)&cnt, 2);
                for (auto& p : data) {
                    uint8_t klen = p.first.length();
                    out.write((char*)&klen, 1);
                    out.write(p.first.data(), klen);
                    out.write((char*)&p.second, 4);
                }
            }
            
        } else if (cmd == "delete") {
            string key; 
            int val;
            iss >> key >> val;
            int b = getBucket(key);
            string fname = getFname(b);
            
            vector<pair<string, int>> data;
            ifstream in(fname, ios::binary);
            if (!in) continue;
            
            uint16_t cnt; in.read((char*)&cnt, 2);
            for (int j = 0; j < cnt; j++) {
                uint8_t klen;
                in.read((char*)&klen, 1);
                string k; k.resize(klen);
                in.read(&k[0], klen);
                int v; in.read((char*)&v, 4);
                if (k != key || v != val) data.push_back({k, v});
            }
            
            if (data.size() != cnt) {
                if (data.empty()) remove(fname.c_str());
                else {
                    ofstream out(fname, ios::binary | ios::trunc);
                    uint16_t cnt = data.size();
                    out.write((char*)&cnt, 2);
                    for (auto& p : data) {
                        uint8_t klen = p.first.length();
                        out.write((char*)&klen, 1);
                        out.write(p.first.data(), klen);
                        out.write((char*)&p.second, 4);
                    }
                }
            }
            
        } else if (cmd == "find") {
            string key;
            iss >> key;
            int b = getBucket(key);
            string fname = getFname(b);
            
            vector<int> vals;
            ifstream in(fname, ios::binary);
            if (in) {
                uint16_t cnt; in.read((char*)&cnt, 2);
                for (int j = 0; j < cnt; j++) {
                    uint8_t klen;
                    in.read((char*)&klen, 1);
                    string k; k.resize(klen);
                    in.read(&k[0], klen);
                    int v; in.read((char*)&v, 4);
                    if (k == key) vals.push_back(v);
                }
            }
            
            if (vals.empty()) {
                cout << "null\n";
            } else {
                sort(vals.begin(), vals.end());
                for (size_t j = 0; j < vals.size(); j++) {
                    if (j) cout << ' ';
                    cout << vals[j];
                }
                cout << '\n';
            }
        }
    }
    
    return 0;
}
