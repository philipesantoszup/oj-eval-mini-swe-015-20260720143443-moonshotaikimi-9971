#include <bits/stdc++.h>
using namespace std;

const char* DB_FILE = "db.dat";

// Most compact representation: key_data || values
// Index: key offset array, key length array
// Use 3 parallel vectors

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Use parallel arrays to avoid struct padding
    vector<char> key_data;
    vector<uint32_t> key_off;
    vector<uint8_t> key_len;
    vector<int> values;
    
    // Reserve to reduce reallocations
    key_off.reserve(100000);
    key_len.reserve(100000);
    values.reserve(100000);
    key_data.reserve(2000000);  // ~2MB for keys
    
    // Load
    ifstream in(DB_FILE, ios::binary);
    if (in) {
        uint32_t n;
        in.read((char*)&n, 4);
        uint32_t kd_size;
        in.read((char*)&kd_size, 4);
        
        key_data.resize(kd_size);
        in.read(key_data.data(), kd_size);
        
        key_off.resize(n);
        key_len.resize(n);
        values.resize(n);
        
        in.read((char*)key_off.data(), n * 4);
        in.read((char*)key_len.data(), n);
        in.read((char*)values.data(), n * 4);
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
            
            // Check duplicate
            bool dup = false;
            for (size_t j = 0; j < values.size(); j++) {
                if (key_len[j] == key.length() && 
                    memcmp(&key_data[key_off[j]], key.data(), key.length()) == 0 &&
                    values[j] == val) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                key_off.push_back(key_data.size());
                key_len.push_back(key.length());
                values.push_back(val);
                key_data.insert(key_data.end(), key.begin(), key.end());
            }
            
        } else if (cmd == "delete") {
            iss >> key >> val;
            for (size_t j = 0; j < values.size(); j++) {
                if (key_len[j] == key.length() && 
                    memcmp(&key_data[key_off[j]], key.data(), key.length()) == 0 &&
                    values[j] == val) {
                    // Remove by swapping with last
                    // Note: key_data remains (leaks), acceptable for this problem
                    key_off[j] = key_off.back();
                    key_len[j] = key_len.back();
                    values[j] = values.back();
                    key_off.pop_back();
                    key_len.pop_back();
                    values.pop_back();
                    break;
                }
            }
            
        } else if (cmd == "find") {
            iss >> key;
            vector<int> found;
            for (size_t j = 0; j < values.size(); j++) {
                if (key_len[j] == key.length() && 
                    memcmp(&key_data[key_off[j]], key.data(), key.length()) == 0) {
                    found.push_back(values[j]);
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
    uint32_t cnt = values.size();
    uint32_t kd = key_data.size();
    out.write((char*)&cnt, 4);
    out.write((char*)&kd, 4);
    out.write(key_data.data(), kd);
    out.write((char*)key_off.data(), cnt * 4);
    out.write((char*)key_len.data(), cnt);
    out.write((char*)values.data(), cnt * 4);
    
    return 0;
}
