//Example Reading from file
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
int main() {
    //Declare and open a text file
    ifstream INFile("number.txt");
    string line;
    int total = 0;
    while(! INFile.eof()) {
        getline(INFile, line);
        //converting line string to int
        stringstream(line) >> total;
        cout << line << endl;
        cout << total +1 << endl;
    }
    INFile.close(); // close the file
    return 0;
}
 