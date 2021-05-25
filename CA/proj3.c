#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void substring(char inst[], char result[], int start, int len);
int typeCheck(char inst[]);
int R_funcCheck(char inst[], unsigned int Registers[]);
int I_opcodeCheck(char inst[], unsigned int Registers[], unsigned char DataMem[][4], unsigned char InstMem[][4], int* inst_num);
int R_rd(char inst[]);
int R_rs(char inst[]);
int R_rt(char inst[]);
int R_sa(char inst[]);
int I_rs(char inst[]);
int I_rt(char inst[]);
int I_im(char inst[]);
int I_im_zeroEx(char inst[]);
int I_label(char inst[]);
int J_opcodeCheck(char inst[], int* inst_num);
int J_target(char inst[]);
void RegPrint(unsigned int Registers[], int inst_num);
void MemPrint(unsigned char DataMem[][4], int startAddress);

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
        //add
        printf("add ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] + Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "100001") == 0) {
        //// addu
        return 0;
    }
    else if (strcmp(result, "100100") == 0) {
        //******************************[ cache ] ******************************//
        //lbu
        printf("lbu ");
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
        //// jr
        
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
        // or
        printf("or ");
        Registers[R_rd(inst)] = Registers[R_rs(inst)] | Registers[R_rt(inst)];
        return 0;
    }
    else if (strcmp(result, "000000") == 0) {
        // sll
        printf("sll ");
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] << R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000100") == 0) {
        return 6;
    }
    else if (strcmp(result, "101010") == 0) {
        //////// slt
        printf("slt ");
        if ((signed)Registers[R_rs(inst)] < (signed)Registers[R_rt(inst)]) {
            Registers[R_rd(inst)] = 1;
        } else {
            Registers[R_rd(inst)] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "101011") == 0) {
        //// sltu
        return 0;
    }
    else if (strcmp(result, "000011") == 0) {
        return 5;
    }
    else if (strcmp(result, "000111") == 0) {
        return 6;
    }
    else if (strcmp(result, "000010") == 0) {
        // srl
        printf("srl ");
        Registers[R_rd(inst)] = (Registers[R_rt(inst)] >> R_sa(inst));
        return 5;
    }
    else if (strcmp(result, "000110") == 0) {
        return 6;
    }
    else if (strcmp(result, "100010") == 0) {
        // sub
        printf("sub ");
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
int I_opcodeCheck(char inst[], unsigned int Registers[], unsigned char DataMem[][4], unsigned char InstMem[][4], int* inst_num) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "001000") == 0) {
        // addi
        printf("addi ");
        Registers[I_rt(inst)] = Registers[I_rs(inst)] + I_im(inst);
        return 0;
    }
    else if (strcmp(result, "001001") == 0) {
        ///// addiu
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        // andi
        printf("andi ");
        Registers[I_rt(inst)] = (Registers[I_rs(inst)] & I_im_zeroEx(inst));
        return 0;
    }
    else if (strcmp(result, "000100") == 0) {
        // beq
        printf("beq ");
        if (Registers[I_rs(inst)] == Registers[I_rt(inst)]) {
            *inst_num+=(I_im(inst));
        }
        return 1;
    }
    else if (strcmp(result, "000101") == 0) {
        //bne
        //if(R[$rs] != R[$rt]) PC ← PC + 4 + SignExt18b({imm, 00})
        printf("bne ");
        if (Registers[I_rs(inst)] != Registers[I_rt(inst)]) {
            *inst_num+=(I_im(inst));
        }
        return 1;
    }
    else if (strcmp(result, "100000") == 0) {
        // bne
        // R[$rt] ← SignExt8b(Mem1B(R[$rs] + SignExt16b(imm)))
        printf("lb ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst) % 4];
        if ((temp >> 7) & 1) {
            temp |= 0xffffff00;
        }
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "100100") == 0) {
        // lbu
        printf("lbu ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im_zeroEx(inst) % 4];
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "100001") == 0) {
        ////******************************[ cache ] ******************************//
        // lh
        printf("lh ");
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
        // lhu
        printf("lhu ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4];
        temp <<= 8;
        temp |= DataMem[(Registers[I_rs(inst)]-(0x10000000)+I_im(inst))/4][I_im(inst)%4+1];
        Registers[I_rt(inst)] = temp;
        return 2;
    }
    else if (strcmp(result, "001111") == 0) {
        // lui
        //lui load upper immediate/ lui $s1, 100 $s1=100*216 $s1의 상위 16비트에 constant 100을 로드
        printf("lui ");
        Registers[I_rt(inst)] = (I_im(inst) << 16);
        return 3;
    }
    else if (strcmp(result, "100011") == 0) {
        //******************************[ cache ] ******************************//
        //lw load word/ lw $s1, 100($s2)
        //$s1=memory[$s2 + 100] : $s1에 메모리 베이스 주소 $s2에 100을 더한 $s2+100의 주소에 있는 값을 저장
        printf("lw ");
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
        // ori
        printf("ori ");
        Registers[I_rt(inst)] = Registers[I_rs(inst)] | I_im_zeroEx(inst);
        return 0;
    }
    else if (strcmp(result, "101000") == 0) {
        //******************************[ cache ] ******************************//
        // sb
        printf("sb ");
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt(inst)] >> i) << (i);
            DataMem[Registers[I_rs(inst)]/4+I_im(inst)/4-(0x10000000)/4][I_im(inst)%4] = ch;
        }
        return 2;
    }
    else if (strcmp(result, "001010") == 0) {
        // slti
        printf("slti ");
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
        //******************************[ cache ] ******************************//
        // sh
        printf("sh ");
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
        //******************************[ cache ] ******************************//
        // sw
        printf("sw ");
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
int J_opcodeCheck(char inst[], int* inst_num) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "000010") == 0) {
        //PC(inst_num)의 첫 4bit + target 26bit를 shift left 2 한거
        printf("j ");
        unsigned int target = 0;
        target = J_target(inst) << 2;
        int origin = *inst_num*4;
        int result = 0;
        origin >>= 28;
        origin <<= 28;
        result = origin | target;
        *inst_num = result/4;
        (*inst_num)--;
        return 0;
    }
    if (strcmp(result, "000011") == 0) {
        ////
        printf("jal ");
        unsigned int target = 0;
        target = J_target(inst) << 2;
        int origin = *inst_num*4;
        int result = 0;
        origin >>= 28;
        origin <<= 28;
        result = origin | target;
        *inst_num = result/4;
        (*inst_num)--;

        //jal (opcode: 000011) ➢ Works in the same way as j, and it also stores PC+4 to the ra register (register $31)
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
void RegPrint(unsigned int Registers[], int inst_num) {
    for (int i = 0; i < 32; i++) {
        printf("$%d: 0x%08x\n", i, Registers[i]);
    }
    printf("PC: 0x%08x\n", inst_num*4);
}

void MemPrint(unsigned char DataMem[][4], int startAddress) {
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
    unsigned char CacheMem[1024/(64*2)][64*2]; //8 * 128byte // 1024/16 = 64 block
    //16bit tag, 10bit index, 6bit offset

    memset(Registers, 0x00000000, sizeof(Registers));
    memset(InstMem, 0xff, sizeof(InstMem));
    memset(DataMem, 0xff, sizeof(DataMem));

    char inst[32];
    FILE *f_input = fopen(argv[1], "rb");

    int temp = 0;
    while(1) {
        int result = fread(InstMem[temp], 1, 4, f_input);
        if (result != 4) {
            break;
        }
        temp++;
    }
    
    int idx;
    int inst_num = 0;
    int hit_num = 0;
    int miss_num = 0;
    int unknown = 0;

    while(inst_num <= N && unknown == 0) {
        idx = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 7; j >= 0; j--) {
                if ((InstMem[inst_num][i] >> j) & 1) {
                    inst[idx] = '1';
                } else {
                    inst[idx] = '0';
                }
                idx++;
            }
        }
        printf("\n");
        printf("inst %d: ", inst_num++);
        for (int i = 0; i < 32; i++) {
            if (i == 0) { printf("tag : "); }
            else if (i == 16) { printf("  index : "); }
            else if (i == 27) { printf("  offset : "); }

            printf("%c", inst[i]);
        }
        printf("\n");
        int opcode = typeCheck(inst);
        if (opcode == 0) { //R-type
            switch(R_funcCheck(inst, Registers)) {
            case 0:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rs(inst)); //rs
                printf("$%d\n", R_rt(inst)); //rt
                break;
            case 1:
                printf("$%d, ", R_rs(inst)); //rs
                printf("$%d\n", R_rt(inst)); //r
                break;
            case 2:
                printf("$%d, ", R_rt(inst)); //rt
                printf("$%d\n", R_rs(inst)); //rs
                break;
            case 3:
                printf("$%d\n", R_rs(inst)); //rs
                break;
            case 4:
                printf("$%d\n", R_rd(inst)); //rd
                break;
            case 5:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rt(inst)); //rt
                printf("%d\n", R_sa(inst)); //sa
                break;
            case 6:
                printf("$%d, ", R_rd(inst)); //rd
                printf("$%d, ", R_rt(inst)); //rt
                printf("$%d\n", R_rs(inst)); //rs
                break;
            case 7:
                printf("\n");
                break;
            default:
                unknown = 1;
            }
        }
        else if (opcode == 1) { //I-type
            switch (I_opcodeCheck(inst, Registers, DataMem, InstMem, &inst_num)) {
            case 0:
                printf("$%d, ", I_rt(inst)); //rt
                printf("$%d, ", I_rs(inst)); //rs
                printf("%d\n", I_im(inst)); //immediate
                break;
            case 1:
                printf("$%d, ", I_rs(inst)); //rs
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d\n", I_im(inst)); //immediate
                break;
            case 2:
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d", I_im(inst)); //immediate
                printf("($%d)\n", I_rs(inst)); //rs
                break;
            case 3:
                printf("$%d, ", I_rt(inst)); //rt
                printf("%d\n" , I_im(inst)); //immediate
                break;
            default:
                unknown = 1;
            }
        } else if (opcode == 2) { //J-type
            J_opcodeCheck(inst, &inst_num);
            printf("%d\n", J_target(inst)); //target 26bit
        }
    }
    if (unknown == 1) {
        printf("unknown instruction\n");
    }
    printf("Instructions: %d\n", inst_num);
    printf("Hits: %d\n", hit_num);
    printf("Misses: %d\n", miss_num);
    printf("Total: %d\n", hit_num+miss_num);

    if (argv[3] != NULL) {
        if (!strcmp(argv[3], "reg")) {
            RegPrint(Registers, inst_num);
        }
        if (!strcmp(argv[3], "mem")) {
            MemPrint(DataMem, strtol(argv[4], NULL, 16));
        }
    }

    fclose(f_input); // 파일 닫기
    return 0;
}