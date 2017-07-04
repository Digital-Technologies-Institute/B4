#include <B4.h>

B4 myB4;
String assemblyProgram = "LOAD(5);ADD(4);WRT();SUB(2);WRT();";

void setup()
{
  Serial.begin(9600);
  myB4.assembler(assemblyProgram);
  myB4.programB4();
}

void loop()
{
}
