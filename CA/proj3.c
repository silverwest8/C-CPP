#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int Registers[32];
unsigned char InstMem[65536/4][4];
unsigned char DataMem[65536/4][4];
unsigned char LRU[8];
unsigned char Valid[16];
unsigned char Dirty[16]; 
unsigned char TagInCache[16][23]; //8 * 128byte // 1024/16 = 64 block
unsigned char CacheMem[16][64];
char inst[32];
int hit_num = 0;
int miss_num = 0;
// 16bit tag 32 - 9    --> 23 bit offset
// index size : 8      --> 3 bit offset
// bolck size : 64byte --> 6 bit offset 1w 4b 2 / 2w 8b 3 /4w 16b 4/ 8w 32b 5 / 16w 64b 6 / 32w 128 71
void substring(char result[], int start, int len);
int typeCheck();
int R_funcCheck();
int I_opcodeCheck(int* inst_num);
int R_rd();
int R_rs();
int R_rt();
int R_sa();
int I_rs();
int I_rt();
int I_im();
int I_im_zeroEx();
int I_label();
int J_opcodeCheck(int* inst_num);
int J_target();
void RegPrint(int inst_num);
void MemPrint(int startAddress);

void Caching();
void PrintAddress(int address);
void Miss(int index, int address);
int getIndex(int address);
int getOffset(int address);
int TagComparator(int address, int id);

void Caching() {
    int address = Registers[I_rs()]+I_im();
    int index = getIndex(address);
    printf("Index is %d\n", index);
    //cache hit인지 확인
    if ( (Valid[index*2] == '1') || Valid[index*2+1] == '1') {
        //tag가 일치하는지 확인
        if (TagComparator(address, 0) == 1 || TagComparator(address, 1) == 1) {
            hit_num++;
            printf("hit_num : %d\n", hit_num);
            PrintAddress(address);
            return;
        }
    }
    Miss(index, address);
    PrintAddress(address);
}
void PrintAddress(int address) {
    for (int i = 31; i >= 0; i--) {
        if (i == 31) { printf("tag : "); }
        else if (i == 8) { printf("  index : "); }
        else if (i == 5) { printf("  offset : "); }
        printf("%d", (address >> i) & 1);
    }
}
void Miss(int index, int address) {
    miss_num++;
    printf("miss_num : %d\n", miss_num);
    if (LRU[index] == '0') {
        if (Valid[index*2] == '0') { // invalid 한 경우
            Valid[index*2] = '1'; // valid set
            Dirty[index*2] = '0';
        }
        for (int i = 31; i >= 9; i--) { // fetched to cache(TagInCache)
            if ((address >> i) & 1) {
                TagInCache[index*2][31-i] = '1'; 
            } else {
                TagInCache[index*2][31-i] = '0';
            }
        }
        LRU[index] = '1'; // LRU change
        // if (Dirty...)
    }
    else if (LRU[index] == '1') {
        if (Valid[index*2+1] == '0') { // invalid 한 경우
            Valid[index*2+1] = '1'; // valid set
            Dirty[index*2+1] = '0';
        }
        for (int i = 31; i >= 9; i--) {
            if ((address >> i) & 1) {
                TagInCache[index*2+1][31-i] = '1';
            } else {
                TagInCache[index*2+1][31-i] = '0';
            }
        }
        LRU[index] = '1'; // LRU change
    }
}
int getIndex(int address) {
    int index = 0;
    for (int i = 8; i >= 6; i--) {
        if ((address >> i) & 1) {
            index += (1>>(i-6));
        }
    }
    return index;
}
int getOffset(int address) {
    int offset = 0;
    for (int i = 5; i >= 0; i--) {
        if ((address >> i) & 1) {
            offset += (1>>i);
        }
    }
    printf("Offset is %d\n", offset);
    return offset;
}
int TagComparator(int address, int id) {
    if (id == 0) {
        for (int i = 31; i >= 9; i--) {
            if ( (TagInCache[getIndex(address)*2][31-i]-48) != ((address >> i) & 1)) {
                return -1;
            }
        }
        return 1;
    }
    else if (id == 1) {
        for (int i = 31; i >= 9; i--) {
            if ( (TagInCache[getIndex(address)*2+1][31-i]-48) != ((address >> i) & 1)) {
                return -1;
            }
        }
        return 1;
    }
    printf("exception\n");
    return 0;
}

void substring(char result[], int start, int len) {
    int i = 0;
    while (i < len) {
        result[i] = inst[start+i];
        i++;
    }
}
int typeCheck() {
    char result[6] = "";
    substring( result, 0, 6);
    if (strcmp(result, "000000") == 0) {
        return 0; //Rtype
    }
    else if (strcmp(result, "000010") == 0 || strcmp(result, "000011") == 0) {
        return 2; //Jtype
    }
    else {
        return 1; //Itype
    }
}
int R_funcCheck() {
    char result[6] = "";
    substring( result, 26, 6);
    if (strcmp(result, "100000") == 0) {
        //add
        printf("add ");
        Registers[R_rd()] = Registers[R_rs()] + Registers[R_rt()];
        return 0;
    }
    else if (strcmp(result, "100001") == 0) {
        //// addu
        printf("addu ");
        Registers[R_rd()] = Registers[R_rs()] + Registers[R_rt()];
        return 0;
    }
    else if (strcmp(result, "100100") == 0) {
        // and
        printf("and ");
        Registers[R_rd()] = Registers[R_rs()] & Registers[R_rt()];
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
        R_rs();
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
        Registers[R_rd()] = Registers[R_rs()] | Registers[R_rt()];
        return 0;
    }
    else if (strcmp(result, "000000") == 0) {
        // sll
        printf("sll ");
        Registers[R_rd()] = (Registers[R_rt()] << R_sa());
        return 5;
    }
    else if (strcmp(result, "000100") == 0) {
        return 6;
    }
    else if (strcmp(result, "101010") == 0) {
        //////// slt
        printf("slt ");
        if ((signed)Registers[R_rs()] < (signed)Registers[R_rt()]) {
            Registers[R_rd()] = 1;
        } else {
            Registers[R_rd()] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "101011") == 0) {
        //// sltu
        printf("sltu ");
        if ((signed)Registers[R_rs()] < (signed)Registers[R_rt()]) {
            Registers[R_rd()] = 1;
        } else {
            Registers[R_rd()] = 0;
        }
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
        Registers[R_rd()] = (Registers[R_rt()] >> R_sa());
        return 5;
    }
    else if (strcmp(result, "000110") == 0) {
        return 6;
    }
    else if (strcmp(result, "100010") == 0) {
        // sub
        printf("sub ");
        Registers[R_rd()] = Registers[R_rs()] - Registers[R_rt()];
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
int I_opcodeCheck(int* inst_num) {
    char result[6] = "";
    substring( result, 0, 6);
    if (strcmp(result, "001000") == 0) {
        // addi
        printf("addi ");
        Registers[I_rt()] = Registers[I_rs()] + I_im();
        return 0;
    }
    else if (strcmp(result, "001001") == 0) {
        ///// addiu
        printf("addiu ");
        Registers[I_rt()] = Registers[I_rs()] + I_im();
        return 0;
    }
    else if (strcmp(result, "001100") == 0) {
        // andi
        printf("andi ");
        Registers[I_rt()] = (Registers[I_rs()] & I_im_zeroEx());
        return 0;
    }
    else if (strcmp(result, "000100") == 0) {
        // beq
        printf("beq ");
        if (Registers[I_rs()] == Registers[I_rt()]) {
            *inst_num+=(I_im());
        }
        return 1;
    }
    else if (strcmp(result, "000101") == 0) {
        //bne
        //if(R[$rs] != R[$rt]) PC ← PC + 4 + SignExt18b({imm, 00})
        printf("bne ");
        if (Registers[I_rs()] != Registers[I_rt()]) {
            *inst_num+=(I_im());
        }
        return 1;
    }
    else if (strcmp(result, "100000") == 0) {
        // lb
        // R[$rt] ← SignExt8b(Mem1B(R[$rs] + SignExt16b(imm)))
        printf("lb ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][I_im() % 4];
        if ((temp >> 7) & 1) {
            temp |= 0xffffff00;
        }
        Registers[I_rt()] = temp;
        ////******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "100100") == 0) {
        // lbu
        printf("lbu ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs()]-(0x10000000)+I_im())/4][I_im_zeroEx() % 4];
        Registers[I_rt()] = temp;
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "100001") == 0) {
        // lh
        printf("lh ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs()]-(0x10000000)+I_im())/4][I_im()%4 < 0 ? I_im()%4+4 : I_im()%4];
        temp <<= 8;
        temp |= DataMem[(Registers[I_rs()]-(0x10000000)+I_im())/4][I_im()%4 < 0 ? I_im()%4+5 : I_im()%4+1];
        if ((temp >> 15) & 1) {
            temp |= 0xffff0000;
        }
        Registers[I_rt()] = temp;
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "100101") == 0) {
        // lhu
        printf("lhu ");
        unsigned int temp = 0;
        temp |= DataMem[(Registers[I_rs()]-(0x10000000)+I_im())/4][I_im()%4];
        temp <<= 8;
        temp |= DataMem[(Registers[I_rs()]-(0x10000000)+I_im())/4][I_im()%4+1];
        Registers[I_rt()] = temp;
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "001111") == 0) {
        // lui
        //lui load upper immediate/ lui $s1, 100 $s1=100*216 $s1의 상위 16비트에 constant 100을 로드
        printf("lui ");
        Registers[I_rt()] = (I_im() << 16);
        return 3;
    }
    else if (strcmp(result, "100011") == 0) {
        //lw load word/ lw $s1, 100($s2)
        //$s1=memory[$s2 + 100] : $s1에 메모리 베이스 주소 $s2에 100을 더한 $s2+100의 주소에 있는 값을 저장
        printf("lw ");
        unsigned int temp = 0;
        for (int i = 31; i >= 24 ; i--) {
            temp |= DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][0];
        }
        temp <<= 8;
        for (int i = 23; i >= 16 ; i--) {
            temp |= DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][1];
        }
        temp <<= 8;
        for (int i = 15; i >= 8 ; i--) {
            temp |= DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][2];
        }
        temp <<= 8;
        for (int i = 7; i >= 0 ; i--) {
            temp |= DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][3];
        }
        Registers[I_rt()] = temp;
        
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "001101") == 0) {
        // ori
        printf("ori ");
        Registers[I_rt()] = Registers[I_rs()] | I_im_zeroEx();
        return 0;
    }
    else if (strcmp(result, "101000") == 0) {
        // sb
        printf("sb ");
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt()] >> i) << (i);
            DataMem[Registers[I_rs()]/4+I_im()/4-(0x10000000)/4][I_im()%4] = ch;
        }
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "001010") == 0) {
        // slti
        printf("slti ");
        if ((signed)Registers[I_rs()] < I_im()) {
            Registers[I_rt()] = 1;
        } else {
            Registers[I_rt()] = 0;
        }
        return 0;
    }
    else if (strcmp(result, "001011") == 0) {
        return 0;
    }
    else if (strcmp(result, "101001") == 0) {
        // sh
        printf("sh ");
        for (int i = 15; i >= 8 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt()] >> i) << (i-8);
            DataMem[Registers[I_rs()]/4+I_im()/4-(0x10000000)/4][I_im()%4+1] = ch;
        }
        for (int i = 7; i >= 0 ; i--) {
            char ch = 0;
            ch |= (Registers[I_rt()] >> i) << (i);
            DataMem[Registers[I_rs()]/4+I_im()/4-(0x10000000)/4][I_im()%4] = ch;
        }
        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "101011") == 0) {
        // sw
        printf("sw ");
        char ch = 0;
        for (int i = 31; i >= 24 ; i--) {
            ch |= (Registers[I_rt()] >> i) << (i-24);
        }
        DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][0] = ch;
        ch = 0;
        for (int i = 23; i >= 16 ; i--) {
            ch |= (Registers[I_rt()] >> i) << (i-16);
        }
        DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][1] = ch;
        ch = 0;
        for (int i = 15; i >= 8 ; i--) {
            ch |= (Registers[I_rt()] >> i) << (i-8);
        }
        DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][2] = ch;
        ch = 0;
        for (int i = 7; i >= 0 ; i--) {
            ch |= (Registers[I_rt()] >> i) << (i);
        }
        DataMem[(Registers[I_rs()]+I_im()-(0x10000000))/4][3] = ch;

        //******************************[ cache ]******************************//
        Caching();
        return 2;
    }
    else if (strcmp(result, "001110") == 0) {
        return 0;
    }
    return 5;
}
int R_rd() {
    char result[5] = "";
    substring( result, 16, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_rs() {
    char result[5] = "";
    substring( result, 6, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_rt() {
    char result[5] = "";
    substring( result, 11, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int R_sa() {
    char result[5] = "";
    substring( result, 21, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_rs() {
    char result[5] = "";
    substring( result, 6, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_rt() {
    char result[5] = "";
    substring( result, 11, 5);
    int rgst = 0;
    for (int i = 4; i >= 0; i--) {
        if (result[i]) {
            rgst += (result[i]-48) << (4-i);
        }
    }
    return rgst;
}
int I_im() {
    char result[16] = "";
    substring( result, 16, 16);
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
int I_im_zeroEx() {
    char result[16] = "";
    substring( result, 16, 16);
    int im = 0;
    for (int i = 15; i >= 0; i--) {
        if (result[i]) {
            im += (result[i]-48) << (15-i);

        }
    }
    return im;
}
int I_label() {
    char result[16] = "";
    substring( result, 16, 16);
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
int J_opcodeCheck(int* num) {
    char result[6] = "";
    substring( result, 0, 6);
    if (strcmp(result, "000010") == 0) {
        //PC(num)의 첫 4bit + target 26bit를 shift left 2 한거
        printf("j ");
        unsigned int target = 0;
        target = J_target() << 2;
        int origin = *num*4;
        int result = 0;
        origin >>= 28;
        origin <<= 28;
        result = origin | target;
        *num = result/4;
        (*num)--;
        return 0;
    }
    else if (strcmp(result, "000011") == 0) {
        //// jal
        printf("jal ");
        unsigned int target = 0;
        target = J_target() << 2;
        int origin = *num*4;
        int result = 0;
        origin >>= 28;
        origin <<= 28;
        result = origin | target;
        *num = result/4;
        (*num)--;
        Registers[31] = result;
        // Same way as j, and stores PC+4 to the ra register($31)
        return 1;
    }
    return 0;
}
int J_target() {
    char result[26] = "";
    substring( result, 6, 26);
    int target = 0;
    for (int i = 25; i >= 0; i--) {
        if (result[i]) {
            target += (result[i]-48) << (25-i);
        }
    }
    return target;
}
void RegPrint(int inst_num) {
    for (int i = 0; i < 32; i++) {
        printf("$%d: 0x%08x\n", i, Registers[i]);
    }
    printf("PC: 0x%08x\n", inst_num*4);
}

void MemPrint(int startAddress) {
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

    memset(Registers, 0x00000000, sizeof(Registers));
    memset(InstMem, 0xff, sizeof(InstMem));
    memset(DataMem, 0xff, sizeof(DataMem));
    memset(LRU, 0x30, sizeof(LRU));
    memset(Valid, 0x30, sizeof(Valid));
    memset(TagInCache, 0x30, sizeof(TagInCache));

    int idx;
    FILE *f_input = fopen(argv[1], "rb");

    int temp = 0;
    while(1) {
        int result = fread(InstMem[temp], 1, 4, f_input);
        if (result != 4) {
            break;
        }
        temp++;
    }
    
    int num = 0;
    int inst_num = 0;
    int unknown = 0;

    while(inst_num <= N && unknown == 0) {
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
        printf("\n");
        printf("inst %d: ", inst_num);

        int opcode = typeCheck();
        if (opcode == 0) { //R-type
            switch(R_funcCheck()) {
            case 0:
                printf("$%d, ", R_rd()); //rd
                printf("$%d, ", R_rs()); //rs
                printf("$%d\n", R_rt()); //rt
                break;
            case 1:
                printf("$%d, ", R_rs()); //rs
                printf("$%d\n", R_rt()); //r
                break;
            case 2:
                printf("$%d, ", R_rt()); //rt
                printf("$%d\n", R_rs()); //rs
                break;
            case 3:
                printf("$%d\n", R_rs()); //rs
                break;
            case 4:
                printf("$%d\n", R_rd()); //rd
                break;
            case 5:
                printf("$%d, ", R_rd()); //rd
                printf("$%d, ", R_rt()); //rt
                printf("%d\n", R_sa()); //sa
                break;
            case 6:
                printf("$%d, ", R_rd()); //rd
                printf("$%d, ", R_rt()); //rt
                printf("$%d\n", R_rs()); //rs
                break;
            case 7:
                printf("\n");
                break;
            default:
                unknown = 1;
            }
        }
        else if (opcode == 1) { //I-type
            switch (I_opcodeCheck(&num)) {
            case 0:
                printf("$%d, ", I_rt()); //rt
                printf("$%d, ", I_rs()); //rs
                printf("%d\n", I_im()); //immediate
                break;
            case 1:
                printf("$%d, ", I_rs()); //rs
                printf("$%d, ", I_rt()); //rt
                printf("%d\n", I_im()); //immediate
                break;
            case 2:
                printf("$%d, ", I_rt()); //rt
                printf("%d", I_im()); //immediate
                printf("($%d)\n", I_rs()); //rs
                break;
            case 3:
                printf("$%d, ", I_rt()); //rt
                printf("%d\n" , I_im()); //immediate
                break;
            default:
                unknown = 1;
            }
        }
        else if (opcode == 2) { //J-type
            J_opcodeCheck(&num);
            printf("%d\n", J_target()); //target 26bit
        }
        num++;
        inst_num++;
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
            RegPrint(inst_num);
        }
        if (!strcmp(argv[3], "mem")) {
            MemPrint(strtol(argv[4], NULL, 16));
        }
    }

    fclose(f_input); // 파일 닫기
    return 0;
}