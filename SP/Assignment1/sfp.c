#include "sfp.h"
#include <stdio.h>
#include <stdlib.h>
sfp getNaN();
sfp getinfinitP();
sfp getinfinitM();
int getSign(sfp input);
int getExp(sfp input);
sfp getFrac(sfp input);
void getInfo(sfp input, int *sign, int *exp, int *E);
int specialCheck(sfp input);
void applySign(sfp* input, int sign);
void applyExp(sfp* input, int exp);
void applyFrac(sfp* input, sfp c);

sfp getNaN() {
    sfp NaN = 0;
    for (int i = 15; i > 0; i--) {
        NaN |= (1<<i);
    }
    return NaN;
}

sfp getinfinitP() {
    sfp infinityP = 0;
    applyExp(&infinityP, 31);
    return infinityP;
}

sfp getinfinitM() {
    sfp infinitM = 0;
    applySign(&infinitM, 1);
    applyExp(&infinitM, 31);
    return infinitM;
}

int getSign(sfp input) {
    return input >> 15 & 1;
}

int getExp(sfp input) {
    int exp, temp;
    exp = 0;
    for (int i = 14; i >= 10; i--) {
        if (input >> i & 1) {
            temp = 1;
            temp <<= i-10;
            exp += temp;
        }
    }
    return exp;
}

sfp getFrac(sfp input) {
    sfp frac;
    for (int i = 9; i >= 0; i--) {
        frac |= input >> i & 1;
    }
    return frac;
}

int isFrac1(sfp input) {
    for (int i = 0; i >= 0; i--) {
        if ((input >> i) & 1) {
            return 1;
        }
    }
    return 0;
}

void getInfo(sfp input, int *sign, int *exp, int *E) {
    int temp;
    *exp = 0;
    *sign = getSign(input);
    *exp = getExp(input);
    if (*exp == 0) {
        *E = -14;
    }
    *E = *exp - 15;
}

int specialCheck(sfp input) {
    if (getExp(input) == 31) {
        if (isFrac1(input) == 0) {
            if (getSign(input)) {
                return -1; //-infinity
            } else {
                return 1; //+infinity
            }
        } else {
            return 2; //NaN
        }
    }
    else if (getExp(input) == 0) {
        if (isFrac1(input) == 0) {
            return -2; //ZERO
        } else {
            // return -2; //demormal
        }
    }
    else if (0 < getExp(input) && getExp(input) < 31) {
        return 0; //normal
    }
    return 3; //??
}

void applySign(sfp* input, int sign) {
    *input |= (sign & 1) << 15;
}

void applyExp(sfp* input, int exp) {
    for (int i = 4; i >= 0; i--) {
        *input |= ((exp >> i) & 1) << (i+10);
    }
}

void applyFrac(sfp* input, sfp c) {
    *input |= c;
}
//1
sfp int2sfp(int input) {
    sfp result = 0;
    if (input == 0) {
        return result;
    }
    int maxsfp =  65504;
    int minsfp = -65504;
    if (input > maxsfp) {
        return getinfinitP();
    }
    else if (input < minsfp) {
        return getinfinitM();
    }
    if ((input >> 31) & 1) { //부호체크
        applySign(&result, 1);
        input = (~input) + 1;
    }
    //exp 체크 //2진수 변경 //만 백..?
    int E = 0;
    int exp = 0;
    int bias = 15;
    for(int i = 30; i >= 0; i--) {
        if((input >> i) & 1) {
            E = i;
            exp = E + 15;
            break;
        }
	}
    applyExp(&result, exp);
    if (E <= 10) {
        input ^= (1>>E); //leading 1
        input <<= (10-E); //frac첫부분으로 이동
    }
    else if (E > 10) {
        input >>= (E-10); //frac첫부분으로 이동
    }
    //frac 설정
    for (int i = 9; i >= 0; i--) {
        result |= (((input >> i) & 1) << i);
    }
	return result;
}
//2
int sfp2int(sfp input) {
    int result = 0;
    int Tmax = 1; //2147483647
    int Tmin = 1; //-2147483648
    Tmax <<= 31;
    Tmin <<= 31;
    Tmax--;
    if (input == getinfinitP()) {
        return Tmax;
    }
    else if (input == getinfinitM()) {
        return Tmin;
    }
    else if (specialCheck(input) == 2) {
        return Tmin;
    }

    //exp = E + bias
    int exp = getExp(input);
    //frac
    if (exp < 15) { //round toward zero
        return result;
    }
    else if (exp >= 15 && exp < 31) {
        int E = exp - 15;
        int push;
        for (int i = 9; i >= 0; i--) {
            if ((input >> i) & 1) {
                push = i;
            }
        }
        for (int i = 9; i >= 0; i--) {
            result |= ((input >> i) & 1) << i;
        }
        result <<= E;
        result |= (1 << (E+10)); //leading 1
        result >>= 10;
        //부호
        if ((input>>15) & 1) {
            result--;
            result = ~result;
            result |= ((input >> 15) & 1) << 31;
        }
    }
    return result;
}
//3
sfp float2sfp(float input) {
    sfp result = 0;
    unsigned save = *(unsigned *)&input;
    for(int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) &1);
    }
    int maxsfp =  65504;
    int minsfp = -65504;
    if (input > maxsfp) {
        return getinfinitP();
    }
    else if (input < minsfp) {
        return getinfinitM();
    }
    //exp
    int fexp = 0;
    int sfpexp = 0;
    int E = 0;
    for (int i = 30; i >= 23; i--) {
        if (save >> i & 1) {
            int temp = 0;
            temp |= 1;
            temp <<= i-23;
            fexp += temp;
        }
    }
    if (fexp == 0) {
        return result;
    }
    E = fexp - 127;
    sfpexp = E + 15;
    //exp
    applyExp(&result, sfpexp);
    //frac
    for(int i = 22; i >= 13; i--) {
        result |= ((save >> i) & 1) << (i-13);
    }
    //부호체크
    if ((save >> 31) & 1) {
        applySign(&result, 1);
    }
    return result;
}
//4
float sfp2float(sfp input) {
    float result = 0;
    unsigned save = *(unsigned *)&result;
    //sign
    save |= ((input >> 15) & 1) << 31;

    //exp
    int sfpexp = getExp(input);
    int E = sfpexp - 15;
    int fexp = E + 127;
    if (sfpexp == 0) {
        fexp = 0;
    }
    for(int i = 7; i >= 0; i--) {
        save |= ((fexp >> i) & 1) << (i+23);
	}
    //frac
    for(int i = 10; i >= 0; i--) {
        save |= ((input >> i) & 1) << (i+13);
	}
    result = *(float*)&save;
    return result;
}
//5
sfp sfp_add(sfp a, sfp b) {
    sfp result = 0;
    int as, aexp, aE;
    int bs, bexp, bE;
    getInfo(a, &as, &aexp, &aE);
    getInfo(b, &bs, &bexp, &bE);

    int aCheck = specialCheck(a);
    int bCheck = specialCheck(b);
    if (aCheck == 1 && bCheck == 1) {
        return a;
    }
    else if ((aCheck == 1 && bCheck == -1) || (aCheck == -1 && bCheck == 1)) {
        sfp c = 1;
        applyFrac(&a, c);
        return a;
    }
    else if ((aCheck == 1 && bCheck == 0) || (aCheck == 0 && bCheck == 1)) {
        if (aCheck == 1) {
            return a;
        } else {
            return b;
        }
    }
    else if (aCheck == -1 && bCheck == -1) {
        return a;
    }
    else if ((aCheck == -1 && bCheck == 0) || (aCheck == 0 && bCheck == -1)) {
        if (aCheck == -1) {
            return a;
        } else {
            return b;
        }
    }
    else if (aCheck == 2 || bCheck == 2) {
        if (aCheck == 2) {
            return a;
        } else {
            return b;
        }
    }
    else if ((aCheck == -2 && bCheck == 0) || (aCheck == 0 && bCheck == -2)) {
        if (aCheck == -2) {
            return b;
        }
        else {
            return a;
        }
    }
    else if (aCheck == -2 && bCheck == -2) {
        return a;
    }

    sfp calaM = 0;
    sfp calbM = 0;
    sfp sumMs = 0;

    //aFrac 계산
    int pushA = 0;
    for (int i = 9; i >= 0; i--) { if (a >> i & 1) { pushA = i; } }
    for (int i = 9; i >= 0; i--) { calaM |= ((a >> i) & 1) << (i-pushA); }
    
    //bFrac 계산
    int pushB = 0;
    for (int i = 9; i >= 0; i--) { if (b >> i & 1) { pushB = i; } }
    for (int i = 9; i >= 0; i--) { calbM |= ((b >> i) & 1) << (i-pushB); }
    
    if ((aexp != 0) && (bexp != 0)) { //둘 다 normal
        calaM |= 1 << (10-pushA);
        calbM |= 1 << (10-pushB);
    }
    else if (aexp == 0 && bexp == 0) { //둘다 demormal
    }
    else if (aexp == 0 || bexp == 0) { //둘중 하나만 denormal
        if (aexp == 0) {
            calbM |= 1 << (10-pushB);
        } else {
            calbM |= 1 << (10-pushB);
        }
    }

    if (aexp > bexp) { //a의 exp가 더 클 경우
        int gap = aexp - bexp;
        if (getSign(a) == getSign(b)) { //둘 다 양수거나 음수
            if (getSign(a)) {
                applySign(&result, 1);
            }
            calaM <<= gap-(pushB-pushA);
            sumMs = calaM + calbM;
            int push = 0;
            for (int i = 15; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = i;
                    break;
                }
            }
            sumMs ^= 1 << push;
            sumMs <<= (10-push);
            applyFrac(&result, sumMs);
            applyExp(&result, aexp);
        }
        else if (getSign(a) ^ getSign(b)) { //둘 중 하나만 양수
            if(getSign(a)) { //a만 음수


            } else { //b만 음수
                calaM <<= gap-(pushB-pushA);
                sumMs = calaM - calbM;
                for (int i = 9; i >= 0; i--) {
                    if ((sumMs >> i) & 1) {
                        sumMs ^= 1 << i;
                        sumMs <<= 10-i;
                        break;
                    }
                }
                applyFrac(&result, sumMs);
                applyExp(&result, aexp);
            }
        }
    }
    //------------------------------------//
    else if (aexp < bexp) { //b의 exp가 더 클 경우
        int gap = bexp - aexp;
        if (getSign(a) == getSign(b)) { //둘다 양수거나 음수
            if (getSign(a)) {
                applySign(&result, 1);
            }
            calaM <<= gap-(pushB-pushA);
            sumMs = calaM + calbM;
            int push = 0;
            for (int i = 15; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = i;
                    break;
                }
            }
            sumMs ^= 1 << push;
            sumMs <<= (10-push);
            applyFrac(&result, sumMs);
            applyExp(&result, aexp);
        }
        else if (getSign(a) ^ getSign(b)) { //둘 중 하나만 양수
            if (getSign(a)) { //a만 음수
            }
            else { //b만 음수
                applySign(&result, 1);
                calbM <<= gap;
                sumMs = calbM - calaM;
                int push = 0;
                for (int i = 9; i >= 0; i--) {
                    if ((sumMs >> i) & 1) {
                        push = 10-i;
                        break;
                    }
                }
                sumMs ^= 1 << push;
                for (int i = 9; i >= 0; i--) {
                    result |= ((sumMs >> i) & 1) << (i+push);
                }
                bexp--;
                for(int i = 4; i >= 0; i--) {
                    result |= ((bexp >> i) & 1) << (i+10);
                }
            }
        }
    }
    //------------------------------------//
    else { //같은 경우
        if (getSign(a) == getSign(b)) { //둘다 양수거나 음수
            if (getSign(a)) {
                applySign(&result, 1);
            }
        }
        else if (getSign(a) ^ getSign(b)) {
            if (getSign(a)) { //a만 음수
            }
            else { // b만 음수
            }
        }
        sumMs = calaM + calbM;
        int push = 0;
        int first1 = 0;
        for (int i = 9; i >= 0; i--) {
            if ((sumMs >> i) & 1) {
                push = 10-i;
                first1 = i;
                break;
            }
        }
        sumMs ^= 1 << first1;
        for (int i = 9; i >= 0; i--) {
            result |= ((sumMs >> i) & 1) << (i+push);
        }
        printf("result : %s\n", sfp2bits(result));
        aexp++;
        for(int i = 4; i >= 0; i--) {
            result |= ((aexp >> i) & 1) << (i+10);
        }
        printf("%s\n", sfp2bits(result));
    }

    return result;
}

sfp sfp_mul(sfp a, sfp b){
    sfp result = 0;
    int as, aexp, aE;
    int bs, bexp, bE;
    getInfo(a, &as, &aexp, &aE);
    getInfo(b, &bs, &bexp, &bE);

    int aCheck = specialCheck(a);
    int bCheck = specialCheck(b);
    ////예외인 경우
    if (aCheck == 1 && bCheck == 1) {
        return a;
    }
    else if ((aCheck == 1 && bCheck == -1) || (aCheck == -1 && bCheck == 1)) {
        if (aCheck == -1) {
            return a;
        } else {
            return b;
        }
    }
    else if (aCheck == -1 && bCheck == -1) {
        a ^= (a << 15);
        //체크하기
        return a;
    }
    else if ((aCheck == 1 && bCheck == 0) || (aCheck == 0 && bCheck == 1)) {
        if (aCheck == 1) {
            return a;
        } else {
            return b;
        }
    }
    else if ((aCheck == -1 && bCheck == 0) || (aCheck == 0 && bCheck == -1)) {
        if (aCheck == -1) {
            return a;
        } else {
            return b;
        }
    }
    else if ((aCheck == 1 && bCheck == -2) || (aCheck == -2 && bCheck == 1)) {
        return getNaN();
    }
    else if ((aCheck == -1 && bCheck == -2) || (aCheck == -2 && bCheck == -1)) {
        return getNaN();
    }
    else if (aCheck == 2 || bCheck == 2) {
        if (aCheck == 2) {
            return a;
        } else {
            return b;
        }
    }
    else if (aCheck == -2 || bCheck == -2) {
        if (aCheck == -2) {
            return a;
        } else {
            return b;
        }
    }

    //예외가 아닌 경우
    sfp calaM = 0;
    sfp calbM = 0;
    sfp mulMs = 0;

    int pushA = 0;
    int pushB = 0;
    //aFrac 계산
    pushA = 0;
    for (int i = 9; i >= 0; i--) { if (a >> i & 1) { pushA = i; } }
    for (int i = 9; i >= 0; i--) { calaM |= ((a >> i) & 1) << (i-pushA); }
    
    //bFrac 계산
    pushB = 0;
    for (int i = 9; i >= 0; i--) { if (b >> i & 1) { pushB = i; } }
    for (int i = 9; i >= 0; i--) { calbM |= ((b >> i) & 1) << (i-pushB); }
    if (aexp != 0) {
        if (pushA == 0) { pushA = 10; }
        calaM |= 1 << (10-pushA);

    }
    if (bexp != 0) {
        if (pushB == 0) { pushB = 10; }
        calbM |= 1 << (10-pushB);
    }
    //계산
    //부호 다르면 -
    if(as != bs) {
        applySign(&result, 1);
    }
    //곱하기
    mulMs = calaM * calbM;
    int push = 0;
    for (int i = 15; i >= 0; i--) {
        if ((mulMs >> i) & 1) {
            push = i;
            break;
        }
    }
    int exp = aE+bE+15;
    if (push - ((10-pushA) + (10-pushB))) {
        exp += (push - ((10-pushA) + (10-pushB)));
    }
    mulMs ^= 1 << push;
    mulMs <<= (10-push);
    applyExp(&result, exp);
    applyFrac(&result, mulMs);

    return result;
}

char* sfp2bits(sfp result) {
    char * bitStream = malloc(sizeof(char)*17);
    for(int i = 15; i >= 0; i--) {
        // printf("%d",(result >> i) & 1);
        if ((result >> i) & 1) {
            bitStream[15-i] = '1';
        } else {
            bitStream[15-i] = '0';
        }
	}
    bitStream[16] = '\0';
    return bitStream;
}