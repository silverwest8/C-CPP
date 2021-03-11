//Example writing to file

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream outFile;
    // open an exist file fout.txt
    outFile.open("number.txt",ios::app);
    if (!outFile.is_open()) {
        cout << " problem with opening the file ";
        } else {
        outFile <<200 <<endl ;
        cout << "done writing" <<endl;
    }
    outFile.close();
    return 0;
}