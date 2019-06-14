#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2;
     	RF()
    	{
          Registers.resize(32);
          Registers[0] = bitset<32> (0);
        }
	// to provide read&wrt access
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
	{
		if (WrtEnable == 1)
		{
			Registers[WrtReg.to_ulong()] = WrtData;
		}
		ReadData1 = Registers[RdReg1.to_ulong()];
		ReadData2 = Registers[RdReg2.to_ulong()];
	}

	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {
                        rfout << Registers[j]<<endl;
                      }

                  }
                  else cout<<"Unable to open file";
                  rfout.close();

               }
	private:
            vector<bitset<32> >Registers;

};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {
                 int ALUop = ALUOP.to_ulong();
                 long long a;
                 a=oprand1.to_ulong();
                 long long b;
                 b=oprand2.to_ulong();
            switch(ALUop)
                 {
              case  1:
                  {long long c;
                  c=a+b;
                ALUresult=bitset<32>(c) ;
                break;}
              case 3:
                  {long long d;
                  d=a-b;
                ALUresult=bitset<32>(d) ;
                break;}
              case 4:
                {ALUresult=(oprand1&oprand2);
                break;}
              case 5:
                {ALUresult=(oprand1|oprand2);
                break;}
              case 7:
                {for(int i=0;i<=31;i++)
                    {ALUresult[i]=!(oprand1[i]|oprand2[i]);}
                break;}
              default:
                  break;
                 }
                 return ALUresult;
               }

};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize);
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {
                        IMem[i] = bitset<8>(line);
                        i++;
                     }

                  }
                  else cout<<"Unable to open file";
                  imem.close();

                  }

          bitset<32> ReadMemory (bitset<32> ReadAddress)
              {
           long Addr=ReadAddress.to_ulong();
                string inst1 = IMem[Addr].to_string();
                string inst2 = IMem[Addr+1].to_string();
                string inst3 = IMem[Addr+2].to_string();
                string inst4 = IMem[Addr+3].to_string();
            string Inst=inst1+inst2+inst3+inst4;
            Instruction=bitset<32>(Inst);
               return Instruction;
              }

      private:
           vector<bitset<8> > IMem;

};

class DataMem
{
      public:
          bitset<32> readdata;
          DataMem()
          {
             DMem.resize(MemSize);
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();

          }
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
          {
               int Addr;
               Addr=Address.to_ulong();
               if(writemem==1)
               {

                   DMem[Addr]=bitset<8>(WriteData.to_string());
                   DMem[Addr+1]=bitset<8>((WriteData<<8).to_string());
                   DMem[Addr+2]=bitset<8>((WriteData<<16).to_string());
                   DMem[Addr+3]=bitset<8>((WriteData<<24).to_string());
               }
               if(readmem==1)
               {
                string rdata1 = (DMem[Addr].to_string());
                string rdata2 = (DMem[Addr+1].to_string());
                string rdata3 = DMem[Addr+2].to_string();
                string rdata4 = DMem[Addr+3].to_string();
           string sreaddata=rdata1+rdata2+rdata3+rdata4;
           readdata=bitset<32>(sreaddata);

               }
               return readdata;
          }

          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {
                        dmemout << DMem[j]<<endl;
                       }

                  }
                  else cout<<"Unable to open file";
                  dmemout.close();

               }

      private:
           vector<bitset<8> > DMem;

};

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;

                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {

                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;

                  }
                  else cout<<"Unable to open file";
                  fileout.close();

}

bitset<32>SignExtend(bitset<16>imm)
{
    bitset<32>SignExtendImm;
    bitset<16>SignExtension;
    for(int i=0;i<=15;i++)
        {SignExtension[i]=imm[15];}
    SignExtendImm=bitset<32>(SignExtension.to_string()+imm.to_string());
    return SignExtendImm;
}

int main()
{
    bitset<32> pc;
    bitset<6> opcode;
    bitset<5> RdReg1;
    bitset<5> RdReg2;
    bitset<5> RdReg3;
    bitset<5> WrtReg;
    bitset<32>oprand1;
    bitset<32>oprand2;
    bitset<32>oprand3;
    bitset<16> Imm;
    bitset<3>ALUop;
    bitset<32> instruction;
    bitset<1>WrtEnable;
    bitset<1>readmem;
    bitset<1>wrtmem;
    bitset<32> DataMemAddress;
    bitset<32> DataMemory;
    bitset<5> regnull;
    bitset<32> oprandnull;
    bitset<32>beqaddr;
    bitset<32>jumpaddr;
    bitset<32>ALUoutcome;
    string Inst;
    int op;
    bool beqEn;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
//three-stage pipeline, no RAW hazard

    while (1) //each loop body corresponds to one clock cycle.
	{
//
//
//STAGE THREE
if(op==35)//load
{
    WrtEnable=1;
    readmem=1;
    wrtmem=0;
    DataMemAddress=ALUoutcome;
    DataMemory=myDataMem.MemoryAccess (DataMemAddress, myRF.ReadData2, readmem, wrtmem);
    oprand3=DataMemory;
    WrtReg=RdReg2;
}
else if(op==43)//store
{
    WrtEnable=0;
    readmem=0;
    wrtmem=1;
    DataMemAddress=ALUoutcome;
    myDataMem.MemoryAccess (DataMemAddress, myRF.ReadData2, readmem, wrtmem);
    DataMemory=myRF.ReadData2;

}
else if(op==9){
    oprand3=ALUoutcome;
    WrtEnable=1;
    readmem=0;
    wrtmem=0;
    WrtReg=RdReg2;
}
else if((op==0)&&(ALUop.to_ulong()>0)){
    oprand3=ALUoutcome;
    WrtEnable=1;
    readmem=0;
    wrtmem=0;
    WrtReg=RdReg3;

}
else if(op=4){
    WrtEnable=0;
    readmem=0;
    wrtmem=0;
    WrtReg.reset();
}
myRF.ReadWrite(regnull,regnull,WrtReg,oprand3,WrtEnable);




//STAGE TWO
  //instruction decoding
   opcode=bitset<6>((instruction).to_string());
   op=opcode.to_ulong();
   RdReg1=bitset<5>((instruction<<6).to_string());
   RdReg2=bitset<5>((instruction<<11).to_string());
   RdReg3=bitset<5>((instruction<<16).to_string());

   if(op==0){
       ALUop=bitset<3>((instruction<<29).to_string());
   }

   else if(op==9){//addiu
        ALUop=bitset<3>((instruction<<3).to_string());
   }

   else if(op==35){//load
       ALUop=bitset<3>((opcode>>2).to_string());

   }

   else if(op==43){//store
       ALUop=bitset<3>((opcode>>2).to_string());
    }

   else if(op==2)//jump
    {
       ALUop.reset();
       bitset<4>head;
       head=bitset<4>(pc.to_string());
       bitset<28>body;
       body=bitset<28>((instruction<<6).to_string());
       string string1;
       string string2;
       string1=head.to_string();
       string2=body.to_string();
       string jump;
       jump=string1+string2;
       jumpaddr=bitset<32>(jump);
    }
  myRF.ReadWrite(RdReg1,RdReg2,regnull,oprandnull,0);
   //readdate1=&reg1;
   //readdata2=&reg2;
//
//ALU operation
   oprand1=myRF.ReadData1;
   if(op==0)
   {
      oprand2=myRF.ReadData2;
      ALUoutcome=myALU.ALUOperation(ALUop,oprand1,oprand2);
   }
   if (op==9)//addiu
   {
    Imm=bitset<16>((instruction<<16).to_string());
    oprand2=SignExtend(Imm);
    ALUoutcome=myALU.ALUOperation(ALUop,oprand1,oprand2);
   }
   if (op==35)//load
   {
    Imm=bitset<16>((instruction<<16).to_string());
    oprand2=SignExtend(Imm);
    ALUoutcome=myALU.ALUOperation(ALUop,oprand1,oprand2);
   }
   if (op==43)//store
   {
    Imm=bitset<16>((instruction<<16).to_string());
    oprand2=SignExtend(Imm);
    ALUoutcome=myALU.ALUOperation(ALUop,oprand1,oprand2);
   }

   if(op==4){
       oprand2=myRF.ReadData2;
       beqEn=(oprand1==oprand2);
   long beqAddr;
         bitset<32>beq;
         Imm=bitset<16>((instruction<<16).to_string());
         beq=SignExtend(Imm);
         beq=(beq<<2);
         beqaddr=bitset<32>(pc.to_ulong()+beq.to_ulong());
       ALUoutcome.reset();
   }

   if(op==2)//jump
   {
       ALUoutcome.reset();
   }

//
//STAGE ONE
 // insrtruction fetch
instruction=myInsMem.ReadMemory(pc);
dumpResults(pc,WrtReg, oprand3,WrtEnable , DataMemAddress,DataMemory , wrtmem);
DataMemAddress.reset();
DataMemory.reset();
oprand3.reset();


if(instruction.to_string()=="11111111111111111111111111111111")
     {
     break;}
else if(op==2){
    pc=jumpaddr;
    instruction=NULL;
}

else if(op==4){
    if(beqEn==1){
         pc=beqaddr;
			beqEn=0;}
    else
        pc=bitset<32>(pc.to_ulong()+4);
}
else
    pc=bitset<32>(pc.to_ulong()+4);
 }
myRF.OutputRF();
myDataMem.OutputDataMem();
 return 0;

}
