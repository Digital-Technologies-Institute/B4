/*
  B4.h - Library.
  Created by Dr. Karsten Schulz, Digital Technologies Institute.
*/

#ifndef B4_h
#define B4_h

class B4
{
public:
    B4();
    void loadDataAndProgram(int mylocalData[], int mylocalProgram[]);
    void programB4();
    void assembler(String assemblerProgram);
    int  latchOutput();
    void clockCycle();
    void functionTest();

  private:
    void setData();
    void setProgram();
    void clearDataRAM();
    void clearProgramRAM();
    void reSetProgramCounter();
    void writeRAM(int port);
    void resetLatch();
  private:
    //program memory address bus (neutral position is B00XX)
    const int P3 = 5; //subtract (active HIGH)
    const int P2 = 4; //write Data RAM (active HIGH)
    const int P1 = 3; // 2-1 selector HIGH: RAM, LOW: Adder)
    const int P0 = 2; // User defined
    
    //data memory address bus
    const int D0 = 6;
    const int D1 = 7;
    const int D2 = 8;
    const int D3 = 9;
    
    //program counter reset. Connected to CLR PIN of Program Counter Module
    const int programCounterReset = A0;

    //program counter set. Connected to Set Program Counter PIN of Program Counter Module, Needed for jump instructions
    const int programCounterSet = A1;

    //selectDataRAMPCSource. LOW: Data RAM PC input is set by Program Counter Module. HIGH: Data RAM PC input is set by Latch. Needed for relative data addressing.
    
    const int selectDataRAMPCSource = A2;

    
    //control signals
    const int latchReset = 10; //active LOW
    const int writeProgramRAM = 11;
    const int writeDataRAM = 12;
    const int clockPIN = 13;
    const int programmerActive = A5; // This was A5 and should be changed back //A3 for I2C Communication
    
    int programCounterValue = 0;
    int myData[16]; 
    int myProgram[16];
};

#endif

