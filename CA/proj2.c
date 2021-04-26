#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void substring(char inst[], char result[], int start, int len);
int typeCheck(char inst[]);
int R_funcCheck(char inst[], unsigned int Registers[]);
int I_opcodeCheck(char inst[], unsigned int Registers[], unsigned char DataMem[][4], unsigned char InstMem[][4], int* num);
int R_rd(char inst[]);
int R_rs(char inst[]);
int R_rt(char inst[]);
int R_sa(char inst[]);
int I_rs(char inst[]);
int I_rt(char inst[]);
int I_im(char inst[]);
int I_im_zeroEx(char inst[]);
int I_label(char inst[]);
int J_opcodeCheck(char inst[], int* num);
int J_target(char inst[]);
void Reg(unsigned int Registers[], int num);
void Mem(unsigned char DataMem[][4], int startAddress);

void substring(char inst[], char result[], int start, int len) {
    int i = 0;
    while (i < len) {
        result[i] = inst[start+i];
        i++;
    }
}
int typeCheck(char inst[]) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "000000") == 0) {
        return 0; //Rtype
    } else if (strcmp(result, "000010") == 0 || strcmp(result, "000011") == 0) {
        return 2; //Jtype
    }
    else {
        return 1; //Itype
    }
}
int R_funcCheck(char inst[], unsigned int Registers[]) {
    char result[6] = "";
    substring(inst, result, 26, 6);
    if (strcmp(result, "100000") == 0) {
        //
        Registers[R_rd(inst)] = Registers[R_rs(inst)] + Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "100001") == 0) {
        return 0;
    }
    else if (strcmp(result, "100100") == 0) {
        //
        Registers[R_rd(inst)] = Registers[R_rs(inst)] & Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "011010") == 0) {
        return 1;
    }
    else if (strcmp(result, "011011") == 0) {
        return 1;
    }
    else if (strcmp(result, "001001") == 0) {
        return 2;
    }
    else if (strcmp(result, "001000") == 0) {
        return 3;
    }
    else if (strcmp(result, "010000") == 0) {
        return 4;
    }
    else if (strcmp(result, "010010") == 0) {
        return 4;
    }
    else if (strcmp(result, "010001") == 0) {
        return 3;
    }
    else if (strcmp(result, "010011") == 0) {
        return 3;
    }
    else if (strcmp(result, "011000") == 0) {
        return 1;
    }
    else if (strcmp(result, "011001") == 0) {
        return 1;
    }
    else if (strcmp(result, "100111") == 0) {
        return 0;
    }
    else if (strcmp(result, "100101") == 0) {
        //
        Registers[R_rd(inst)] = Registers[R_rs(inst)] | Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "000000") == 0) {
        //
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] << R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000100") == 0) {
        return 6;
    }
    else if (strcmp(result, "101010") == 0) {
        ////////
        if ((signed)Registers[R_rs(inst)] < (signed)Registers[R_rt(inst)]) {
            Registers[R_rd(inst)] = 1;
        } else {
            Registers[R_rd(inst)] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "101011") == 0) {
        return 0;
    }
    else if (strcmp(result, "000011") == 0) {
        return 5;
    }
    else if (strcmp(result, "000111") == 0) {
        return 6;
    }
    else if (strcmp(result, "000010") == 0) {
        //
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] >> R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000110") == 0) {
        return 6;
    }
    else if (strcmp(result, "100010") == 0) {
        //
        Registers[R_rd(inst)] = Registers[R_rs(inst)] - Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "100011") == 0) {
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        return 7;
    }
    else if (strcmp(result, "100110") == 0) {
        return 0;
    }
    return 8;
}
int I_opcodeCheck(char inst[], unsigned int Registers[], unsigned char DataMem[][4], unsigned char InstMem[][4], int* num) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "001000") == 0) {
        //
        Registers[I_rt(inst)] = Registers[I_rs(inst)] + I_im(inst);
        return 0;
    }
    else if (strcmp(result, "001001") == 0) {
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        //
        Registers[I_rt(inst)] = (Registers[I_rs(inst)] & I_im_zeroEx(inst));
        return 0;
    }
    else if (strcmp(result, "000100") == 0) {
        //
        if (Registers[I_rs(inst)] == Registers[I_rt(inst)]) {
            *num+=(I_im(inst));
        }
        return 1;
    }
    else if (strcmp(result, "000101") == 0) {
        //if(R[$rs] != R[$rt]) PC ← PC + 4 + SignExt18b({imm, 00})
        if (Registers[I_rs(inst)] != Registers[I_rt(inst)]) {
            *num+=(I_im(inst));
        }
        return 1;
    }
    else if (strcmp(result, "100000") == 0) {
        //R[$rt] ← SignExt8b(Mem1B(R[$rs] + SignExt16b(imm)))
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst) % 4];
        if ((temp >> 7) & 1) {
            temp |= 0xffffff00;
        }
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "100100") == 0) {
        //
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im_zeroEx(inst) % 4];
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "100001") == 0) {
        //
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4 < 0 ? I_im(inst)%4+4 : I_im(inst)%4];
        temp <<= 8;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4 < 0 ? I_im(inst)%4+5 : I_im(inst)%4+1];
        if ((temp >> 15) & 1) {
            temp |= 0xffff0000;
        }
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "100101") == 0) {
        //
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4];
        temp <<= 8;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4+1];
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "001111") == 0) {
        //lui load upper immediate/ lui $s1, 100 $s1=100*216 $s1의 상위 16비트에 constant 100을 로드
        Registers[I_rt(inst)] = (I_im(inst) << 16);
        return 3;
    }
    else if (strcmp(result, "100011") == 0) {
        //lw load word/ lw $s1, 100($s2)
        //$s1=memory[$s2 + 100] : $s1에 메모리 베이스 주소 $s2에 100을 더한 $s2+100의 주소에 있는 값을 저장
        unsigned int temp = 0;
        for (int i = 31; i >= 24 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][0];
        }
        temp <<= 8;
        for (int i = 23; i >= 16 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][1];
        }
        temp <<= 8;
        for (int i = 15; i >= 8 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][2];
        }
        temp <<= 8;
        for (int i = 7; i >= 0 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][3];
        }
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "001101") == 0) {
        //
        Registers[I_rt(inst)] = Registers[I_rs(inst)] | I_im_zeroEx(inst);
        return 0;
    }
    else if (strcmp(result, "101000") == 0) {
        //
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i);
            DataMem[Registers[I_rs(inst)]/4+I_im(inst)/4-(0x10000000)/4][I_im(inst)%4] = ch;
        }
        return 2;
    }
    else if (strcmp(result, "001010") == 0) {
        //
        if ((signed)Registers[I_rs(inst)] < I_im(inst)) {
            Registers[I_rt(inst)] = 1;
        } else {
            Registers[I_rt(inst)] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "001011") == 0) {
        return 0;
    }
    else if (strcmp(result, "101001") == 0) {
        //
        for (int i = 15; i >= 8 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i-8);
            DataMem[Registers[I_rs(inst)]/4+I_im(inst)/4-(0x10000000)/4][I_im(inst)%4+1] = ch;
        }
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i);
            DataMem[Registers[I_rs(inst)]/4+I_im(inst)/4-(0x10000000)/4][I_im(inst)%4] = ch;
        }
        return 2;
    }
    else if (strcmp(result, "101011") == 0) {
        //
        char ch = 0;
        for (int i = 31; i >= 24 ; i--) {
            ch |= (Registers[I_rt(inst)] >> i) << (i-24);
        }
        DataMem[(Registers[I_rs(inst)]+I_im(inst)-(0x10000000))/4][0] = ch;
        ch = 0;
        for (int i = 23; i >= 16 ; i--) {
            ch |= (Registers[I_rt(inst)] >> i) << (i-16);
        }
        DataMem[(Registers[I_rs(inst)]+I_im(inst)-(0x10000000))/4][1] = ch;
        ch = 0;
        for (int i = 15; i >= 8 ; i--) {
            ch |= (Registers[I_rt(inst)] >> i) << (i-8);
        }
        DataMem[(Registers[I_rs(inst)]+I_im(inst)-(0x10000000))/4][2] = ch;
        ch = 0;
        for (int i = 7; i >= 0 ; i--) {
            ch |= (Registers[I_rt(inst)] >> i) << (i);
        }
        DataMem[(Registers[I_rs(inst)]+I_im(inst)-(0x10000000))/4][3] = ch;
        return 2;
    }
    else if (strcmp(result, "001110") == 0) {
        return 0;
    }
    return 5;
}
int R_rd(char inst[]) {
    char result[5] = "";
    substring(inst, result, 16, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_rs(char inst[]) {
    char result[5] = "";
    substring(inst, result, 6, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_rt(char inst[]) {
    char result[5] = "";
    substring(inst, result, 11, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_sa(char inst[]) {
    char result[5] = "";
    substring(inst, result, 21, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_rs(char inst[]) {
    char result[5] = "";
    substring(inst, result, 6, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_rt(char inst[]) {
    char result[5] = "";
    substring(inst, result, 11, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_im(char inst[]) {
    char result[16] = "";
    substring(inst, result, 16, 16);
    int im = 0;
    for (int i = 15; i >= 0; i--) {
        if (result[i]) {
            if (i == 0) {
                im -= (result[i]-48) << (15-i);
            } else {
                im += (result[i]-48) << (15-i);
            }
        }
    }
    return im;
}
int I_im_zeroEx(char inst[]) {
    char result[16] = "";
    substring(inst, result, 16, 16);
    int im = 0;
    for (int i = 15; i >= 0; i--) {
        if (result[i]) {
            im += (result[i]-48) << (15-i);

        }
    }
    return im;
}
int I_label(char inst[]) {
    char result[16] = "";
    substring(inst, result, 16, 16);
    int label = 0;
    for (int i = 15; i >= 0; i--) {
        if (result[i]) {
            if (i == 0) {
                label -= (result[i]-48) << (15-i);
            } else {
                label += (result[i]-48) << (15-i);
            }
        }
    }
    return label;
}
int J_opcodeCheck(char inst[], int* num) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "000010") == 0) {
        //PC(num)의 첫 4bit + target 26bit를 shift left 2 한거
        unsigned int target = 0;
        target = J_target(inst) << 2;
        int origin = *num*4;
        int result = 0;
        origin >>= 28;
        origin <<= 28;
        result = origin | target;
        *num = result/4;
        (*num)--;
        return 0;
    }
    else {
        return 1;
    }
}
int J_target(char inst[]) {
    char result[26] = "";
    substring(inst, result, 6, 26);
    int target = 0;
    for (int i = 25; i >= 0; i--) {
        if (result[i]) {
            target += (result[i]-48) << (25-i);
        }
    }
    return target;
}
void Reg(unsigned int Registers[], int num) {
    for (int i = 0; i < 32; i++) {
        printf("$%d: %08x\n", i, Registers[i]);
    }
    printf("PC: %08x\n", num*4);
}

void Mem(unsigned char DataMem[][4], int startAddress) {
    for (int k = 0; k < 4; k++) {
        printf("0x");
        for (int i = 0; i < 4; i++) {
            printf("%02x", (DataMem[(startAddress-0x10000000)/4+k][i]));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int N = atoi(argv[2]);
    unsigned int Registers[32];
    unsigned char InstMem[65536/4][4];
    unsigned char DataMem[65536/4][4];
    memset(Registers, 0x00000000, sizeof(Registers));
    memset(InstMem, 0xff, sizeof(InstMem));
    memset(DataMem, 0xff, sizeof(DataMem));
    int arr[4];
    char inst[32];
    int idx;
    FILE *f_input = fopen(argv[1], "rb");

    int x = 0;
    while(1) {
        int result = fread(InstMem[x], 1, 4, f_input);
        if (result != 4) {
            break;
        }
        x++;
    }

    int num = 0;
    int checkN = 1;
    int unknown = 0;

    while(checkN <= N && unknown == 0) {
        idx = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 7; j >= 0; j--) {
                if ((InstMem[num][i] >> j) & 1) {
                    inst[idx] = '1';
                } else {
                    inst[idx] = '0';
                }
                idx++;
            }
        }
        int opcode = typeCheck(inst);
        if (opcode == 0) { //R-type
            switch(R_funcCheck(inst, Registers)) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            default:
                unknown = 1;
            }
        } else if (opcode == 1) { //I-type
            switch (I_opcodeCheck(inst, Registers, DataMem, InstMem, &num)) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                unknown = 1;
            }
        } else if (opcode == 2) { //J-type
            J_opcodeCheck(inst, &num);
        }
        num++;
        checkN++;
    }
    if (unknown == 1) {
        printf("unknown instruction\n");
    }
    if (argv[3] != NULL) {
        if (!strcmp(argv[3], "reg")) {
            Reg(Registers, num);
        }
        if (!strcmp(argv[3], "mem")) {
            Mem(DataMem, strtol(argv[4], NULL, 16));
        }
    }

    fclose(f_input); // 파일 닫기
    return 0;
}