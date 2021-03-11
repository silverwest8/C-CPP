//File I/O Example: Writing

//Second Method ( use Open function)
#include <iostream>
#include <fstream>
using namespace std;
int main() {
    // declare output file variable
    ofstream outFile;
    // open an exist file fout.txt
    outFile.open("fout.txt");
    //behave just like cout, put the word into  the file
    outFile << "Hello World!";
    outFile.close();
    return 0;
}