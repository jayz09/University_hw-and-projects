#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define filename "input_files/fib.bin" // Change the name for different inputs

// Global variables
int memory[0x1000000]; // 16 MB memory
int pc, inst;
int reg[32];

struct instruction {
	int opcode, rs, rd, rt, imm, shamt, func, j_addr, b_addr;
	int exe_inst, i_inst, r_inst, j_inst, mem_access_inst, branch_taken;
	int result, save_index, acc_mem;
};
struct instruction instr;

// Main function declaration
void load_instruction(FILE* stream);
void initialize();
void InstructionFetch();
int InstructionDecode();
int execution();
int lw_sw_memory();
int write_back();

// Additional function declaration
int swap(unsigned char a[]);
void check();
void decode();
char get_inst_type();
int check_imm(unsigned int imm);
int print_output();

int main()
{
	FILE* stream;

	// Open file and load to memory
	if ((stream = fopen(filename, "rb")) == NULL) {
		perror("File cannot be opened");
		exit(1);
	}
	load_instruction(stream);
	fclose(stream);

	// Initialize everything at the beginning
	initialize();

	// In the loop...
	while (1) {
		InstructionFetch();
		InstructionDecode();
		execution();
		lw_sw_memory();
		write_back();
	}

	return 0;
}

// Loading instrution to memory
void load_instruction(FILE* stream) {
	int ret = 1, index = 0;
	int *ptr;
	unsigned char in[4];

	while (ret == 1) {
		for (int i = 0; i < 4; i++)
				ret = fread(&in[i], 1, 1, stream);

		memory[index] = swap(in);
		printf("(%d) load Mem[%X] pa: 0x%X val: 0x%X\n", (int)ret, index, &memory[index], memory[index]);
		index++;
	}
	instr.save_index = index;
}

// Initializer
void initialize() {
	memset(&reg, 0, sizeof(reg));
	reg[31] = -1;
	reg[29] = 0x1000000;
	pc = 0;
	instr.exe_inst = 0;
	instr.r_inst = 0;
	instr.i_inst = 0;
	instr.j_inst = 0;
	instr.mem_access_inst = 0;
	instr.branch_taken = 0;
}

// Instruction fetch phase
void InstructionFetch() {
	check("Program End"); // Checking whether there is still more instruction or not
	memset(&inst, 0, sizeof(inst));
	inst = memory[pc/4];
	printf("\n********** PC : %X **********\nFetch\t: %08X\n", pc, inst);
}

// Instruction decode phase
int InstructionDecode() {
	if (inst == 0x0) return 0;
	decode();

	char inst_type;
	inst_type = get_inst_type();
	printf("Decode\t: %c-type Instruction\n", inst_type);
	
	switch (inst_type)
	{
		case 'R':
			printf("\t  Op:%X\tRs:%X\tRt:%X\tRd:%X\tShamt:%X\tFunc:%X\n", instr.opcode, instr.rs, instr.rt, instr.rd, instr.shamt, instr.func);
			break;
		case 'I':
			printf("\t  Op:%X\tRs:%X\tRt:%X\tImm:%X\n", instr.opcode, instr.rs, instr.rt, instr.imm);
			break;
		case 'J':
			printf("\t  Op:%X\tAddress:%X\n", instr.opcode, instr.j_addr);
			break;
		default:
			printf("Instruction format is Unknown\n");
			break;
	}
	return 0;
}

// Handle big-little endian
int swap(unsigned char a[]) {
	unsigned tmp;
    int *ptr;

    tmp = a[1];
    a[1] = a[2];
    a[2] = tmp;

    tmp = a[3];
    a[3] = a[0];
    a[0] = tmp;

    ptr = (int *)&a[0];
	return *ptr;
}

// Error check function
void check(const char* a) {
	if (instr.save_index == (pc/4)) pc = -1;
	if (pc == -1) {
		print_output();
		perror(a);
		exit(0);
	}
}

// Decode Instructions
void decode() {
	instr.opcode = (inst >> 26) & 0x3f;
	instr.rs = (inst >> 21) & 0x1f;
	instr.rt = (inst >> 16) & 0x1f;
	instr.rd = (inst >> 11) & 0x1f;
	instr.shamt = (inst >> 6) & 0x1f;
	instr.func = (inst) & 0x3f;
	instr.imm = (inst) & 0xffff;
	instr.j_addr = (inst) & 0x3FFFFFF;
}

// Check immediate to be positive or negative
int check_imm(unsigned int imm) {
	if ((instr.opcode == 0xc) | (instr.opcode == 0xd)) 
		imm = (unsigned int)imm;
	else
		imm = (short)imm;
	return imm;
}

// Memory access
int lw_sw_memory() {
	if (inst == 0x0) return 0;
	// for store word
	if (instr.opcode == 0x2b) { 
		memory[(int32_t)(instr.result)] = reg[instr.rt];
		instr.mem_access_inst++;
	}
	// for load word
	else if (instr.opcode == 0x23) { 
		instr.acc_mem = memory[(int32_t)(instr.result)]; 
		instr.mem_access_inst++;
	}
}

// Intruction execution phase
int execution() {
	int result;
	if (inst == 0x0) return 0;

	switch (instr.opcode)
	{
		case 0x0:
			switch (instr.func)
			{
				case 0x0:
					result = reg[instr.rt] << instr.shamt;
					break;
				case 0x2:
					result = reg[instr.rt] >> instr.shamt;
					break;
				case 0x8:
					result = reg[instr.rs];
					break;
				case 0x20:
					result = reg[instr.rs] + reg[instr.rt];
					break;
				case 0x21:
					result = reg[instr.rs] + reg[instr.rt];
					break;
				case 0x22:
					result = reg[instr.rs] - reg[instr.rt];
					break;
				case 0x23:
					result = reg[instr.rs] - reg[instr.rt];
					break;
				case 0x24:
					result = reg[instr.rs] & reg[instr.rt];
					break;
				case 0x25:
					result = reg[instr.rs] | reg[instr.rt];
					break;
				case 0x27:
					result = !(reg[instr.rs] | reg[instr.rt]);
					break;
				case 0x2a:
					result = reg[instr.rs] < reg[instr.rt];
					break;
				case 0x2b:
					result = reg[instr.rs] < reg[instr.rt];
					break;
				default:
					break;
			}
			break;
		case 0x2:
			result = (instr.j_addr << 2);
			break;
		case 0x3:
			result = pc + 8;
			break;
		case 0x4:
			result = reg[instr.rs] == reg[instr.rt];
			break;
		case 0x5:
			result = reg[instr.rs] != reg[instr.rt];
			break;
		case 0x8:
			result = reg[instr.rs] + check_imm(instr.imm);
			break;
		case 0x9:
			result = reg[instr.rs] + check_imm(instr.imm);
			break;
		case 0xa:
			result = reg[instr.rs] < check_imm(instr.imm);
			break;
		case 0xb:
			result = reg[instr.rs] < check_imm(instr.imm);
			break;
		case 0xc:
			result = reg[instr.rs] & check_imm(instr.imm);
			break;
		case 0xd:
			result = reg[instr.rs] | check_imm(instr.imm);
			break;
		case 0xf:
			result = instr.imm;
			break;
		case 0x23:
			result = reg[instr.rs] + check_imm(instr.imm);
			break;
		case 0x2b:
			result = reg[instr.rs] + check_imm(instr.imm);
			break;
		default:
			printf("Opcode is unavailable");
			break;
	}
	instr.result = result;
	printf("Execute\t:\n");
}

// Get instruction type
char get_inst_type() {
	char type;
	if (inst == 0x0) return 0;
	
	switch (instr.opcode)
	{
		case 0x0:
			type = 'R';
			break;
		case 0x2:
			type = 'J';
			break;
		case 0x3:
			type = 'J';
			break;
		case 0x4:
			type = 'I';
			break;
		case 0x5:
			type = 'I';
			break;
		case 0x8:
			type = 'I';
			break;
		case 0x9:
			type = 'I';
			break;
		case 0xa:
			type = 'I';
			break;
		case 0xb:
			type = 'I';
			break;
		case 0xc:
			type = 'I';
			break;
		case 0xd:
			type = 'I';
			break;
		case 0xf:
			type = 'I';
			break;
		case 0x23:
			type = 'I';
			break;
		case 0x2b:
			type = 'I';
			break;
		default:
			printf("Opcode is unavailable");
			break;
		}

	instr.exe_inst++;
	
	if (type == 'I') instr.i_inst++;
	else if (type == 'R') instr.r_inst++;
	else if (type == 'J') instr.j_inst++;

	return type;
}

// Instruction write back phase
int write_back() {
	char *name;
	if (inst == 0x0) {
		pc = pc + 4;
		return 0;
	}

	switch (instr.opcode) {
		case 0x0:
			switch (instr.func)
			{
				case 0x0:
					name = "SLL";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d << %d\n", name, instr.rd, reg[instr.rd], instr.rt, instr.shamt);
					break;
				case 0x2:
					name = "SRL";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d >> %d\n", name, instr.rd, reg[instr.rd], instr.rt, instr.shamt);
					break;
				case 0x8:
					name = "JR";
					pc = instr.result;
					printf("WB\t: %s --> %X\n", name, pc);
					break;
				case 0x20:
					name = "ADD";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d + R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x21:
					name = "ADDU";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d + R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x22:
					name = "SUB";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d - R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x23:
					name = "SUBU";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (0x%X) = R%d - R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x24:
					name = "AND";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (%d) = R%d & R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x25:
					name = "OR";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (%d) = R%d | R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x27:
					name = "NOR";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (%d) = !(R%d | R%d)\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x2a:
					name = "SLT";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (%d) = R%d < R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				case 0x2b:
					name = "SLTU";
					reg[instr.rd] = instr.result;
					printf("WB\t: %s --> R%d (%d) = R%d < R%d\n", name, instr.rd, reg[instr.rd], instr.rs, instr.rt);
					break;
				default:
					break;
			}
			break;
		case 0x2:
			name = "J";
			pc = instr.result;
			printf("WB\t: %s --> %X\n", name, pc);
			break;
		case 0x3:
			name = "JAL";
			reg[31] = instr.result;
			pc = (instr.j_addr << 2);
			printf("WB\t: %s --> %X\n", name, pc);
			break;
		case 0x4:
			if (instr.result == 1) {
				name = "BEQ";
				pc = pc + 4 + (check_imm(instr.imm) << 2);
				instr.branch_taken++;
				printf("WB\t: %s --> %X\n", name, pc);
			}
			else {
				name = "Branch not taken";
				printf("WB\t: %s\n", name);
			}
			break;
		case 0x5:
			if (instr.result == 1) {
				name = "BNE";
				pc = pc + 4 + (check_imm(instr.imm) << 2);
				instr.branch_taken++;
				printf("WB\t: %s --> %X\n", name, pc);
			}
			else {
				name = "Branch not taken";
				printf("WB\t: %s\n", name);
			}
			break;
		case 0x8:
			name = "ADDI";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (0x%X) = R%d + %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0x9:
			name = "ADDIU";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (0x%X) = R%d + %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0xa:
			name = "SLTI";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (%d) = R%d < %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0xb:
			name = "SLTIU";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (%d) = R%d < %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0xc:
			name = "ANDI";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (%d) = R%d & %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0xd:
			name = "ORI";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (%d) = R%d | %d\n", name, instr.rt, reg[instr.rt], instr.rs, check_imm(instr.imm));
			break;
		case 0xf:
			name = "LUI";
			reg[instr.rt] = instr.result;
			printf("WB\t: %s --> R%d (%d)\n", name, instr.rt, reg[instr.rt]);
			break;
		case 0x23:
			name = "LW";
			reg[instr.rt] = instr.acc_mem;
			printf("WB\t: %s --> R%d = Memory[R%d + %d] (%X)\n", name, instr.rt, instr.rs, check_imm(instr.imm), instr.acc_mem);
			break;
		case 0x2b:
			name = "SW";
			printf("WB\t: %s --> Memory[%X] = R%d (0x%X)\n", name, instr.result, instr.rt, reg[instr.rt]);
			break;
		default:
			break;
	}
	if (name != "J" && name != "JAL" && name != "BEQ" && name != "BNE" && name != "JR")
        pc = pc + 4;
}

// Print output after completion of program
int print_output() {
	printf("\nFinal return value\t: %d\n", reg[2]);
	printf("Executed instructions\t: %d\n", instr.exe_inst);
	printf("R-type instructions\t: %d\n", instr.r_inst);
	printf("I-type instructions\t: %d\n", instr.i_inst);
	printf("J-type instructions\t: %d\n", instr.j_inst);
	printf("Memory instructions\t: %d\n", instr.mem_access_inst);
	printf("Branch Taken\t\t: %d\n\n", instr.branch_taken);
	return 0;
}