//<File I/O Example: Open the file with validation>
//First Method (use the constructor)
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    //declare and automatically ope n the file
    ofstream outFile("fout.txt");
    // Open validation
    if(!outFile) {
        cout << "Cannot open file.\n ";
        return 1;
    }
    return 0;
}
