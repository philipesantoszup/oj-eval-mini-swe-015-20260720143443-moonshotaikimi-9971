#include <bits/stdc++.h>
using namespace std;

const char* DB_FILE = "db.dat";

// Simple approach: keep sorted vector in memory, but use more efficient storage
// Store entries as: [key_len: 1][key chars...][value: 4] in flat buffer

struct Entry {
    uint32_t key_off;
    uint8_t key_len;
    int value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<char> key_data;  // All keys concatenated
    vector<Entry> entries;
    
    // Load
    ifstream in(DB_FILE, ios::binary);
    if (in) {
        uint32_t n;
        in.read((char*)&n, 4);
        uint32_t kd_size;
        in.read((char*)&kd_size, 4);
        
        key_data.resize(kd_size);
        in.read(key_data.data(), kd_size);
        
        entries.resize(n);
        for (uint32_t i = 0; i < n; i++) {
            in.read((char*)&entries[i].key_off, 4);
            in.read((char*)&entries[i].key_len, 1);
            in.read((char*)&entries[i].value, 4);
        }
    }
    
    auto cmp = [&](const Entry& a, const Entry& b) {
        // Compare keys
        int len = min((int)a.key_len, (int)b.key_len);
        int c = memcmp(&key_data[a.key_off], &key_data[b.key_off], len);
        if (c != 0) return c < 0;
        if (a.key_len != b.key_len) return a.key_len < b.key_len;
        return a.value < b.value;
    };
    
    auto find_key = [&](const string& k) {
        Entry dummy;
        dummy.key_off = 0;
        dummy.key_len = k.length();
        // This won't work directly... need a different approach
        return entries.end();
    };
    
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
            
            // Check duplicate
            bool dup = false;
            for (auto& e : entries) {
                if (e.key_len == key.length() && 
                    memcmp(&key_data[e.key_off], key.data(), key.length()) == 0 &&
                    e.value == val) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                Entry ne;
                ne.key_off = key_data.size();
                ne.key_len = key.length();
                ne.value = val;
                key_data.insert(key_data.end(), key.begin(), key.end());
                entries.push_back(ne);
            }
            
        } else if (cmd == "delete") {
            iss >> key >> val;
            for (auto it = entries.begin(); it != entries.end(); ++it) {
                if (it->key_len == key.length() && 
                    memcmp(&key_data[it->key_off], key.data(), key.length()) == 0 &&
                    it->value == val) {
                    entries.erase(it);
                    break;
                }
            }
            
        } else if (cmd == "find") {
            iss >> key;
            vector<int> found;
            for (auto& e : entries) {
                if (e.key_len == key.length() && 
                    memcmp(&key_data[e.key_off], key.data(), key.length()) == 0) {
                    found.push_back(e.value);
                }
            }
            sort(found.begin(), found.end());
            if (found.empty()) {
                cout << "null\n";
            } else {
                for (size_t j = 0; j < found.size(); j++) {
                    if (j) cout << ' ';
                    cout << found[j];
                }
                cout << '\n';
            }
        }
    }
    
    // Save
    ofstream out(DB_FILE, ios::binary | ios::trunc);
    uint32_t cnt = entries.size();
    uint32_t kd = key_data.size();
    out.write((char*)&cnt, 4);
    out.write((char*)&kd, 4);
    out.write(key_data.data(), kd);
    for (auto& e : entries) {
        out.write((char*)&e.key_off, 4);
        out.write((char*)&e.key_len, 1);
        out.write((char*)&e.value, 4);
    }
    
    return 0;
}
