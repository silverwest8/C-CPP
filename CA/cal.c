#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void substring(char inst[], char result[], int start, int len);
int typeCheck(char inst[]);
int R_funcCheck(char inst[], unsigned int Registers[]);
int I_opcodeCheck(char inst[], unsigned int Register[], unsigned char Datamem[][4], int* num);
int R_rd(char inst[]);
int R_rs(char inst[]);
int R_rt(char inst[]);
int R_sa(char inst[]);
int I_rs(char inst[]);
int I_rt(char inst[]);
int I_im(char inst[]);
int I_label(char inst[]);
int J_opcodeCheck(char inst[]);
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
        printf("add ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] + Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "100001") == 0) {
        printf("addu ");
        return 0;
    }
    else if (strcmp(result, "100100") == 0) {
        //
        printf("and ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] & Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "011010") == 0) {
        printf("div ");
        return 1;
    }
    else if (strcmp(result, "011011") == 0) {
        printf("divu ");
        return 1;
    }
    else if (strcmp(result, "001001") == 0) {
        printf("jalr ");
        return 2;
    }
    else if (strcmp(result, "001000") == 0) {
        printf("jr ");
        return 3;
    }
    else if (strcmp(result, "010000") == 0) {
        printf("mfhi ");
        return 4;
    }
    else if (strcmp(result, "010010") == 0) {
        printf("mflo ");
        return 4;
    }
    else if (strcmp(result, "010001") == 0) {
        printf("mthi ");
        return 3;
    }
    else if (strcmp(result, "010011") == 0) {
        printf("mtlo ");
        return 3;
    }
    else if (strcmp(result, "011000") == 0) {
        printf("mult ");
        return 1;
    }
    else if (strcmp(result, "011001") == 0) {
        printf("multu ");
        return 1;
    }
    else if (strcmp(result, "100111") == 0) {
        printf("nor ");
        return 0;
    }
    else if (strcmp(result, "100101") == 0) {
        //
        printf("or ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] | Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "000000") == 0) {
        //
        printf("sll ");
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] << R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000100") == 0) {
        printf("sllv ");
        return 6;
    }
    else if (strcmp(result, "101010") == 0) {
        ////////
        printf("slt ");
        if (Registers[R_rs(inst)] < Registers[R_rt(inst)]) {
            Registers[R_rd(inst)] = 1;
        } else {
            Registers[R_rd(inst)] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "101011") == 0) {
        printf("sltu ");
        return 0;
    }
    else if (strcmp(result, "000011") == 0) {
        printf("sra ");
        return 5;
    }
    else if (strcmp(result, "000111") == 0) {
        printf("srav ");
        return 6;
    }
    else if (strcmp(result, "000010") == 0) {
        //
        printf("srl ");
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] >> R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000110") == 0) {
        printf("srlv ");
        return 6;
    }
    else if (strcmp(result, "100010") == 0) {
        //
        printf("sub ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] - Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "100011") == 0) {
        printf("subu ");
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        printf("syscall ");
        return 7;
    }
    else if (strcmp(result, "100110") == 0) {
        printf("xor ");
        return 0;
    }
    return 8;
}
int I_opcodeCheck(char inst[], unsigned int Registers[], unsigned char DataMem[][4], int* num) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "001000") == 0) {
        // 
        printf("addi ");
        Registers[I_rt(inst)] = Registers[I_rs(inst)] + I_im(inst);
        return 0;
    }
    else if (strcmp(result, "001001") == 0) {
        printf("addiu ");
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        //
        printf("andi ");
        Registers[I_rt(inst)] = (Registers[I_rs(inst)] & I_im(inst));
        return 0;
    }
    else if (strcmp(result, "000100") == 0) {
        //
        printf("beq ");
        return 1;
    }
    else if (strcmp(result, "000101") == 0) {
        //if(R[$rs] != R[$rt]) PC ← PC + 4 + SignExt18b({imm, 00})
        printf("bne ");
        if (Registers[I_rs(inst)] != Registers[I_rt(inst)]) {
            *num+=I_im(inst);
        }
        return 1;
    }
    else if (strcmp(result, "100000") == 0) {
        //
        printf("lb ");
        return 2;
    }
    else if (strcmp(result, "100100") == 0) {
        //
        printf("lbu ");
        return 2;
    }
    else if (strcmp(result, "100001") == 0) {
        //
        printf("lh ");
        return 2;
    }
    else if (strcmp(result, "100101") == 0) {
        //
        printf("lhu ");
        return 2;
    }
    else if (strcmp(result, "001111") == 0) {
        //lui load upper immediate/ lui $s1, 100 $s1=100*216 $s1의 상위 16비트에 constant 100을 로드
        printf("lui ");
        Registers[I_rt(inst)] = (I_im(inst) << 16);
        return 3;
    }
    else if (strcmp(result, "100011") == 0) {
        //lw load word/ lw $s1, 100($s2)
        //$s1=memory[$s2 + 100] : $s1에 메모리 베이스 주소 $s2에 100을 더한 $s2+100의 주소에 있는 값을 저장
        printf("lw ");
        printf("\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (Registers[I_rs(inst)] >> j) & 1);
        }
        printf("\n");
        unsigned int temp = 0;
        for (int i = 31; i >= 24 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][0];
        }
        temp <<= 8;
        printf("temp\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (temp >> j) & 1);
        }
        printf("\n");
        for (int i = 23; i >= 16 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][1];
        }
        temp <<= 8;
        printf("temp\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (temp >> j) & 1);
        }
        printf("\n");
        for (int i = 15; i >= 8 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][2];
        }
        temp <<= 8;
        printf("temp\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (temp >> j) & 1);
        }
        printf("\n");
        for (int i = 7; i >= 0 ; i--) {
            temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][3];
        }
        printf("temp\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (temp >> j) & 1);
        }
        printf("\n");
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "001101") == 0) {
        //
        printf("ori ");
        Registers[I_rt(inst)] = Registers[I_rs(inst)] | I_im(inst);
        return 0;
    }
    else if (strcmp(result, "101000") == 0) {
        //
        printf("sb ");
        return 2;
    }
    else if (strcmp(result, "001010") == 0) {
        //
        printf("slti ");
        if (Registers[I_rs(inst)] < I_im(inst)) {
            Registers[I_rt(inst)] = 1;
        } else {
            Registers[I_rt(inst)] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "001011") == 0) {
        printf("sltiu ");
        return 0;
    }
    else if (strcmp(result, "101001") == 0) {
        //
        printf("sh ");
        return 2;
    }
    else if (strcmp(result, "101011") == 0) {
        //
        printf("sw ");
        printf("\nrt : \n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (Registers[I_rt(inst)] >> j) & 1);
        }
        for (int i = 31; i >= 24 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i-24);
            DataMem[Registers[I_rs(inst)]+I_im(inst)/4-(0x10000000)][0] = ch;
        }
        for (int i = 23; i >= 16 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i-16);
            DataMem[Registers[I_rs(inst)]+I_im(inst)/4-(0x10000000)][1] = ch;
        }
        for (int i = 15; i >= 8 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i-8);
            DataMem[Registers[I_rs(inst)]+I_im(inst)/4-(0x10000000)][2] = ch;
        }
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i);
            DataMem[Registers[I_rs(inst)]+I_im(inst)/4-(0x10000000)][3] = ch;
        }
        printf("\nRegister[rs]\n");
        for (int j = 31; j >= 0; j--) {
            printf("%d", (Registers[I_rs(inst)] >> j) & 1);
        }
        Mem(DataMem, 0x10000000);
        return 2;
    }
    else if (strcmp(result, "001110") == 0) {
        printf("wxori ");
        return 0;
    }
    else if (strcmp(result, "000010") == 0) {
        //
        printf("j ");
        return 4;
    }
    else if (strcmp(result, "000011") == 0) {
        printf("jal ");   
        return 4;
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
int J_opcodeCheck(char inst[]) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "000010") == 0) {
        printf("j ");
        return 0;
    }
    else {
        printf("jal ");
        return 1;
    }
}
int J_target(char inst[]) {
    char result[26] = "";
    substring(inst, result, 6, 26);
    int target = 0;
    for (int i = 25; i >= 0; i--) {
        if (result[i]) {
            if (i == 0) {
                target -= (result[i]-48) << (25-i);
            } else {
                target += (result[i]-48) << (25-i);
            }
        }
    }
    return target;
}
void Reg(unsigned int Registers[], int num) {
    for (int i = 0; i < 32; i++) {
        printf("$%d: %08x\n", i, Registers[i]);
    }
    printf("PC: %08x\n", num*4+4);
}

void Mem(unsigned char DataMem[][4], int startAddress) {
    printf("\nin Mem\n");
    for (int k = 0; k < 4; k++) {
        printf("0x");
        for (int i = 0; i < 4; i++) {
            printf("%02x", (DataMem[(startAddress-0x10000000)/4+k][i]));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    // printf("%s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]);
    int N = atoi(argv[2]);
    printf("N : %d\n", N);
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
        for (int i = 0; i < 4; i++) {
            for (int j = 3; j >= 0; j--) {
                printf("%d", (InstMem[x][i]>>j) & 1);
            }
        }
        printf("\n");
        x++;
    }
    printf("\n");
    printf("InstMem\n");
    // for (int k = 0; k < 65536/4; k++) {
    for (int k = 0; k < 30; k++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 3; j >= 0; j--) {
                printf("%d", (InstMem[k][i]>>j) & 1);
            }
        }
        printf("\n");
    }

    printf("\n");
    printf("DataMem\n");
    for (int k = 0; k < 30; k++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 3; j >= 0; j--) {
                printf("%d", (DataMem[k][i]>>j) & 1);
            }
        }
        printf("\n");
    }
    int num = 0;
    while(1) {
        printf("inst %d: ", num);
        for (int i = 0; i < 4; i++) {
            for (int j = 7; j >= 0; j--) {
                printf("%d", (InstMem[num][i]>>j) & 1);
            }
        }

        idx = 0;
        for (int i = 0; i < 4; i++) {
            // printf(" ");
            for (int j = 7; j >= 0; j--) {
                // printf("%d", InstMem[num*4+i] >> j & 1);
                if ((InstMem[num][i] >> j) & 1) {
                    inst[idx] = '1';
                } else {
                    inst[idx] = '0';
                }
                idx++;
            }
        }
        printf(" ");
        int opcode = typeCheck(inst);
        if (opcode == 0) { //R-type
            switch(R_funcCheck(inst, Registers)) {
            case 0:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rs(inst)); //rs
                printf("$%d\n", R_rt(inst)); //rt
                Reg(Registers, 0);
                break;
            case 1:
                printf("$%d, ", R_rs(inst)); //rs
                printf("$%d\n", R_rt(inst)); //r
                Reg(Registers, 0);
                break;
            case 2:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d\n", R_rs(inst)); //rs
                Reg(Registers, 0);
                break;
            case 3:
                printf("$%d\n", R_rs(inst)); //rs
                Reg(Registers, 0);
                break;
            case 4:
                printf("$%d\n", R_rd(inst)); //rd
                Reg(Registers, 0);
                break;
            case 5:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rt(inst)); //rt
                printf("%d\n", R_sa(inst)); //sa
                Reg(Registers, 0);
                break;

            case 6:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rt(inst)); //rt
                printf("$%d\n", R_rs(inst)); //rs
                Reg(Registers, 0);
                break;
            case 7:
                printf("\n");
                break;
            default:
                printf("unknown instruction\n");
                num--;
                if (!strcmp(argv[3], "reg")) {
                    Reg(Registers, num);
                }
                if (!strcmp(argv[3], "mem")) {
                    Mem(DataMem, atoi(argv[4]));
                }
            }
        } else if (opcode == 1) { //I-type
            switch (I_opcodeCheck(inst, Registers, DataMem, &num)) {
            case 0:
                printf("$%d, ", I_rt(inst)); //rt
                printf("$%d, ", I_rs(inst)); //rs
                printf("%d\n", I_im(inst)); //immediate
                Reg(Registers, 0);
                break;
            case 1:
                printf("$%d, ", I_rs(inst)); //rs
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d\n", I_im(inst)); //immediate
                Reg(Registers, 0);
                break;
            case 2:
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d", I_im(inst)); //immediate
                printf("($%d)\n", I_rs(inst)); //rs
                Reg(Registers, 0);
                break;
            case 3:
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d\n" , I_im(inst)); //immediate
                Reg(Registers, 0);
                break;
            default:
                printf("unknown instruction\n");
                num--;
                if (!strcmp(argv[3], "reg")) {
                    Reg(Registers, num);
                }
                if (!strcmp(argv[3], "mem")) {
                    Mem(DataMem, atoi(argv[4]));
                }
                return 0;
            }
        } else if (opcode == 2) { //J-type
            J_opcodeCheck(inst);
            printf("%d\n", J_target(inst)); //target 26bit
        }
        num++;
    }
    fclose(f_input); // 파일 닫기
    return 0;
}