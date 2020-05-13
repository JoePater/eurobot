#include "system.h"

unsigned char i2c_status=0;
unsigned char i2c_settings=0;
unsigned char i2c_val=0;
struct properties props;

unsigned char motorDirArr[4] = {0,0,0,0};
unsigned char motorPWMArr[4] = {0,0,0,0};
unsigned char driveDir = 0;
unsigned char drivePWM1 = 0;
unsigned char drivePWM2 = 0; 
