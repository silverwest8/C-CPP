#include "sfp.h"
#include <stdio.h>
#include <stdlib.h>

int Check(sfp input) {
    for (int i = 15; i >= 0; i--) {
        if (((input >> i) & 1) == 0) {
            if (i == 0) {
                return 0; //0
            }
        }
    }
    for (int i = 14; i >= 10; i--) {
        if ((input >> i) & 1) {
            continue;
        }
        else {
            return -1; //not special
        }
    }
    for (int j = 9; j >= 0; j--) {
        if (((input >> j) & 1) == 0) {
            if (j == 0) {
                if (input >> 15 & 1){
                    return 2; //-infinity
                }
                else {
                    return 1; //+infinity
                }
            }
        }
        else {
            return 3; //NaN
        }
    }
    return -1;
}

sfp int2sfp(int input) {
    
    sfp result = 0;
    //sign 비트 설정
    result |= ((input >> 31) << 15);
    //exp 설정
    int E = 0;
    int bias = 15;
    for(int i = 30; i >= 0; i--) {
        if ((input >> i) & 1) {
            E = i;
            break;
        }
	}
    int exp = E + bias;

    //frac 설정
    if (E == 0) { //denormalized
    }
    else if (exp > 0 && exp < 31) { //normalized 
        for(int i = 4; i >= 0; i--) {
            result |= ((exp >> i) & 1) << (i+10);
        }
        for(int i = 9; i >= 0; i--) {
            result |= ((input >> (E+i-10)) & 1) << i;
        }
    }
    else if (exp >= 31) { //special
        int frac = 0;
        exp = 31;
        for(int i = 4; i >= 0; i--) {
            result |= ((exp >> i) & 1) << (i+10);
        }
        for(int i = 9; i >= 0; i--) {
            result |= (frac >> 9) & 1;
        }
    }
	return result;
}

int sfp2int(sfp input) {
    int result = 0;
    //input이 양의 무한대 exp = 11..1, frac = 00..0
    //input이 음의 무한대 exp = 11..1, frac = 00..0
    //+- NaN exp = 11..1, frac != 00..0
    //round toward zero
    //denorm
    //norm

    //부호
    result |= ((input >> 15) & 1) << 31;

    //exp = E + bias
    int exp = 0;
    int temp = 0;
    for (int i = 14; i >= 10; i--) {
        if (input >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-10;
            exp += temp;
        }
    }

    //frac
    if (exp > 0 && exp < 31) {
        int E = exp - 15;
        for (int i = 9; i >= 0; i--) {
            result |= ((input >> i) & 1) << (i-(10-E));
        }
        result |= 1 << E;
    } else if (exp == 0) {
        //그대로
    } else if (exp == 31) {
        int Tmin = 1;
        Tmin <<= 31;
        int Tmax = 1;
        Tmax <<= 31;
        Tmax--;
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
    return result;
}

sfp float2sfp(float input) {
    sfp result = 0;
    unsigned save = *(unsigned *)&input;

    //sign
    result |= ((save >> 31) & 1) << 15;
    //exp
    int fexp = 0;
    int temp = 0;
    for (int i = 30; i >= 23; i--) {
        if (save >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-23;
            fexp += temp;
        }
    }
    int E = fexp - 127;
    int sfpexp = E + 15;

    if (sfpexp < 0) { //+-무한대
        sfpexp = 31;
    }
    for(int i = 4; i >= 0; i--) {
        result |= ((sfpexp >> i) & 1) << (i+10);
	}
    //frac
    for(int i = 22; i >= 13; i--) {
        result |= ((save >> i) & 1) << (i-13);
	}
    return result;
}

float sfp2float(sfp input) {
    float result = 0;
    unsigned save = *(unsigned *)&result;
    //sign
    save |= ((input >> 15) & 1) << 31;

    //exp
    int sfpexp = 0;
    int temp = 0;
    for (int i = 14; i >= 10; i--) {
        if (input >> i & 1) {
            temp = 0;
            temp |= 1;
            temp <<= i-10;
            sfpexp += temp;
        }
    }
    int E = sfpexp - 15;
    int fexp = E + 127;
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

sfp sfp_add(sfp a, sfp b) {
    sfp result = 0;
    //지수구하기
    int aexp = 0;
    int atemp = 0;
    for (int i = 14; i >= 10; i--) {
        if (a >> i & 1) {
            atemp = 0;
            atemp |= 1;
            atemp <<= i-10;
            aexp += atemp;
        }
    }
    int aE = aexp - 15;
    int bexp = 0;
    int btemp = 0;
    for (int i = 14; i >= 10; i--) {
        if (b >> i & 1) {
            btemp = 0;
            btemp |= 1;
            btemp <<= i-10;
            bexp += btemp;
        }
    }
    int bE = bexp - 15;
    //지수조정

    //frac 연산
    sfp calaM = 0;
    sfp calbM = 0;
    sfp sumMs = 0;

    if ((aE > 0 && aE < 31) && (bE > 0 && bE < 31)) { //둘다 normal
        for (int i = 9; i >= 0; i--) {
            calaM |= ((a >> i) & 1) << (i-10+aE); //4 3 2 1 0
        }
        calaM |= 1 << aE;
        for (int i = 9; i >= 0; i--) {
            calbM |= ((b >> i) & 1) << (i-10+bE); //9-3 = 6 // 2 1 0
        }
        calbM |= 1 << bE;
        if (aexp > bexp) { //a의 exp가 더 클 경우
            int gap = aexp - bexp;
            sumMs = calaM + calbM;
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
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        else if (aexp < bexp) { //b의 exp가 더 클 경우
            int gap = bexp - aexp;
            sumMs = calaM + calbM;
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
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        else { //같은 경우
            sumMs = calaM + calbM;
            int push = 0;
            for (int i = 9; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = 10-i;
                    break;
                }
            }
            for (int i = 9; i >= 0; i--) {
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            aexp ++;
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
    }
    else {
        for (int i = 9; i >= 0; i--) {
            calaM |= ((a >> i) & 1) << (i-10+aE);
        }
        for (int i = 9; i >= 0; i--) {
            calbM |= ((b >> i) & 1) << (i-10+bE);
        }
        if (aexp == bexp) { //같은 경우
            sumMs = calaM + calbM;
            int push = 0;
            for (int i = 9; i >= 0; i--) {
                if ((sumMs >> i) & 1) {
                    push = 10-i;
                    break;
                }
            }
            for (int i = 9; i >= 0; i--) {
                result |= ((sumMs >> i) & 1) << (i+push);
            }
            aexp ++;
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        else if (aexp > bexp) { //a의 exp가 더 클 경우
            int gap = aexp - bexp;
            sumMs = calaM + calbM;
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
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
        else if (aexp < bexp) { //b의 exp가 더 클 경우
            int gap = bexp - aexp;
            sumMs = calaM + calbM;
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
            for(int i = 4; i >= 0; i--) {
                result |= ((aexp >> i) & 1) << (i+10);
            }
        }
    }
    return result;
}

sfp sfp_mul(sfp a, sfp b){
    sfp result = 0;
    if (Check(a) == 1 && Check(b) == 1) {
        return a;
    } else if (Check(a) == 1 && Check(b) == 2 || Check(a) == 2 && Check(b) == 1) {
        if (Check(a) == 2) {
            return a;
        } else {
            return b;
        }
    } else if (Check(a) == 2 && Check(b) == 2) {
        a ^= 1 << 15;
        return a;
    } else if (Check(a) == 1 && Check(b) == -1 || Check(a) == -1 && Check(b) == 1) {
        if(Check(a) == 1) {
            return a;
        } else {
            return b;
        }
    } else if (Check(a) == 2 && Check(b) == -1 || Check(a) == -1 && Check(b) == 2) {
        if(Check(a) == -1) {
            return a;
        } else {
            return b;
        }
    } else if (Check(a) == 1 && Check(b) == 0 || Check(a) == 2 && Check(b) == 0) {
        if (Check(a) == 1) {
            a ^= 1<<9;
            return a;
        } else {
            b ^= 1<<9;
            return b;
        }
    } else if (Check(a) == 3 || Check(b) == 3) {
        if (Check(a) == 3) {
            return a;
        } else {
            return b;
        }
    }
    int aexp = 0;
    int atemp = 0;
    for (int i = 14; i >= 10; i--) {
        if (a >> i & 1) {
            atemp = 0;
            atemp |= 1;
            atemp <<= i-10;
            aexp += atemp;
        }
    }
    int aE = aexp - 15;
    int bexp = 0;
    int btemp = 0;
    for (int i = 14; i >= 10; i--) {
        if (b >> i & 1) {
            btemp = 0;
            btemp |= 1;
            btemp <<= i-10;
            bexp += btemp;
        }
    }
    int bE = bexp - 15;
    //지수끼리 더하기
    if (aexp == 31) {
        sfp infinityP = 0;
        sfp infinityM = 0;
        return result;
    }

    //frac 곱하기
    int E = aE + bE;
    sfp calaM = 0;
    sfp calbM = 0;
    sfp mulMs = 0;

    if ((aexp > 0 && aexp < 31) && (bexp > 0 && bexp < 31)) {
        for (int i = 9; i >= 0; i--) {
            calaM |= ((a >> i) & 1) << (i-10+aE); //4 3 2 1 0
        }
        calaM |= 1 << aE;
        for (int i = 9; i >= 0; i--) {
            calbM |= ((b >> i) & 1) << (i-10+bE); //9-3 = 6 // 2 1 0
        }
        calbM |= 1 << bE;
        mulMs = calaM * calbM;
        int push = 0;
        for (int i = 15; i >= 0; i--) {
            if ((mulMs >> i) & 1) {
                push = i;
                break;
            }
        }
        mulMs ^= 1 << push;
        mulMs <<= (10-push);
        for (int i = 9; i >= 0; i--) {
            result |= ((mulMs >> i) & 1) << i;
        }
        int exp = push+15; //push 10-25, 9-24, 7-22
        for (int i = 4; i >= 0; i--) {
            result |= ((exp >> i) & 1) << (i+10);
        }
    }
    else {
        
    }
    return result;
}

char* sfp2bits(sfp result) {
    char * bitStream = malloc(sizeof(char)*17);
    for(int i = 15; i >= 0; i--) {
        if ((result >> i) & 1) {
            bitStream[15-i] = '1';
        } else {
            bitStream[15-i] = '0';
        }
	}
    bitStream[16] = '\0';
    return bitStream;
}