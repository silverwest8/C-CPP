#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int R_funcCheck(char inst[]) {
    char result[6] = "";
    substring(inst, result, 26, 6);
    if (strcmp(result, "100000") == 0) {
        printf("add ");
        return 0;
    }
    else if (strcmp(result, "100001") == 0) {
        printf("addu ");
        return 0;
    }
    else if (strcmp(result, "100100") == 0) {
        printf("and ");
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
        printf("or ");
        return 0;
    }
    else if (strcmp(result, "000000") == 0) {
        printf("sll ");
        return 5;
    }
    else if (strcmp(result, "000100") == 0) {
        printf("sllv ");
        return 6;
    }
    else if (strcmp(result, "101010") == 0) {
        printf("slt ");
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
        printf("srl ");
        return 5;
    }
    else if (strcmp(result, "000110") == 0) {
        printf("srlv ");
        return 6;
    }
    else if (strcmp(result, "100010") == 0) {
        printf("sub ");
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
int I_opcodeCheck(char inst[]) {
    char result[6] = "";
    substring(inst, result, 0, 6);
    if (strcmp(result, "001000") == 0) {
        printf("addi ");
        return 0;
    }
    else if (strcmp(result, "001001") == 0) {
        printf("addiu ");
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        printf("andi ");
        return 0;
    }
    else if (strcmp(result, "000100") == 0) {
        printf("beq ");
        return 1;
    }
    else if (strcmp(result, "000101") == 0) {
        printf("bne ");
        return 1;
    }
    else if (strcmp(result, "100000") == 0) {
        printf("lb ");
        return 2;
    }
    else if (strcmp(result, "100100") == 0) {
        printf("lbu ");
        return 2;
    }
    else if (strcmp(result, "100001") == 0) {
        printf("lh ");
        return 2;
    }
    else if (strcmp(result, "100101") == 0) {
        printf("lhu ");
        return 2;
    }
    else if (strcmp(result, "001111") == 0) {
        printf("lui ");
        return 3;
    }
    else if (strcmp(result, "100011") == 0) {
        printf("lw ");
        return 2;
    }
    else if (strcmp(result, "001101") == 0) {
        printf("ori ");
        return 0;
    }
    else if (strcmp(result, "101000") == 0) {
        printf("sb ");
        return 2;
    }
    else if (strcmp(result, "001010") == 0) {
        printf("slti ");
        return 0;
    }
    else if (strcmp(result, "001011") == 0) {
        printf("sltiu ");
        return 0;
    }
    else if (strcmp(result, "101001") == 0) {
        printf("sh ");
        return 2;
    }
    else if (strcmp(result, "101011") == 0) {
        printf("sw ");
        return 2;
    }
    else if (strcmp(result, "001110") == 0) {
        printf("wxori ");
        return 0;
    }
    else if (strcmp(result, "000010") == 0) {
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
int main(int argc, char *argv[]) {
    int arr[4];
    char inst[32];
    int idx;
    // test1.bin 이라는 이진파일을 읽기 전용으로 열기
    FILE *f_input = fopen(argv[1], "rb");
    int num = 0;
    while(1) {
        idx = 0;
        for (int i = 0; i < 4; i++) { //!= EOF
            arr[i] = fgetc(f_input);
            if (arr[i] == -1) {
                return 0;
            }
        }
        printf("inst %d: ", num++);
        for(int i = 0; i < 4; i++){;
            printf("%02x", arr[i]);
        }
        for (int i = 0; i < 4; i++) {
            // printf(" ");
            for (int j = 7; j >= 0; j--) {
                // printf("%d", arr[i] >> j & 1);
                if (arr[i] >> j & 1) {
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
            switch(R_funcCheck(inst)) {
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
                printf("unknown instruction\n");
            }
        } else if (opcode == 1) { //I-type
            switch (I_opcodeCheck(inst)) {
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
                printf("unknown instruction\n");
                break;
            }
        } else if (opcode == 2) { //J-type
            J_opcodeCheck(inst);
            printf("%d\n", J_target(inst)); //target 26bit
        }
    }
    fclose(f_input); // 파일 닫기
    return 0;
}