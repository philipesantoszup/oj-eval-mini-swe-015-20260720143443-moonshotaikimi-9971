#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>

using namespace std;

// File-based key-value storage
// Each index has its own file storing sorted values

string getFilename(const string& index) {
    return "db_" + index + ".dat";
}

vector<int> readValues(const string& filename) {
    vector<int> values;
    ifstream in(filename, ios::binary);
    if (!in) return values;
    
    int val;
    while (in.read((char*)&val, sizeof(int))) {
        values.push_back(val);
    }
    return values;
}

void writeValues(const string& filename, const vector<int>& values) {
    ofstream out(filename, ios::binary | ios::trunc);
    for (int val : values) {
        out.write((char*)&val, sizeof(int));
    }
}

void insertEntry(const string& index, int value) {
    string filename = getFilename(index);
    vector<int> values = readValues(filename);
    
    // Check if value already exists
    auto it = lower_bound(values.begin(), values.end(), value);
    if (it != values.end() && *it == value) {
        return; // Already exists
    }
    
    // Insert in sorted order
    values.insert(it, value);
    writeValues(filename, values);
}

void deleteEntry(const string& index, int value) {
    string filename = getFilename(index);
    vector<int> values = readValues(filename);
    
    auto it = lower_bound(values.begin(), values.end(), value);
    if (it != values.end() && *it == value) {
        values.erase(it);
        if (values.empty()) {
            remove(filename.c_str());
        } else {
            writeValues(filename, values);
        }
    }
}

void findEntry(const string& index) {
    string filename = getFilename(index);
    vector<int> values = readValues(filename);
    
    if (values.empty()) {
        cout << "null" << endl;
        return;
    }
    
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) cout << " ";
        cout << values[i];
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore();
    
    string line;
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        istringstream iss(line);
        
        string cmd;
        iss >> cmd;
        
        if (cmd == "insert") {
            string index;
            int value;
            iss >> index >> value;
            insertEntry(index, value);
        } else if (cmd == "delete") {
            string index;
            int value;
            iss >> index >> value;
            deleteEntry(index, value);
        } else if (cmd == "find") {
            string index;
            iss >> index;
            findEntry(index);
        }
    }
    
    return 0;
}
