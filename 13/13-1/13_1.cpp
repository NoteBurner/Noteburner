#include <stdio.h>

int main(int argc, char* argv[])  { 
  try  {
    throw 1;	// �׳��쳣
  }
  catch ( int e )  {
    printf(" ���� int �쳣 \r\n");
  }
  catch ( float e)  {
    printf(" ���� float �쳣 \r\n");
  }
  return 0;
}
