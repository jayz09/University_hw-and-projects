#define j 0x2
#define jal 0x3
#define jr 0x08
#define add 0x20
#define addi 0x8
#define addiu 0x9
#define addu 0x21
#define and 0x24
#define andi 0xc
#define beq 0x4
#define bne 0x5
#define lui 0xf
#define lw 0x23
#define nor 0x27
#define or 0x25
#define ori 0xd
#define slt 0x2a
#define slti 0xa
#define sltiu 0xb
#define sltu 0x2b
#define sll 0x00
#define srl 0x02
#define sw  0x2b
#define sub 0x22
#define subu 0x23

struct ifid{
    int inst;
    int pc;
}ifid;

struct idex{
    int inst;
    int pc;
    int opcode,rs,rt,rd,imm,addr,shamt,funct,signextimm;
    int jumpaddr,branchaddr,zeroextimm;
    int regdest,alusrc,memtoreg,regwrite,memread,memwrite,pcsrc1,pcsrc2;
    int v0,v1,temp,word;
    int index;
}idex;

struct exmem{
    int inst;
    int pc;
    int opcode,rs,rt,rd,imm,addr,shamt,funct,signextimm;
    int jumpaddr,branchaddr,zeroextimm;
    int regdest,alusrc,memtoreg,regwrite,memread,memwrite,pcsrc1,pcsrc2;
    int v0,v1,temp,word;
    int writereg,memresult;
    int index;
}exmem;

struct memwb{
    int inst;
    int pc;
    int opcode,rs,rt,rd,imm,addr,shamt,funct,signextimm;
    int jumpaddr,branchaddr,zeroextimm;
    int regdest,alusrc,memtoreg,regwrite,memread,memwrite,pcsrc1,pcsrc2;
    int v0,v1,temp,word;
    int memresult,writereg;
    int index;
}memwb;

struct cache_{
	unsigned int tag;
	unsigned int valid;
	unsigned int dirty;
	int data[16];
}cache_;
