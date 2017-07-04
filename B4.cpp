/*
 B4.cpp - Library.
 Created by Dr. Karsten Schulz, Digital Technologies Institute
 */

#include "Arduino.h"
#include "B4.h"

B4::B4()
{
}

void B4::programB4()
{
    //Serial.begin(9600);
    pinMode(P0, OUTPUT);
    pinMode(P1, OUTPUT);
    pinMode(P2, OUTPUT);
    pinMode(P3, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(programCounterReset, OUTPUT);
    pinMode(programCounterSet, OUTPUT);
    pinMode(selectDataRAMPCSource, OUTPUT);
    pinMode(latchReset, OUTPUT);
    pinMode(writeProgramRAM, OUTPUT);
    pinMode(writeDataRAM, OUTPUT);
    pinMode(clockPIN, OUTPUT);
    pinMode(programmerActive, OUTPUT);
    
    //set the control signals to neutral position
    digitalWrite(programCounterReset, LOW);
    digitalWrite(programCounterSet, LOW);
    digitalWrite(selectDataRAMPCSource, LOW);
    digitalWrite(latchReset, HIGH);
    digitalWrite(writeProgramRAM, HIGH);
    digitalWrite(writeDataRAM, HIGH);
    digitalWrite(clockPIN, LOW);
    digitalWrite(programmerActive, LOW);
    //Start Programming
    Serial.println("Activating Automatic Programmer");
    
    digitalWrite(programmerActive, HIGH);
    
    Serial.println("Resetting Latch to 0000");
    resetLatch();
    
    Serial.println("Clearing Data RAM");
    clearDataRAM();
    
    Serial.println("Clearing Program RAM");
    clearProgramRAM();
    
    Serial.println("Programming Data RAM");
    setData();
    
    Serial.println("Programming Program RAM");
    setProgram();
    
    Serial.println("Resetting Latch to 0000");
    resetLatch();
    
    
    Serial.println("Deactivating Automatic Programmer");
    //switch ports to high impedance to avoid interference with the B4's own signals
    pinMode(P0, INPUT);
    pinMode(P1, INPUT);
    pinMode(P2, INPUT);
    pinMode(P3, INPUT);
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(programCounterSet, INPUT);
    pinMode(programCounterReset, INPUT);
    pinMode(selectDataRAMPCSource, INPUT);
    pinMode(latchReset, INPUT);
    pinMode(writeProgramRAM, INPUT);
    pinMode(writeDataRAM, INPUT);
    pinMode(clockPIN, INPUT);
    digitalWrite(programmerActive, LOW);
    
    Serial.println("Programming Complete");
    Serial.println("You can now run your program by pressing the Enter-button on the Program Counter.");
    //the programming is now done
    
}

void B4::setData()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(D3, HIGH && (myData[programCounterValue] & B1000));
        digitalWrite(D2, HIGH && (myData[programCounterValue] & B0100));
        digitalWrite(D1, HIGH && (myData[programCounterValue] & B0010));
        digitalWrite(D0, HIGH && (myData[programCounterValue] & B0001));
        writeRAM(writeDataRAM);
        clockCycle();
    }
}
void B4::setProgram()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(P3, HIGH && (myProgram[programCounterValue] & B1000));
        digitalWrite(P2, HIGH && (myProgram[programCounterValue] & B0100));
        digitalWrite(P1, HIGH && (myProgram[programCounterValue] & B0010));
        digitalWrite(P0, HIGH && (myProgram[programCounterValue] & B0001));
        writeRAM(writeProgramRAM);
        clockCycle();
    }
}

void B4::clearDataRAM()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        writeRAM(writeDataRAM);
        clockCycle();
    }
}

void B4::clearProgramRAM()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(P0, LOW);
        digitalWrite(P1, LOW);
        digitalWrite(P2, LOW);
        digitalWrite(P3, LOW);
        writeRAM(writeProgramRAM);
        clockCycle();
    }
}

void B4::reSetProgramCounter()
{
    // for HW Revision 1.0: programCounterReset is active LOW
    // for HW Revision>1.1: programCounterReset is active HIGH
    pinMode(programCounterReset, OUTPUT);
    digitalWrite(programCounterReset, HIGH); //active
    delay(100);
    digitalWrite(programCounterReset, LOW); //passive
    programCounterValue = 0;
    pinMode(programCounterReset, INPUT);
}

void B4::clockCycle()
{
    pinMode(clockPIN, OUTPUT);
    digitalWrite(clockPIN, HIGH);
    delay(100);
    digitalWrite(clockPIN, LOW);
    programCounterValue++;
    pinMode(clockPIN, INPUT);
}

void B4::writeRAM(int port)
{
    digitalWrite(port, LOW);
    delay(50);
    digitalWrite(port, HIGH);
    delay(50);
}

void B4::resetLatch()
{
    //reset the latch.
    digitalWrite(latchReset, LOW);
    delay(100);
    digitalWrite(latchReset, HIGH);
}

void B4::loadDataAndProgram(int mylocalData[], int mylocalProgram[])
{
    int i=0;
    while (i<16)
    {
        myData[i]=mylocalData[i];
        Serial.println(myData[i]);
        myProgram[i]=mylocalProgram[i];
        Serial.println(myProgram[i]);
        i++;
    }
}

void B4::assembler(String assemblerProgram)
{
    String assemblerProgramLines[16];
    String assemblerCodes[] = {"LOAD", "ADD", "SUB", "WRT"};
    int machineCodes[] = {B0010, B0000, B1000, B0110};
    String assemblerCode;
    String dataCode;
    int semicolonIndex = 0;
    int openBracketIndex = 0;
    int closingBracketIndex = 0;
    int programCounter = 0;
    int programLength = 0;
    B4 myBlueberry;
    
    int DataRAMContent[] = {
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
    };
    
    int ProgramRAMContent[] = {
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
    };
    
    //first, we break the assemblerProgram into its individual commands. The ';' separates them.
    while ((semicolonIndex != -1) && (programCounter <= 15))
    {
        semicolonIndex = assemblerProgram.indexOf(';');
        assemblerProgramLines[programCounter] = assemblerProgram.substring(0, semicolonIndex);
        assemblerProgram = assemblerProgram.substring(semicolonIndex + 1); // We can now remove the command from the assemblerProgram string.
        Serial.println(assemblerProgramLines[programCounter]);
        programCounter++;
    }
    Serial.print("Program length: ");
    Serial.println(programCounter);
    programLength=programCounter-1; //Programs start at index 0;
    
    //next, we break each program line into program and data component and translate the assembly codes into machine codes.
    
    programCounter=0;
    while (programCounter <= programLength)
    {
        openBracketIndex = assemblerProgramLines[programCounter].indexOf('('); // The identifier is now the opening bracket
        if (openBracketIndex!=-1)
        {
            assemblerCode=assemblerProgramLines[programCounter].substring(0, openBracketIndex);
            Serial.println(assemblerCode);
            if (assemblerCode.equals(assemblerCodes[0]))
            {
                ProgramRAMContent[programCounter]=machineCodes[0];
            }
            else if (assemblerCode.equals(assemblerCodes[1]))
            {
                ProgramRAMContent[programCounter]=machineCodes[1];
            }
            else if (assemblerCode.equals(assemblerCodes[2]))
            {
                ProgramRAMContent[programCounter]=machineCodes[2];
            }
            else if (assemblerCode.equals(assemblerCodes[3]))
            {
                ProgramRAMContent[programCounter]=machineCodes[3];
            }
        }
        // Next, look for the data
        assemblerProgramLines[programCounter]=assemblerProgramLines[programCounter].substring(openBracketIndex + 1);
        closingBracketIndex = assemblerProgramLines[programCounter].indexOf(')'); // The identifier is now the closing bracket
        if (closingBracketIndex!=-1)
        {
            dataCode=assemblerProgramLines[programCounter].substring(0, closingBracketIndex);
            Serial.println(dataCode);
            DataRAMContent[programCounter]=dataCode.toInt();
        }
        programCounter++;
    }
    
    programCounter=0;
    Serial.println("Program Machine Code");
    while (programCounter <= 15)
    {
        Serial.println(ProgramRAMContent[programCounter],BIN);
        programCounter++;
    }
    
    Serial.println("Data Machine Code");
    programCounter=0;
    while (programCounter <= 15)
    {
        Serial.println(DataRAMContent[programCounter],BIN);
        programCounter++;
    }
    loadDataAndProgram(DataRAMContent, ProgramRAMContent);
}

int B4::latchOutput()
{
    int output=0;
    output=output+digitalRead(D0);
    output=output+digitalRead(D1)*2;
    output=output+digitalRead(D2)*4;
    output=output+digitalRead(D3)*8;
    return output;
}

void B4::functionTest()
{
    String assemblerProgram= "LOAD(1);ADD(2);WRT();SUB(2);WRT();ADD(4);WRT();SUB(1);WRT();ADD(11);WRT();SUB(1);WRT();ADD(2);SUB(1);WRT()";
    assembler(assemblerProgram);
    programB4();
    
    pinMode(clockPIN, OUTPUT);
    pinMode(programCounterReset, OUTPUT);

    reSetProgramCounter();
    
    while (programCounterValue<15)
    {
        //clockCycle();
        delay(500);
    }
        
    pinMode(programCounterReset, INPUT);
    pinMode(clockPIN, INPUT);

}



