#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

void xormethod(vector<char>& data, char key) {
    for (char& byte : data) { 
        byte ^= key;
    }
}

int main() {
    ifstream file1("./program1.exe", ios::binary | ios::ate);
    streamsize size1 = file1.tellg(); 
    file1.seekg(0, ios::beg);
    vector<char> buffer1(size1); 
    file1.read(buffer1.data(), size1);
    file1.close();

    xormethod(buffer1, 0x40);

    ifstream file2("./program2.exe", ios::binary | ios::ate);
    streamsize size2 = file2.tellg(); 
    file2.seekg(0, ios::beg);
    vector<char> buffer2(size2);  
    file2.read(buffer2.data(), size2);
    file2.close();

    ofstream output("retrofitting.exe", ios::binary);
    output.write(buffer2.data(), size2);
    output.write(buffer1.data(), size1);
    output.close();

    return 0;
}

