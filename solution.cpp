#include <bits/stdc++.h>
using namespace std;

const int NUM_BUCKETS = 16;  // Must be <= 20

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
    
    // Pre-declare variables
    string line, cmd, key;
    int val;
    size_t j;
    
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        istringstream iss(line);
        iss >> cmd;
        
        if (cmd == "insert") {
            iss >> key >> val;
            int b = getBucket(key);
            string fname = getFname(b);
            
            // Read bucket
            vector<pair<string, int>> data;
            data.reserve(100);
            ifstream in(fname, ios::binary);
            if (in) {
                uint16_t cnt;
                in.read((char*)&cnt, 2);
                data.resize(cnt);
                for (j = 0; j < cnt; j++) {
                    uint8_t klen;
                    in.read((char*)&klen, 1);
                    data[j].first.resize(klen);
                    in.read(&data[j].first[0], klen);
                    in.read((char*)&data[j].second, 4);
                }
            }
            
            // Check duplicate
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
            iss >> key >> val;
            int b = getBucket(key);
            string fname = getFname(b);
            
            vector<pair<string, int>> data;
            data.reserve(100);
            ifstream in(fname, ios::binary);
            if (!in) continue;
            
            uint16_t cnt; 
            in.read((char*)&cnt, 2);
            for (int j = 0; j < cnt; j++) {
                uint8_t klen;
                in.read((char*)&klen, 1);
                string k; 
                k.resize(klen);
                in.read(&k[0], klen);
                int v; 
                in.read((char*)&v, 4);
                if (k != key || v != val) data.push_back({k, v});
            }
            
            if ((int)data.size() != cnt) {
                if (data.empty()) {
                    remove(fname.c_str());
                } else {
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
            iss >> key;
            int b = getBucket(key);
            string fname = getFname(b);
            
            vector<int> vals;
            vals.reserve(50);
            ifstream in(fname, ios::binary);
            if (in) {
                uint16_t cnt; 
                in.read((char*)&cnt, 2);
                for (int j = 0; j < cnt; j++) {
                    uint8_t klen;
                    in.read((char*)&klen, 1);
                    string k; 
                    k.resize(klen);
                    in.read(&k[0], klen);
                    int v; 
                    in.read((char*)&v, 4);
                    if (k == key) vals.push_back(v);
                }
            }
            
            if (vals.empty()) {
                cout << "null\n";
            } else {
                sort(vals.begin(), vals.end());
                for (j = 0; j < vals.size(); j++) {
                    if (j) cout << ' ';
                    cout << vals[j];
                }
                cout << '\n';
            }
        }
    }
    
    return 0;
}
