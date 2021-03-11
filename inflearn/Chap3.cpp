#include <iostream>
#include <cstring> //strlen()

using namespace std;

int main() {
    char a[] = "hello";

    /*================================*/

    //사용자 입력과 string
    const int Size = 15;
    char name1[Size];
    char name2[Size] = "C++programing";

    cout << "안녕하세요! 저는 " << name2;
    cout << "입니다! 성함이 어떻게 되시나요?\n";

    cin >> name1;
    //cin.get(name1, Size); getline 과 똑같이 동작

    cout << "음, " << name1 << "씨, 당신의 이름은 ";
    cout << strlen(name1) << "자입니다만\n";
    cout << sizeof(name1) << "바이트 크기의 배열에 저장되었습니다. \n";
    cout << "이름이 " << name1[0] << "자로 시작하는군요.\n";
    name2[3] = '\0';
    cout << "제 이름의 처음 세 문자는 다음과 같습니다: ";
    cout << name2 << endl;

    char char1[20];
    char char2[20] = "jaguar";
    string str1;
    string str2 = "panda";
    //char1 = char2; //틀리다
    str1 = str2; // 맞다
    cout << str1 << endl;

    /*================================*/

    struct MyStruct {
        string name;
        string position;
        int height;
        int weight;
    } B;
    
    MyStruct A;
    A.name = "Son";
    A.position = "Striker";
    A.height = 183;
    A.weight = 77;

    /*
    MyStruct A = {
        "Son",
        "Striker",
        183,
        77
    }
    */

    cout << A.name << endl;
    cout << A.position << endl;
    cout << A.height << endl;
    cout << A.weight << endl;

    //B = {}; //빈값은 0으로 초기화

    cout << B.height << endl;

    //구조체의 배열
    MyStruct C[2] = {
        {"A", "A", 1, 1},
        {"B", "B", 2, 2}
    };
    cout << C[0].height << endl;

    //공용체 union - 서로 다른 데이터형을 한 번에 한가지만 저장할 수 있음
    union Myunion {
        int intVal;
        long longVal;
        float floatVal;
    };
    Myunion test;
    test.intVal=3;
    cout << test.intVal << endl;
    test.intVal=3;
    test.longVal=33;
    cout << test.longVal << endl;
    test.intVal=3;
    test.floatVal=3.3;
    cout << test.longVal << endl;
    cout << test.floatVal << endl;


    //열거체 
    //기호 상수를 만드는 또다른 방법
    enum spectrum { red, orange, yellow, green, blue, purple};
    spectrum q = orange;
    cout << q << endl;


    return 0;
}