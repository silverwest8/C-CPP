//File I/O Example: Writing

//First Method (use the constructor)
#include <iostream>
#include <fstream>
using namespace std;
int main() {
    /* declare and automatically open the file*/
   ofstream outFile("fout.txt");
   //behave just like cout, put the word into  the file
   outFile << "Hello World!";
   outFile.close();
    return 0;
}