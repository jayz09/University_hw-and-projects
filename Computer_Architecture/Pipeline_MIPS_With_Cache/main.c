#include <stdio.h>
#include <string.h>
#include "header.h"
#define _CRT_SECURE_NO_WARNING

#define filename "input_files/simple.bin" // Change the name for different inputs

//global variables
int pc = 0;
int mem[0x100000] = {0, };
int R[32] ={0, };
int instr_count, exe_count, memacc, regops, branch, n_branch, jump;

//variables for cache
int coldmiss_count = 0;
int readmem_count = 0;
int mem_write_count = 0;
int hit_count = 0;
int miss_count = 0;
double cache_miss, cache_hit = 0;

//declare struct type
struct ifid latch_IFID[2];
struct idex latch_IDEX[2];
struct memwb latch_MEMWB[2];
struct exmem latch_EXMEM[2];
struct cache_ cache[1024];


//function variables
void IF(); //Instruction Fetch
void CS(); //Control Signal
void ID(); //Instruction Decode
void EXE(); //Instruction Execution 
void MEM(); //Memory access
void WB(); //Writeback result
int readmem(); //read memory by cache (load word)
int mem_write(); //write memory by cache (store word)


void main()
{
    FILE *fd;
    int cycle=0;
    int data=0;
    int i=0;
    int ret=0;
    unsigned inst_temp;
    unsigned temp;
    R[31] = 0xffffffff;
    R[29] = 0x100000;
    int inst = 0;

    fd=fopen(filename, "rb");
    while(1)
    {
        ret=fread(&data,sizeof(int),1,fd);
        if(ret==0)
        {
            break;
        }
        
        inst=((data & 0xff)<<24)|(((data & 0xff000000)>>24)& 0xff);
        inst_temp=((data& 0xff00)<<8)|(((data & 0xff0000)>>8) & 0xff00);
        inst = (inst | inst_temp);
        printf("mem[0x%08x] = 0x%08x\n", i, inst);
        mem[i++]=inst;
    }
    fclose(fd);

    while(1)
    {
        printf("___________ Cycle : %d _____________\n", cycle);
       
	    WB();
		IF();
		ID();
        EXE(); 
        MEM();

        latch_IFID[1]=latch_IFID[0];
        latch_IDEX[1]=latch_IDEX[0];
        latch_EXMEM[1]=latch_EXMEM[0];
        latch_MEMWB[1]=latch_MEMWB[0];
        
        if(pc==0xffffffff)
            break;
            
	    instr_count++;
    	cycle++;
        
        printf("r[2] : %d r[29] :  %d r[30] :  %d\n", R[2],R[29],R[30]);
    }
    cache_hit = (double)(hit_count)/(hit_count + miss_count) * 100;
    cache_miss = (double)(miss_count)/(hit_count + miss_count) * 100;
    printf("\n-------------------------------------------------\n\n");
    printf("Final result : r[2] = %d\n", R[2]);
    printf("Total number of cycles : %d\n", cycle);
    //printf("Instruction Execution Statistics :\n");
    //printf("Instruction count: %d\n", instr_count);
    printf("Memory access count = %d\n", memacc);
    printf("Register operation count = %d\n", regops);
    printf("Branch insruction count = %d\n", branch + n_branch);
    //printf("Taken branch count = %d\n", branch);
    //printf("Not taken branch count = %d\n", n_branch);
    //printf("Jump instruction count = %d\n", jump);
	printf("Cache hit = %f\n", cache_hit);
	printf("Cache miss = %f\n", cache_miss);
}


void IF(){
    latch_IFID[0].inst=readmem(pc);
    latch_IFID[0].pc=pc;
    pc=pc+4;
    //printf("Fetch -> pc[0x%08x] = 0x%08x\n", latch_IFID[0].pc, latch_IFID[0].inst);
}

void IDLatchUpdate(){
    latch_IDEX[0].pc=latch_IFID[1].pc;
    latch_IDEX[0].inst=latch_IFID[1].inst;
}

void CS()
{
    latch_IDEX[0].opcode=((latch_IFID[1].inst & 0xfc000000) >> 26) & 0x3f;
 
 	//sets the control signal
    if(latch_IDEX[0].opcode==0)
    {
        latch_IDEX[0].regdest=1;
    }
    else 
    {
        latch_IDEX[0].regdest=0;
    }
    
    if((latch_IDEX[0].opcode!=0) && (latch_IDEX[0].opcode!=bne) && (latch_IDEX[0].opcode!=beq))
    {  
        latch_IDEX[0].alusrc=1;
    }
    else 
    {
        latch_IDEX[0].alusrc=0;
    }
    
    if(latch_IDEX[0].opcode==lw)
    {
        latch_IDEX[0].memtoreg=1;
    }
    else 
    {
        latch_IDEX[0].memtoreg=0;
    }
    
    if((latch_IDEX[0].opcode!=sw) && (latch_IDEX[0].opcode!=beq) && (latch_IDEX[0].opcode!=bne) && (latch_IDEX[0].opcode!=j) && (latch_IDEX[0].opcode!=jr) && (latch_IDEX[0].opcode !=jal))
    {
        latch_IDEX[0].regwrite=1;
    }
    else 
    {
        latch_IDEX[0].regwrite=0;
    }
    
    if(latch_IDEX[0].opcode==lw)
    {
        latch_IDEX[0].memread=1;
    }
    else
    {
        latch_IDEX[0].memread=0;
    }
    
    if(latch_IDEX[0].opcode==sw)
    {
        latch_IDEX[0].memwrite=1;
    }
    else 
    {
        latch_IDEX[0].memwrite=0;
    }
    
    if((latch_IDEX[0].opcode==j) || (latch_IDEX[0].opcode==jal))
    {
        latch_IDEX[0].pcsrc1=1;
	}
    else
    {
       latch_IDEX[0].pcsrc1=0; 
    } 
    
    if((latch_IDEX[0].opcode==bne) || (latch_IDEX[0].opcode==beq))
    {
        latch_IDEX[0].pcsrc2=1;
    }
    else 
    {
        latch_IDEX[0].pcsrc2=0;
    }
}

void ID(){
    latch_IDEX[0].opcode = (((latch_IFID[1].inst & 0xfc000000)>> 26) & 0x3f);

    switch(latch_IDEX[0].opcode){
        case 0 : //R-TYPE
            latch_IDEX[0].rs=(latch_IFID[1].inst & 0x03e00000)>>21;
            latch_IDEX[0].rt=(latch_IFID[1].inst & 0x001f0000)>>16;
            latch_IDEX[0].rd=(latch_IFID[1].inst & 0x0000f800)>>11;
            latch_IDEX[0].shamt=(latch_IFID[1].inst & 0x000007c0)>>6;
            latch_IDEX[0].funct=(latch_IFID[1].inst & 0x0000003f);
            //printf("Decode -> opcode: %d rs : %d rt : %d rd : %d shamt : %d funct : %d\n", latch_IDEX[0].opcode, latch_IDEX[0].rs, latch_IDEX[0].rt, latch_IDEX[0].rd, latch_IDEX[0].shamt, latch_IDEX[0].funct);
            break;

        case j:
            latch_IDEX[0].addr = (latch_IFID[1].inst & 0x03ffffff);
            //printf("Decode -> opcode: %d address: %d\n", latch_IDEX[0].opcode, latch_IDEX[0].addr);
			break;
            
        case jal:
            latch_IDEX[0].addr = (latch_IFID[1].inst & 0x03ffffff);
            //printf("Decode -> opcode: %d address: %d\n", latch_IDEX[0].opcode, latch_IDEX[0].addr);
			break;

        default: // I-type
            latch_IDEX[0].rs=(latch_IFID[1].inst & 0x03e00000)>>21;
            latch_IDEX[0].rt=(latch_IFID[1].inst & 0x001f0000)>>16;
            latch_IDEX[0].imm=(latch_IFID[1].inst & 0x0000ffff);
			//printf("Decode -> opcode: %d\t rs : %d\t rt : %d\t imm : %x\n", latch_IDEX[0].opcode, latch_IDEX[0].rs, latch_IDEX[0].rt, latch_IDEX[0].imm);
		break;
    }
		
	//sign extended immediate
	if((latch_IFID[1].inst & 0x0000ffff) >= 0x8000){
		latch_IDEX[0].signextimm=0xffff0000 | (latch_IFID[1].inst & 0x0000ffff);	
		//printf("sign extended immediate : %x\n", latch_IDEX[0].signextimm);	
	}		   	 
	else{
		latch_IDEX[0].signextimm=(latch_IFID[1].inst & 0x0000ffff);	
		//printf("sign extended immediate : %x\n", latch_IDEX[0].signextimm);	
	}
			
	//jump address
	latch_IDEX[0].jumpaddr=(latch_IFID[1].pc & 0xf0000000) | (latch_IDEX[0].addr << 2);

	IDLatchUpdate();
	
	// jump
	if(latch_IDEX[0].opcode==j){
	    pc=latch_IDEX[0].jumpaddr;
	    //printf("JUMP pc : %d\n" , latch_IDEX[0].jumpaddr);
	    exe_count++;
	    jump++;
	}
	//jump and link
	else if(latch_IDEX[0].opcode==jal){
	    R[31]=latch_IDEX[0].pc+8;
	    pc=latch_IDEX[0].jumpaddr;
	    //printf("JAL @ r[31] = %d , pc = &d\n" , latch_IDEX[0].jumpaddr);
		jump++;
		exe_count++;
	}
	CS();
}

void EXE()
{
	int v0, v1;
	
	latch_IDEX[0].v0= R[latch_IDEX[0].rs];
	latch_IDEX[0].v1= R[latch_IDEX[0].rt];
	latch_IDEX[1].zeroextimm = (latch_IDEX[1].inst & 0x0000ffff);
	
	//data dependency handling
	if((latch_IDEX[1].rs!=0)&&(latch_IDEX[1].rs==latch_EXMEM[0].writereg)&&(latch_EXMEM[0].regwrite))
	{
	    latch_IDEX[1].v0 = latch_EXMEM[0].temp;
	}
	else if((latch_IDEX[1].rs!=0)&&(latch_IDEX[1].rs==latch_MEMWB[0].writereg)&&(latch_MEMWB[0].regwrite))
	{
	    if(latch_MEMWB[0].memtoreg==1){
	        latch_IDEX[1].v0 = latch_MEMWB[0].memresult;
	    }
	    else{
	        latch_IDEX[1].v0 = latch_MEMWB[0].temp;
	    }
	}
	
	if((latch_IDEX[1].rt!=0)&&(latch_IDEX[1].rt==latch_EXMEM[0].writereg)&&(latch_EXMEM[0].regwrite))
	{
	    latch_IDEX[1].v1 = latch_EXMEM[0].temp;
	}
	else if((latch_IDEX[1].rt!=0)&&(latch_IDEX[1].rt==latch_MEMWB[0].writereg)&&(latch_MEMWB[0].regwrite))
	{
	    if(latch_MEMWB[0].memtoreg==1){
	        latch_IDEX[1].v1 = latch_MEMWB[0].memresult;
	    }
	    else{
	        latch_IDEX[1].v1 = latch_MEMWB[0].temp;
	    }
	
	}
	
	v0 = latch_IDEX[1].v0;
	v1 = latch_IDEX[1].v1;
	
	//execute according to the control signal
	    if(latch_IDEX[1].regdest==1)
	    {
	        switch(latch_IDEX[1].funct)
	        {
	            case add:
	                latch_EXMEM[0].temp=v0+v1;
	                //printf("instruction : add\n");
	                exe_count++;
	                break;
	            case addu:
	                latch_EXMEM[0].temp=v0+v1;
	                //printf("instruction : addu\n");
	                exe_count++;
	                break;
	            case and:
	                latch_EXMEM[0].temp=v0&v1;
	                //printf("instruction : and\n");
	                exe_count++;
	                break;
	            case jr:
	                pc=v0;
	                //printf("v0 = %d\n", v0);
	                memset(&latch_IFID[0], 0 ,sizeof(ifid));
	                memset(&latch_IDEX[0], 0 ,sizeof(idex));
	                //printf("instruction : jr\n");
	                jump++;
	                break;
	            case nor:
	                latch_EXMEM[0].temp!= (v0|v1);
	                //printf("instruction : nor\n");
	                exe_count++;
	                break;
	            case or:
	                latch_EXMEM[0].temp=v0|v1;
	                //printf("instruction : or\n");
	                exe_count++;
	                break;
	            case slt:
	                latch_EXMEM[0].temp=((v0<v1) ? 1:0);
	                //printf("instruction : slt\n");
	                exe_count++;
	                break;
	            case sltu:
	                latch_EXMEM[0].temp=((v0<v1) ? 1:0);
	                //printf("instruction : sltu\n");
	                exe_count++;
	                break;
	            case sll:
	                latch_EXMEM[0].temp=v1<<latch_IDEX[1].shamt;
	                exe_count++;
	                if(latch_IDEX[1].inst == 0x00000000)
	                {
	                    //printf("instruction: nop\n");
	                }
	                //printf("instruction: sll\n");
	                break;
	            case srl:
	                latch_EXMEM[0].temp= v1 >>latch_IDEX[1].shamt;
	                //printf("instruction : srl\n");
	                exe_count++;
	                break;
	            case sub:
	                latch_EXMEM[0].temp=v0-v1;
	                //printf("instruction : sub\n");
	                exe_count++;
	                break;
	            case subu:
	                latch_EXMEM[0].temp=v0-v1;
	                //printf("instruction : subu\n");
	                exe_count++;
	                break;
	            default:
	                break;
	        }
	    }
	    else if (latch_IDEX[1].alusrc==1)
	    {
	        switch(latch_IDEX[1].opcode)
	        {
	            case addi:
	                latch_EXMEM[0].temp=(v0 + latch_IDEX[1].signextimm);
	                //printf("instruction : addi\n");
	                exe_count++;
	                break;
	            case sw:
	                latch_EXMEM[0].temp=v1;
	                latch_EXMEM[0].word=v0+latch_IDEX[1].signextimm;
	                //printf("instruction : sw\n");
	                exe_count++;
	                break;
	            case lw:
	                latch_EXMEM[0].word=v0+latch_IDEX[1].signextimm;
	                //printf("instruction : lw\n");
	                exe_count++;
	                break;
	            case addiu:
	                latch_EXMEM[0].temp=v0+latch_IDEX[1].signextimm;
	                //printf("instruction : addiu\n");
	                exe_count++;
	                break;
	            case andi:
	                latch_EXMEM[0].temp=v0 & latch_IDEX[1].zeroextimm;
	                //printf("instruction : andi\n");
	                exe_count++;
	                break;
	            case lui:
	                latch_EXMEM[0].temp=latch_IDEX[1].imm << 16;
	                //printf("instruction : lui\n");
	                exe_count++;
	                break;
	            case ori:
	                latch_EXMEM[0].temp=v0|latch_IDEX[1].zeroextimm;
	                //printf("instruction : ori\n");
	                exe_count++;
	                break;
	            case slti:
	                latch_EXMEM[0].temp=((v0<latch_IDEX[1].signextimm) ? 1:0);
	                //printf("instruction : slti\n");
	                exe_count++;
	                break;
	            case sltiu:
	                latch_EXMEM[0].temp=((v0<latch_IDEX[1].signextimm) ? 1:0);
	                //printf("instruction : sltiu\n");
	                exe_count++;
	                break;
	            default:
	                break;
	        }
	    }
	    else if (latch_IDEX[1].pcsrc2==1)
	    {
	        switch(latch_IDEX[1].opcode)
	        {
	            case beq:
	                if(v0==v1)
	                {
	                    latch_IDEX[1].branchaddr=latch_IDEX[1].signextimm<<2;
	                    pc=latch_IDEX[1].pc + latch_IDEX[1].branchaddr +4;
	                    memset(&latch_IFID[0], 0 ,sizeof(ifid));
	                    memset(&latch_IDEX[0], 0 ,sizeof(idex)); 
	                    exe_count++;
	                    branch++;              
	                }
	                else
	                {
	                    n_branch++;
	                }
	                break;
	            case bne:
	                if(v0!=v1)
	                {
	                    latch_IDEX[1].branchaddr=latch_IDEX[1].signextimm<<2;
	                    pc=latch_IDEX[1].pc + latch_IDEX[1].branchaddr +4;
	                    memset(&latch_IFID[0], 0 ,sizeof(ifid));
	                    memset(&latch_IDEX[0], 0 ,sizeof(idex)); 
	                    exe_count++;
	                    branch++;              
	                }
	                else
	                {
	                    n_branch++;
	                }
	                break;
	            default:
	                break;
	        }
	    }
	    
	if(latch_IDEX[1].regdest==1){
	    latch_EXMEM[0].writereg = latch_IDEX[1].rd;
	}
	else {
	    latch_EXMEM[0].writereg = latch_IDEX[1].rt;
	}
	
	//Get the next instruction through latch
	latch_EXMEM[0].pc=latch_IDEX[1].pc;
	latch_EXMEM[0].inst=latch_IDEX[1].inst;
	latch_EXMEM[0].opcode=latch_IDEX[1].opcode;
	latch_EXMEM[0].funct=latch_IDEX[1].funct;
	latch_EXMEM[0].memtoreg=latch_IDEX[1].memtoreg;
	latch_EXMEM[0].regwrite=latch_IDEX[1].regwrite;
	latch_EXMEM[0].memread=latch_IDEX[1].memread;
	latch_EXMEM[0].memwrite=latch_IDEX[1].memwrite;
	latch_EXMEM[0].pcsrc1=latch_IDEX[1].pcsrc1;
	latch_EXMEM[0].pcsrc2=latch_IDEX[1].pcsrc2;
	latch_EXMEM[0].v1= v1;

}

void MEM()
{
    	//load word
        if(latch_EXMEM[1].memread)
        {
			latch_MEMWB[0].memresult = readmem(latch_EXMEM[1].word);
        	readmem_count++;
			memacc++;
        }
        //store word
        if(latch_EXMEM[1].memwrite)
        {
        	mem_write(latch_EXMEM[1].index, latch_EXMEM[1].temp);
			mem_write_count++;
        	memacc++;
        }
	
		//continue to the next instruction
        latch_MEMWB[0].inst=latch_EXMEM[1].inst;
        latch_MEMWB[0].opcode=latch_EXMEM[1].opcode;
        //latch_MEMWB[0].memresult=latch_EXMEM[1].memresult;
        latch_MEMWB[0].funct=latch_EXMEM[1].funct;
        latch_MEMWB[0].memtoreg=latch_EXMEM[1].memtoreg;
        latch_MEMWB[0].regwrite=latch_EXMEM[1].regwrite;
        latch_MEMWB[0].v0=latch_EXMEM[1].v0;
        latch_MEMWB[0].v1=latch_EXMEM[1].v1;
        latch_MEMWB[0].temp=latch_EXMEM[1].temp;
        latch_MEMWB[0].writereg=latch_EXMEM[1].writereg;
}   

void WB()
{
    if(latch_MEMWB[1].regwrite == 1)
    {
        if(latch_MEMWB[1].memtoreg == 1){
            R[latch_MEMWB[1].writereg] = latch_MEMWB[1].memresult;
            //printf("WB 1\n R[%d] = %d\n", latch_MEMWB[1].writereg, latch_MEMWB[1].memresult);
		}
        else
        {
            if(latch_MEMWB[1].writereg != 0)
                R[latch_MEMWB[1].writereg]=latch_MEMWB[1].temp;
            else R[latch_MEMWB[1].writereg]=0;
            //printf("WB 2\n R[%d] = %d\n", latch_MEMWB[1].writereg, latch_MEMWB[1].temp);
        }
        regops++;
    }
    else{}
}

int mem_write(int addr, int value){
	int tag = (addr & 0xffff0000) >> 16;
	int index = (addr & 0x0000ffc0) >> 6;
	int offset = (addr & 0x0000003f);
	int dest_addr = (addr & 0xffffffc0);
	int drain_addr = (cache[index].tag << 16) | (index << 6);
	
	//cold miss
	if(cache[index].valid != 1){
	 	for (int i = 0; i < 16; i++){
	 		cache[index].data[i] = mem[(dest_addr/4) + i];
	 		//printf("cache data: 0x%08x\n", cache[index].data[i]);
		}
		cache[index].data[offset/4] = value;
		cache[index].tag = tag;
		cache[index].valid = 1;
		cache[index].dirty = 1;
		miss_count++;
		return 0;
	}
	
	//miss case
	else if(cache[index].valid == 1){
		if (cache[index].tag != tag){ //miss case
						
			if (cache[index].dirty == 1){ //dirty drain the cacheline
				for(int i = 0; i<16; i++){ //drain memory
					mem[(drain_addr/4) + i] = cache[index].data[i];
				}
			}
			for(int i = 0; i<16; i++){
				//fill the cache storage
				cache[index].data[i] = mem[(dest_addr/4) + i];
			}
			}
		cache[index].data[offset/4] = value;
		cache[index].tag = tag;
		cache[index].valid = 1;
		cache[index].dirty = 1;
		hit_count++;
	
		return 0;
	}
	
	//hit case
	else if(cache[index].tag == tag){
		cache[index].data[offset/4] = value;
		cache[index].dirty = 1;
		return 0;
	}
}

int readmem(int addr){
	int tag = (addr & 0xffff0000) >> 16;
	int index = (addr & 0x0000ffc0) >> 6;
	int offset = (addr & 0x0000003f);
	int dest_addr = (addr & 0xffffffc0);
	int drain_addr = (cache[index].tag << 16) | (index << 6);
	
	//cold miss
	if(cache[index].valid != 1){
		for (int i = 0; i < 16; i++){
	 		cache[index].data[i] = mem[(dest_addr/4) + i];
	 		//printf("cache data: 0x%08x\n", cache[index].data[i]);
		}
		cache[index].tag = tag;
		cache[index].valid = 1;
		cache[index].dirty = 0;
		miss_count++;
		coldmiss_count++;
		return cache[index].data[offset/4];
	}
	
	//miss case
	else if(cache[index].valid == 1){
		if (cache[index].tag != tag){ //miss case
			if (cache[index].dirty == 1){ //dirty drain the cacheline
				for(int i = 0; i<16; i++){ //drain memory
					mem[(drain_addr/4) + i] = cache[index].data[i];
				}
			}
			
			for(int i = 0; i<16; i++){
				//fill the cache storage
				cache[index].data[i] = mem[(dest_addr/4) + i];
			}
		}
		
		cache[index].tag = tag;
		cache[index].valid = 1;
		cache[index].dirty = 0;
		miss_count++;
	
		return cache[index].data[offset/4];
	}
	
	//hit case
	else if(cache[index].tag == tag){
		hit_count++;
		return cache[index].data[offset/4];
	}
}
