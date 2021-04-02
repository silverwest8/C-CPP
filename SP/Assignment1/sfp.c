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
            temp = 0;
            temp |= 1;
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

void getInfo(sfp input, int *sign, int *exp, int *E) {
    int temp;
    *exp = 0;
    *sign = getSign(input);
    *exp = getExp(input);
    *E = *exp - 15;
}

int specialCheck(sfp input) {
    if (getExp(input) == 31) {
        if (getFrac(input) == 0) {
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
        if (getFrac(input) == 0) {
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
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
    sfp result = 0;
    if (input == 0) {
        return result;
    }
    //부호체크
    if ((input >> 31) & 1) {
        applySign(&result, 1);
    }
    //exp 체크 //2진수 변경 //만 백..?
    int E = 0;
    int exp = 0;
    int bias = 15; //e가 14부터는 demormal
    for(int i = 30; i >= 0; i--) {
        if((input >> i) & 1) {
            E = i;
            exp = E + 15;
            break;
        }
	}
    int maxsfp =  65504;
    int minsfp = -65504;
    if (input > maxsfp) {
        printf("maximum of sfp\n");
        return getinfinitP();
    }
    else if (input < minsfp) {
        printf("minimum of sfp\n");
        return getinfinitM();
    }
    else if (E <= 10) {
        printf("no rounding\n");
        input ^= (1>>E);
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
        input <<= (10-E);
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
        applyExp(&result, exp);
        //frac 설정
        for (int i = 9; i >= 0; i--) {
            printf("%d", ((input >> i) & 1) );
            result |= (((input >> i) & 1) << i);
        }
        printf("\n");
    }
    else if (E > 10) {
        printf("\nexp : %d\n", exp);
        printf("\nE : %d\n", E);
        printf("\nRounding\n");
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
        input >>= (E-10);
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
        applyExp(&result, exp);
        //frac 설정
        for (int i = 9; i >= 0; i--) {
            printf("%d", ((input >> i) & 1) );
            result |= (((input >> i) & 1) << i);
        }
        printf("\n");
    }
	return result;
}
//2
int sfp2int(sfp input) {
    printf("\ninput : \n%s\n", sfp2bits(input));
    int result = 0;
    int Tmin = 1;
    Tmin <<= 31;
    int Tmax = 1;
    Tmax <<= 31;
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
    //round toward zero
    //frac
    if (exp == 0) {
        return 0;
    }
    else if (exp > 0 && exp < 31) {
        int E = exp - 15;
        if (E < 0) {
            return 0;
        }
        int push;
        for (int i = 9; i >= 0; i--) {
            printf("%d", (input >> i) & 1);
            if ((input >> i) & 1) {
                push = i;
            }
        }
        printf("\npush : %d\n", push);
        printf("\nexp : %d\n", exp);
        printf("\nE : %d\n", E);
        for (int i = 31; i >= 0; i--) {
            printf("%d", (result >> i) & 1);
        }
        printf("\n");
        for (int i = 9; i >= 0; i--) {
            result |= ((input >> i) & 1) << i;
        }
        result <<= E;
        for (int i = 31; i >= 0; i--) {
            printf("%d", (result >> i) & 1);
        }
        printf("\n");
        result |= (1 << (E+10));
        for (int i = 31; i >= 0; i--) {
            printf("%d", (result >> i) & 1);
        }
        printf("\n");
        result >>= 10;
        //부호
        result |= ((input >> 15) & 1) << 31;
        for (int i = 31; i >= 0; i--) {
            printf("%d", (result >> i) & 1);
        }
        printf("\n");
    }
    return result;
}
//3
sfp float2sfp(float input) {
    sfp result = 0;
    unsigned save = *(unsigned *)&input;
    printf("input:\n");
    for(int i = 31; i >= 0; i--) {
        printf("%d",(save >> i) & 1);
	}
    printf("\n");
    if (input == 0) {
        return result;
    }
    int maxsfp =  65504;
    int minsfp = -65504;
    if (input > maxsfp) {
        printf("maximum of sfp\n");
        return getinfinitP();
    }
    else if (input < minsfp) {
        printf("minimum of sfp\n");
        return getinfinitM();
    }
    //exp
    int fexp = 0;
    int fE = 0;
    int temp = 0;
    for (int i = 30; i >= 23; i--) {
        if (save >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-23;
            fexp += temp;
        }
    }
    fE = fexp - 127;

    if (fexp == 0) { //float demormal
        fE = 1-127;
    }
    else {
        fE = fexp - 127;
    }
    int sfpexp = fE + 15;
    int sfpE = 0;
    if (sfpexp < 0) {
        //round toward zero
        printf("\nsfpexp < 0\nsfpexp : %d\n", sfpexp);
    }
    else if (sfpexp > 31) {
        //round toward zero
        //+-무한대
        printf("\nsfpexp > 31nsfpexp : %d\n", sfpexp);
    }
    else if (sfpexp == 0) { //demormal
        sfpE = 1 - 15;
    }
    else if (sfpexp == 31) { //special

    }
    else {
        //exp
        applyExp(&result, sfpexp);
        //frac
        for(int i = 22; i >= 13; i--) {
            result |= ((save >> i) & 1) << (i-13);
        }
    }
    //부호체크
    if ((save >> 31) & 1) {
        applySign(&result, 1);
    }
    return result;
}
//4
float sfp2float(sfp input) {
    printf("\ninput : %s\n", sfp2bits(input));

    float result = 0;
    unsigned save = *(unsigned *)&result;
    printf("save : ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n");
    //sign
    save |= ((input >> 15) & 1) << 31;

    //exp
    int sfpexp = getExp(input);
    printf("sfpexp : %d\n", sfpexp);
    int E = sfpexp - 15;
    printf("E : %d\n", E);
    int fexp = E + 127;
    if (sfpexp == 0) {
        fexp = 0;
    }
    printf("fexp : %d\n", fexp);
    for(int i = 7; i >= 0; i--) {
        save |= ((fexp >> i) & 1) << (i+23);
	}
    printf("save : ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n");
    //frac
    for(int i = 10; i >= 0; i--) {
        save |= ((input >> i) & 1) << (i+13);
	}
    printf("save : ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n");
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

    sfp calaM = 0;
    sfp calbM = 0;
    sfp sumMs = 0;

    if ((aexp > 0 && aexp < 31) && (bexp > 0 && bexp < 31)) { //둘 다 normal
        //------------------------------------//
        printf("둘 다 normal");
        //aFrac 계산
        printf("\naM : ");
        int pushA = 0;
        for (int i = 9; i >= 0; i--) {
            printf("%d",(a >> i) & 1);
            if (a >> i & 1) {
                pushA = i;
            }
        }
        for (int i = 9; i >= 0; i--) {
            calaM |= ((a >> i) & 1) << (i-pushA);
        }
        calaM |= 1 << (10-pushA);
        //bFrac 계산
        int pushB = 0;
        printf("\nbM : ");
        for (int i = 9; i >= 0; i--) {
            printf("%d",(b >> i) & 1);
            if (b >> i & 1) {
                pushB = i;
            }
        }
        for (int i = 9; i >= 0; i--) {
            calbM |= ((b >> i) & 1) << (i-pushB);
        }
        calbM |= 1 << (10-pushB);
        printf("\npushA : %d, pushB : %d\n", pushA, pushB);
        printf("\ncalaM : \n"); for (int i = 15; i >= 0; i--) { printf("%d", (calaM >> i) & 1); }
        printf("\ncalbM : \n"); for (int i = 15; i >= 0; i--) { printf("%d", (calbM >> i) & 1); }
        //------------------------------------//
        if (aexp > bexp) { //a의 exp가 더 클 경우
            printf("\n\na의 exp가 더 클 경우\n");
            int gap = aexp - bexp;
            printf("gap : %d\n", gap);
            if (getSign(a) == getSign(b)) { //둘 다 양수거나 음수
                printf("\n\n둘 다 양수거나 음수");
                if (getSign(a)) {
                    applySign(&result, 1);
                }
                calaM <<= gap-(pushB-pushA);
                printf("\ncalaM : %s\n", sfp2bits(calaM));
                printf("\ncalbM : %s\n", sfp2bits(calbM));
                sumMs = calaM + calbM;
                printf("\nsumMs : %s\n", sfp2bits(sumMs));
                int push = 0;
                for (int i = 15; i >= 0; i--) {
                    if ((sumMs >> i) & 1) {
                        push = i;
                        break;
                    }
                }
                printf("push : %d", push);
                sumMs ^= 1 << push;
                sumMs <<= (10-push);
                printf("\nsumMs : %s\n", sfp2bits(sumMs));
                applyFrac(&result, sumMs);
                applyExp(&result, aexp);
            }
            else if (getSign(a) ^ getSign(b)) { //둘 중 하나만 양수
                if(getSign(a)) { //a만 음수
                    printf("\n\na만 음수\n");

                } else { //b만 음수
                    printf("\n\nb만 음수\n");
                    // applySign(&result, 1);
                    calaM <<= gap-(pushB-pushA);
                    // calaM <<= gap;
                    printf("%s\n", sfp2bits(calaM));
                    printf("%s\n", sfp2bits(calbM));
                    sumMs = calaM - calbM;
                    printf("sumMs : %s\n", sfp2bits(sumMs));
                    for (int i = 9; i >= 0; i--) {
                        if ((sumMs >> i) & 1) {
                            sumMs ^= 1 << i;
                            printf("sumMs : %s\n", sfp2bits(sumMs));
                            sumMs <<= 10-i;
                            printf("sumMs : %s\n", sfp2bits(sumMs));
                            break;
                        }
                    }
                    // sumMs ^= 1 << push;
                    // printf("sumMs : %s\n", sfp2bits(sumMs));
                    applyFrac(&result, sumMs);
                    printf("\nresult : %s\n", sfp2bits(result));
                    printf("\naexp : %d\n", aexp);
                    applyExp(&result, aexp);
                    printf("\nresult : %s\n", sfp2bits(result));
                }
            }
        }
        //------------------------------------//
        else if (aexp < bexp) { //b의 exp가 더 클 경우
            printf("\n\nb의 exp가 더 클 경우\n");
            int gap = bexp - aexp;
            printf("gap : %d\n", gap);
            if (getSign(a) == getSign(b)) { //둘다 양수거나 음수
                printf("\n\n둘 다 양수거나 음수");
                if (getSign(a)) {
                    applySign(&result, 1);
                }
calaM <<= gap-(pushB-pushA);
                printf("\ncalaM : %s\n", sfp2bits(calaM));
                printf("\ncalbM : %s\n", sfp2bits(calbM));
                sumMs = calaM + calbM;
                printf("\nsumMs : %s\n", sfp2bits(sumMs));
                int push = 0;
for (int i = 15; i >= 0; i--) {
    if ((sumMs >> i) & 1) {
        push = i;
        break;
    }
}
                printf("push : %d", push);
                sumMs ^= 1 << push;
sumMs <<= (10-push);
                printf("\nsumMs : %s\n", sfp2bits(sumMs));
                applyFrac(&result, sumMs);
                applyExp(&result, aexp);
            }
            else if (getSign(a) ^ getSign(b)) { //둘 중 하나만 양수
                if (getSign(a)) { //a만 음수
                }
                else { //b만 음수
                    printf("\n\nb만 음수\n");
                    applySign(&result, 1);
                    calbM <<= gap;
                    printf("%s\n", sfp2bits(calaM));
                    printf("%s\n", sfp2bits(calbM));
                    sumMs = calbM - calaM;
                    int push = 0;
                    for (int i = 9; i >= 0; i--) {
                        if ((sumMs >> i) & 1) {
                            push = 10-i;
                            break;
                        }
                    }
                    sumMs ^= 1 << push;
                    printf("sumMs : %s\n", sfp2bits(sumMs));
                    printf("\nsumMs : \n");
                    for (int i = 9; i >= 0; i--) {
                        result |= ((sumMs >> i) & 1) << (i+push);
                    }
                    printf("\nresult : %s\n", sfp2bits(result));
                    printf("\n");
                    bexp--;
                    printf("\nbexp : %d\n", bexp);
                    for(int i = 4; i >= 0; i--) {
                        printf("%d",(bexp >> i) & 1);
                        result |= ((bexp >> i) & 1) << (i+10);
                    }
                    printf("\n");
                }
            }
        }
        //------------------------------------//
        else { //같은 경우
            printf("\naexp, bexp가 같은경우");
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
            printf("\npush: %d\n", push);
            printf("\nsumMs : \n");
            for (int i = 9; i >= 0; i--) {
                printf("%d",(sumMs >> i) & 1);
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            aexp++;
            printf("\naexp : %d\n", aexp);
            for(int i = 4; i >= 0; i--) {
                printf("%d",(aexp >> i) & 1);
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        printf("\nresult : %s\n", sfp2bits(result));
        printf("\n");
    }

    else if (aexp == 0 && bexp == 0) { //둘다 demormal
        //------------------------------------//
        printf("\naM : ");
        for (int i = 9; i >= 0; i--) {
            printf("%d",(a >> i) & 1);
            calaM |= ((a >> i) & 1) << (i-10+aE); 
        }
        printf("\nbM : ");
        for (int i = 9; i >= 0; i--) {
            printf("%d",(b >> i) & 1);
            calbM |= ((b >> i) & 1) << (i-10+bE); 
        }
        //------------------------------------//
        if (aexp == bexp) { //같은 경우
            printf("\naexp, bexp가 같은경우");
            sumMs = calaM + calbM;
            int push = 0;
            for (int i = 9; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = 10-i;
                    break;
                }
            }
            ////////////
            printf("\ncalaM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calaM >> i) & 1);
            }
            printf("\ncalbM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calbM >> i) & 1);
            }
            ////////////
            printf("\nsumMs : \n");
            for (int i = 9; i >= 0; i--) {
                printf("%d",(sumMs >> i) & 1);
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            printf("\npush: %d\n", push);
            aexp ++;
            printf("aexp : %d\n", aexp);
            for(int i = 4; i >= 0; i--) {
                printf("%d",(aexp >> i) & 1);
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        //------------------------------------//
        else if (aexp > bexp) { //a의 exp가 더 클 경우
            printf("\na의 exp가 더 클 경우\n");
            int gap = aexp - bexp;
            printf("gap : %d\n", gap);

            ////////////
            printf("\ncalaM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calaM >> i) & 1);
            }
            printf("\ncalbM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calbM >> i) & 1);
            }
            ////////////
            sumMs = calaM + calbM;
            printf("\n");
            int push = 0;
            for (int i = 9; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = 10-i;
                    break;
                }
            }
            sumMs ^= 1 << push;
            printf("\nsumMs : \n");
            for (int i = 9; i >= 0; i--) {
                printf("%d",(sumMs >> i) & 1);
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            printf("\nresult : \n");
            for(int i = 15; i >= 0; i--) {
                printf("%d",(result >> i) & 1);
            }
            printf("\naexp : %d\n", aexp);
            for(int i = 4; i >= 0; i--) {
                printf("%d",(aexp >> i) & 1);
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        //------------------------------------//
        else if (aexp < bexp) { //b의 exp가 더 클 경우
            printf("\nb의 exp가 더 클 경우");
            int gap = bexp - aexp;
            printf("gap : %d\n", gap);

            ////////////
            printf("\ncalaM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calaM >> i) & 1);
            }
            printf("\ncalbM : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(calbM >> i) & 1);
            }
            ////////////
            sumMs = calaM + calbM;
            printf("\n");
            int push = 0;
            for (int i = 9; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = 10-i;
                    break;
                }
            }
            sumMs ^= 1 << push;
            printf("\nsumMs : \n");
            for (int i = 9; i >= 0; i--) {
                printf("%d",(sumMs >> i) & 1);
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            printf("\nresult : \n");
            for(int i = 15; i >= 0; i--) {
                printf("%d",(result >> i) & 1);
            }
            printf("\naexp : %d\n", aexp);
            for(int i = 4; i >= 0; i--) {
                printf("%d",(aexp >> i) & 1);
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        //------------------------------------//
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

    sfp calaM = 0;
    sfp calbM = 0;
    sfp mulMs = 0;

    //frac 곱하기
    if ((aexp > 0 && aexp < 31) && (bexp > 0 && bexp < 31)) { //둘 다 normal
        if(as != bs) {
            applySign(&result, 1);
        }
        printf("\n둘 다 normal\n");
        //aFrac 계산
        printf("aM : ");
        int pushA = 0;
        for (int i = 9; i >= 0; i--) {
            printf("%d",(a >> i) & 1);
            if (a >> i & 1) {
                pushA = i;
            }
        }
        for (int i = 9; i >= 0; i--) { calaM |= ((a >> i) & 1) << (i-pushA); }
        calaM |= 1 << (10-pushA);
        //bFrac 계산
        int pushB = 0;
        printf("\nbM : ");
        for (int i = 9; i >= 0; i--) {
            printf("%d",(b >> i) & 1);
            if (b >> i & 1) {
                pushB = i;
            }
        }
        for (int i = 9; i >= 0; i--) { calbM |= ((b >> i) & 1) << (i-pushB); }
        calbM |= 1 << (10-pushB);
        printf("\npushA : %d, pushB : %d", pushA, pushB);
        printf("\ncalaM : "); for (int i = 15; i >= 0; i--) { printf("%d", (calaM >> i) & 1); }
        printf("\ncalbM : "); for (int i = 15; i >= 0; i--) { printf("%d", (calbM >> i) & 1); }
        //----------------------------------------//
        mulMs = calaM * calbM;
        printf("\nmulMs : %s\n", sfp2bits(mulMs));
        int push = 0;
        for (int i = 15; i >= 0; i--) {
            if ((mulMs >> i) & 1) {
                push = i;
                break;
            }
        }
        printf("\npush : %d\n", push);
        int exp = aE+bE+15;
        printf("exp : %d\n", exp);
        if (push - ((10-pushA) + (10-pushB))) {
            exp += (push - ((10-pushA) + (10-pushB)));
        }
        printf("exp : %d\n", exp);
        printf("aE + bE : %d\n", aE+bE);
        mulMs ^= 1 << push;
        printf("mulMs : %s\n", sfp2bits(mulMs));
        mulMs <<= (10-push);
        printf("mulMs : %s\n", sfp2bits(mulMs));
        applyExp(&result, exp);
        applyFrac(&result, mulMs);
        printf("result : %s\n", sfp2bits(result));
    }
    
    else if (aexp == 0 && bexp == 0) { //둘다 demormal
        
    }

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