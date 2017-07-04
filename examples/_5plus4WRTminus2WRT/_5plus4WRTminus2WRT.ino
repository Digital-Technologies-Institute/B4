#include <B4.h>

B4 myB4;
/*  
 *  5+4 WRT -2 WRT
 */

  int DataRAMContent[] = {
    B0101, B0100, B0000, B0010,
    B0000, B0000, B0000, B0000,
    B0000, B0000, B0000, B0000,
    B0000, B0000, B0000, B0000,
  };

  int ProgramRAMContent[] = {
    B0010, B0000, B0110, B1000,
    B0110, B0000, B0000, B0000,
    B0000, B0000, B0000, B0000,
    B0000, B0000, B0000, B0000,
  };

void setup()
{
  myB4.loadDataAndProgram(DataRAMContent, ProgramRAMContent);
  myB4.programB4();
}

void loop()
{
}
