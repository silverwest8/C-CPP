#include "sfp.h"
#include <stdio.h>
#include <stdlib.h>
sfp getNaN() {
    sfp NaN = 0;
    for (int i = 15; i > 0; i--) {
        NaN |= (1<<i);
    }
    for (int i = 9; i >= 0; i--) {
        printf("%d", (NaN << i) & 1);
    }
    return NaN;
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
    printf("sign : %d, exp : %d E : %d\n", *sign, *exp, *E);
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

sfp int2sfp(int input) {
    
    sfp result = 0;
    printf("input: %d\n", input);
    for(int i = 31; i >= 0; i--) {
        printf("%d",(input >> i) & 1);
	}
    printf("\n");
    //2진수 변경 //만 백..?

    //sign 비트 설정
    applySign(&result, (input >> 31) & 1);
    printf("\nresult : %s\n", sfp2bits(result));

    //exp 설정
    int E = 0;
    int bias = 15;
    for(int i = 30; i >= 0; i--) {
        printf("%d", (input >> i) & 1);
        if ((input >> i) & 1) {
            E = i;
            break;
        }
	}
    printf("\n");
    int exp = E + bias;

    //frac 설정
    printf("frac : \n");
    if (E == 0) { //denormalized
        printf("\n case 0 \n");
    }
    else if (exp > 0 && exp < 31) { //normalized 
        for(int i = 4; i >= 0; i--) {
            printf("%d",(exp >> i) & 1);
            result |= ((exp >> i) & 1) << (i+10);
        }
        printf("\n\n");
        printf("result : \n");
        for(int i = 15; i >= 0; i--) {
            printf("%d",(result >> i) & 1);
        }
        printf("\n\n");
        for(int i = 9; i >= 0; i--) {
            printf("%d", ((input >> (E+i-10)) & 1));
            result |= ((input >> (E+i-10)) & 1) << i;
        }
    }
    else if (exp >= 31) { //special
        int frac = 0;
        exp = 31;
        for(int i = 4; i >= 0; i--) {
            printf("%d",(exp >> i) & 1);
            result |= ((exp >> i) & 1) << (i+10);
        }
        printf("\n\n");
        printf("result : \n");
        for(int i = 15; i >= 0; i--) {
            printf("%d",(result >> i) & 1);
        }
        for(int i = 9; i >= 0; i--) {
            printf("%d", 0);
            result |= (frac >> 9) & 1;
        }
    }


    printf("\n\n");
    printf("result : \n");
    for(int i = 15; i >= 0; i--) {
        printf("%d",(result >> i) & 1);
	}
    printf("\n\n");
	return result;
}

int sfp2int(sfp input) {
    printf("input : \n");
    for (int i = 15; i >=0; i--) {
        printf("%d", (input >> i) & 1);
    }
    int result = 0;
    printf("\nresult: %d\n", result);
    for (int i = 31; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    
    //input이 양의 무한대 exp = 11..1, frac = 00..0

    printf("\n\n");
    //input이 음의 무한대 exp = 11..1, frac = 00..0
    //+- NaN exp = 11..1, frac != 00..0
    //round toward zero
    //denorm
    //norm

    //부호
    printf("sign : %d\n", (input >> 15) & 1);
    result |= ((input >> 15) & 1) << 31;
    
    printf("result : \n");
    for(int i = 31; i >= 0; i--) {
        printf("%d",(result >> i) & 1);
    }
    printf("\n\n");

    //exp = E + bias
    int exp = 0;
    int temp = 0;
    printf("exp : ");
    for (int i = 14; i >= 10; i--) {
        printf("%d",(input >> i) & 1);
        if (input >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-10;
            exp += temp;
        }
    }
    printf("\nexp : %d\n", exp);

    //frac
    if (exp > 0 && exp < 31) {
        int E = exp - 15;
        printf("\nE : %d\n", E);
        for (int i = 9; i >= 0; i--) {
            printf("%d",(input >> i) & 1);
            result |= ((input >> i) & 1) << (i-(10-E));
        }
        result |= 1 << E;
        printf("\n\n");
    } else if (exp == 0) {
        //그대로
    } else if (exp == 31) {
        int Tmin = 1;
        Tmin <<= 31;
        printf("\nTmin : %d\n", Tmin);
        int Tmax = 1;
        Tmax <<= 31;
        Tmax--;
        printf("Tmax : %d\n", Tmax);
        for (int i = 9; i >= 0; i--) {
            if ((input >> i) & 1) {
                for(int i = 15; i >= 0; i--) {
                    result |= ((Tmin >> i) & 1) << i;
                }
                break; // NaN인 경우
            }
        }
        if ((input >> 15) & 1) {
            //-무한대 -> Tmin, -NaN -> Tmin, +NaN -> Tmin
            for(int i = 15; i >= 0; i--) {
                result |= ((Tmin >> i) & 1) << i;
            }
        }
        else {
            //+무한대 -> Tmax, 
            for(int i = 31; i >= 0; i--) {
                result |= ((Tmax >> i) & 1) << i;
            }
        }
    }
    printf("\n\nresult: %d\n", result);
    for (int i = 31; i >=0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf("\n\n");
    return result;
}

sfp float2sfp(float input) {
    sfp result = 0;
    printf("input : %f\n", input);
    unsigned save = *(unsigned *)&input;
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n\n");

    //sign
    printf("sign : ");
    printf("%d",(save >> 31) & 1);
    result |= ((save >> 31) & 1) << 15;
    printf("\nresult:\n");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }

    //exp
    printf("\n\n");
    int fexp = 0;
    int temp = 0;
    printf("fexp : ");
    for (int i = 30; i >= 23; i--) {
        printf("%d",(save >> i) & 1);
        if (save >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-23;
            fexp += temp;
        }
    }
    printf("\nexp : %d\n", fexp);
    int E = fexp - 127;
    printf("\nE : %d\n", E);

    int sfpexp = E + 15;
    printf("\nsfpexp : %d\n", sfpexp);

    if (sfpexp < 0) {
        //+-무한대
        sfpexp = 31;
        printf("\nsfpexp : %d\n", sfpexp);
    }
    for(int i = 4; i >= 0; i--) {
        printf("%d", (sfpexp >> i) & 1);
        result |= ((sfpexp >> i) & 1) << (i+10);
	}
    printf("\n\n");
    printf("result:\n");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf("\n\n");

    //frac
    printf("Ffrac : \n");
    for(int i = 22; i >= 13; i--) {
        printf("%d", (save >> i) & 1);
        result |= ((save >> i) & 1) << (i-13);
	}
    printf("\n\n");
    printf("result:\n");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf("\n\n");
    return result;
}

float sfp2float(sfp input) {
    float result = 0;
    unsigned save = *(unsigned *)&result;
    printf("input : ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (input >> i) & 1);
    }
    printf("\n\n");

    //sign
    printf("sign : ");
    printf("%d",(input >> 15) & 1);
    save |= ((input >> 15) & 1) << 31;
    printf("\n\n");
    printf("result : ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }

    //exp
    printf("\n\n");
    int sfpexp = 0;
    int temp = 0;
    printf("sfpexp : ");
    for (int i = 14; i >= 10; i--) {
        printf("%d",(input >> i) & 1);
        if (input >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-10;
            sfpexp += temp;
        }
    }
    printf("\n\n");
    printf("sfpexp : %d\n", sfpexp);
    printf("\n\n");
    int E = sfpexp - 15;
    printf("E : %d\n", E);
    printf("\n\n");

    int fexp = E + 127;
    printf("fexp : %d\n", fexp);
    for(int i = 7; i >= 0; i--) {
        printf("%d", (fexp >> i) & 1);
        save |= ((fexp >> i) & 1) << (i+23);
	}
    printf("\n\n");
    printf("result:\n");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n\n");

    //frac
    printf("frac : \n");
    for(int i = 10; i >= 0; i--) {
        printf("%d", (input >> i) & 1);
        save |= ((input >> i) & 1) << (i+13);
	}
    printf("\n\n");
    printf("result:\n");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (save >> i) & 1);
    }
    printf("\n\n");
    result = *(float*)&save;
    return result;
}

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
                }
            }
        }
        //------------------------------------//
        else if (aexp < bexp) { //b의 exp가 더 클 경우
            printf("\n\nb의 exp가 더 클 경우\n");
            int gap = bexp - aexp; printf("gap : %d\n", gap);
            printf("\ncalaM : \n"); for (int i = 15; i >= 0; i--) { printf("%d",(calaM >> i) & 1); }
            printf("\ncalbM : \n"); for (int i = 15; i >= 0; i--) { printf("%d",(calbM >> i) & 1); }
            if (getSign(a) && getSign(b)) { //둘다 음수
                printf("둘 다 음수\n");
                sumMs = calaM + calbM;
                applySign(&result, 1);
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
                printf("\nresult : %s\n", sfp2bits(result));
                printf("\naexp : %d\n", aexp);
                for(int i = 4; i >= 0; i--) {
                    printf("%d",(aexp >> i) & 1);
                    result |= ((aexp >> i) & 1) << (i+10);
                }
                printf("\n");
            }
            else if (!getSign(a) && !getSign(b)) { //둘다 양수
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
                printf("\nresult : %s\n", sfp2bits(result));
                printf("\n");
                printf("\naexp : %d\n", aexp);
                for(int i = 4; i >= 0; i--) {
                    printf("%d",(aexp >> i) & 1);
                    result |= ((aexp >> i) & 1) << (i+10);
                }
                printf("\n");
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
            calaM |= ((a >> i) & 1) << (i-10+aE); //4 3 2 1 0
        }
        printf("\nbM : ");
        for (int i = 9; i >= 0; i--) {
            printf("%d",(b >> i) & 1);
            calbM |= ((b >> i) & 1) << (i-10+bE); //9-3 = 6 // 2 1 0
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
        printf("\n둘 다 normal\n");
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
        //----------------------------------------//

        if (aexp > bexp) { //a의 exp가 더 클 경우
            mulMs = calaM * calbM;
            printf("\n");
            int push = 0;
            for (int i = 15; i >= 0; i--) {
                if ((mulMs >> i) & 1) {
                    push = i;
                    break;
                }
            }
            printf("push : %d\n", push);
            printf("\nmulMs : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(mulMs >> i) & 1);
            }
            printf("\n");
            mulMs ^= 1 << push;
            mulMs <<= (10-push);
            for (int i = 9; i >= 0; i--) {
                printf("%d",(mulMs >> i) & 1);
                result |= ((mulMs >> i) & 1) << i;
            }
            printf("\nresult : \n");
            for(int i = 15; i >= 0; i--) {
                printf("%d",(result >> i) & 1);
            }
            printf("\n");
            int exp = push+15; //push 10-25, 9-24, 7-22
            printf("\nexp : %d\n", exp);
            for (int i = 4; i >= 0; i--) {
                printf("%d", (exp >> i) & 1);
                result |= ((exp >> i) & 1) << (i+10);
            }
        }
        else if (aexp < bexp) { //b의 exp가 더 클 경우

        }
        else { //a와 b의 exp가 같은 경우
            mulMs = calaM * calbM;
            printf("\n");
            int push = 0;
            for (int i = 15; i >= 0; i--) {
                if ((mulMs >> i) & 1) {
                    push = i;
                    break;
                }
            }
            printf("push : %d\n", push);
            printf("\nmulMs : \n");
            for (int i = 15; i >= 0; i--) {
                printf("%d",(mulMs >> i) & 1);
            }
            printf("\n");
            mulMs ^= 1 << push;
            mulMs <<= (10-push);
            for (int i = 9; i >= 0; i--) {
                printf("%d",(mulMs >> i) & 1);
                result |= ((mulMs >> i) & 1) << i;
            }
            printf("\nresult : \n");
            for(int i = 15; i >= 0; i--) {
                printf("%d",(result >> i) & 1);
            }
            printf("\n");
            int exp = push+15; //push 10-25, 9-24, 7-22
            printf("\nexp : %d\n", exp);
            for (int i = 4; i >= 0; i--) {
                printf("%d", (exp >> i) & 1);
                result |= ((exp >> i) & 1) << (i+10);
            }
        }
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