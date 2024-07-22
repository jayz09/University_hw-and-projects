#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hexadecimal to Integer Converter Function
int hex2int(char *hex){
    int value;
    sscanf(hex, "%x",&value);
    return value;
}

// Calculation betweeen Registers or Hexadecimals or Register and Hexadecimal function
int calc(int R[10], char *operand){
    char reg = 'R', val = '0';
    char *token;

    if (reg == operand[0]){
        token = strtok(operand, "R");
        return R[atoi(token)];
    }
    else if (val == operand[0]){
        return hex2int(operand);
    }     
}

// Compare Function
int compare(int R[10], char *operand1, char *operand2){
    char *arg1, *arg2;

    arg1 = strtok(operand1, "R");
    arg2 = strtok(operand2, "R");

    if (R[atoi(arg1)] >= R[atoi(arg2)])
        return 0;
    else if (R[atoi(arg1)] < R[atoi(arg2)])
        return 1;
}

// GCD Function
int GCD(int R[10], char *operand1, char *operand2){
    int n1 = calc(R, operand1);
    int n2 = calc(R, operand2);

    while (n1 != n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    return n1;
}

// Get register index function
int tar_reg(char *operand){
    char *token;
    token = strtok(operand, "R");
    return atoi(token);
}

// Restore memory to the original function
void restore_mem(char mem[][32], char mem2[][32]){
    for (int x = 0; x < 20; x++)
        strcpy (mem[x], mem2[x]);
}

int main() {

    int R[10] = {0, };                      // 10 Registers
    int PC = 1;                             // Program counter starts from 1
    int i = 0;                              // Instructions counter

    // String tokenizer variables
    char str[32];                          
    char *opcode, *operand1, *operand2;
    char *delim = "\t\n ";
    
    // Open input.txt file
    FILE *stream = fopen("input.txt","r");
    if (stream == NULL){
        perror("File can't be opened\n");
        return 0;
    }
    else
        printf("Content of this file are :\n\n");

    // Memory to store the instructions
    char mem[32][32];
    char mem2[32][32];
    for (; fgets(str, 32, stream) != NULL ; i++){
        strcpy(mem[i], str);
        strcpy(mem2[i], mem[i]);
    }
	fclose(stream);

    // Main Execution of Instructions
    for (; PC <= i; PC++){
        opcode = strtok(mem[PC-1], delim);
		operand1 = strtok(NULL, delim);
		operand2 = strtok(NULL, delim);
    
        switch(*opcode){

            case '+':
                R[1] = calc(R, operand1);
                R[2] = calc(R, operand2);
                R[0] = R[1] + R[2];
                printf("PC = %d\n", PC);
                printf("%s %s %s -> R0 : %d = %d + %d\n", opcode, operand1, operand2, R[0], R[1], R[2]);
                break;
            
            case '-':
                R[1] = calc(R, operand1);
                R[2] = calc(R, operand2);
                R[0] = R[1] - R[2];
                printf("PC = %d\n", PC);
                printf("%s %s %s -> R0 : %d = %d - %d\n", opcode, operand1, operand2, R[0], R[1], R[2]);
                break;

            case '*':
                R[1] = calc(R, operand1);
                R[2] = calc(R, operand2);
                R[0] = R[1] * R[2];
                printf("PC = %d\n", PC);
                printf("%s %s %s -> R0 : %d = %d * %d\n", opcode, operand1, operand2, R[0], R[1], R[2]);
                break;

            case '/':
                R[1] = calc(R, operand1);
                R[2] = calc(R, operand2);
                printf("PC = %d\n", PC);
                if (R[2] == 0){
                    printf("%s %s %s -> R0 : INFINITE = %d / %d\n", opcode, operand1, operand2, R[1], R[2]);
                }
                else {
                    R[0] = R[1] / R[2];
                    printf("%s %s %s -> R0 : %d = %d / %d\n", opcode, operand1, operand2, R[0], R[1], R[2]);
                }
                break;

            case 'M':
                R[tar_reg(operand1)] = calc(R, operand2);
                printf("PC = %d\n", PC);
                printf("%s %s %s -> %s : %d\n", opcode, operand1, operand2, operand1, R[tar_reg(operand1)]);
                break;

            case 'J':
                printf("PC = %d\n", PC);
                PC = calc(R, operand1) - 1;
                printf("%s %s -> Jump to PC %d\n", opcode, operand1, calc(R, operand1));
                restore_mem(mem, mem2);
                break;

            case 'C':
                R[0] = compare(R, operand1, operand2);
                printf("PC = %d\n", PC);
                if (R[0] == 0)
                    printf("%s %s %s -> R0 : %d, where %d >= %d\n", opcode, operand1, operand2, R[0], R[tar_reg(operand1)], R[tar_reg(operand2)]);
                else
                    printf("%s %s %s -> R0 : %d, where %d < %d\n", opcode, operand1, operand2, R[0], R[tar_reg(operand1)], R[tar_reg(operand2)]);
                break;

            case 'B':
                printf("PC = %d\n", PC);
                if (R[0] == 1)
                    PC = calc(R, operand1) - 1;
                printf("%s %s -> Branch to PC %d\n", opcode, operand1, calc(R, operand1));
                restore_mem(mem, mem2);
                break;

            case 'G':
                R[0] = GCD(R, operand1, operand2);
                printf("PC = %d\n", PC);
                printf("%s %s %s -> R0 : %d\n", opcode, operand1, operand2, R[0]);
                break;

            case 'H':
                printf("PC = %d\n", PC);
                printf("Program Terminated with opcode Halt");
                return 0;

            default:
                printf("PC = %d\n", PC);
                printf("Invalid format\n");
                break;
        }
        printf("----------------------------------\n");	
    }
    return 0;
}