#include <iostream> //전처리 지시자
#include <climits>
//#define PIE = 3.1415926535 //c style
//#include <cstdio> //cstyle

/*
Cpp에서 함수를 사용하고자 한다면 반드시 그 함수의 원형을 미리 정의하여야 한다.
iostream에 cout이 정의되어 있음
*/

using namespace std;
// std를 안하면 cout과 endl앖에 std::를 붙여줘야 함.

int main() {
    cout << "World!"; //꺽쇠는 데이터의 흐름(방향)
    cout << "Hello" << endl; //endl은 줄바꿈의 역할
    //printf("Hello, World!\n"); //cstyle

    /*================================*/

    int A= 7;
    int B = 3;
    cout << "A = " << A << endl << "B = " << B << endl;

    /*================================*/

    int n_int = INT_MAX;
	short n_short = SHRT_MAX;
	long n_long = LONG_MAX;
    long long n_llong = LLONG_MAX;

	cout << "int는 " << sizeof n_int << "바이트이다." << endl;
	cout << "이 바이트의 최대값은 " << n_int << " 이다." << endl;
    
	cout << "short은 " << sizeof n_short << "바이트이다." << endl;
	cout << "이 바이트의 최대값은 " << n_short << " 이다." << endl;
    
	cout << "long은 " << sizeof n_long << "바이트이다." << endl;
	cout << "이 바이트의 최대값은 " << n_long << " 이다." << endl;
    
	cout << "long long은 " << sizeof n_llong << "바이트이다." << endl;
	cout << "이 바이트의 최대값은 " << n_llong << " 이다." << endl;

    unsigned short a = -2; //-1
    cout << a << endl;

    /*================================*/

    float b = 3.14;
    int c = 3.14;
    cout << b << " " << c << endl;

    /*================================*/
    
    // ASCII코드 문자에 숫자 대응
    int d = 77;
    char e = d; //M
    e = 'a';
    /*
    null 문자 '\0' 문자열에서 어디까지 문자인지 확인하기 위한 것
    큰따옴표에는 널문자가 포함됨을 의미 => string
    한글은 ascii 지원x  유니코드 <= wput, 로케이션 지역 설정, string
    */
    char f[] = {'a', 'b', 'c', '\0'};
    cout << f << endl;

    /*================================*/

    //bool : 0 혹은 1 -- cpp에서만
    bool ba = 0;
    bool bb = 1;
    bool bc = 10;
    cout << ba << " " << bb << " " << bc << endl;

    /*================================*/

    //원의 넓이를 구하는 프로그램

    //상수
     const float PIE = 3.1415926535;

     int r = 3;
     float s = r * r * PIE;

     //데이터형 변환
     /*
     1. 특정 데이터형의 변수에 다른 데이터형의 값을 대입했을 때
     2. 수식에 데이터형을 혼합하여 사용했을 때
     3. 함수에 매개변수를 전달할 때

     강제적 형변환
     typeName(a) (typeName)a
     */
     int g = 3.141592;
     cout << g << endl;

     char ch = 'M';
     cout << (int)ch << " " << int(ch) << endl;
     cout << static_cast<int>(ch) << endl;

    /*================================*/

    //산술연산자와 auto
    //나눗셈 - 둘중 하나라도 실수라면 실수형으로 반환
    auto n = 100; //int
    auto x = 1.5; //float
    auto y = 1.3e12L; //long long

    cout << n << " " << x << " " << y << endl;
    return 0;
}