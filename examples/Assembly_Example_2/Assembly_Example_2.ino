#include <B4.h>

B4 myB4;
String assemblyProgram = "LOAD(1);ADD(2);ADD(3);ADD(4);SUB(7);LOAD(0);LOAD(0);";

void setup()
{
  Serial.begin(9600);
  myB4.assembler(assemblyProgram);
  myB4.programB4();
}

void loop()
{
}
