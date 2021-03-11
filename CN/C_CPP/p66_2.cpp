//<File I/O Example: Open the file with validation>
//Second Method ( use Open function)
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    // declare output file variable
    ofstream outFile;
    
    // open an exist file fout.txt
    outFile.open("fout.txt");
    // Open validation
    if(! outFile.is_open() ) {
        cout << "Cannot open file.\n ";
        return 1;
    }
    return 0;
}