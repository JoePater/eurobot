#ifndef DRIVE_H
#define DRIVE_H

class Drive{
    int left,right;
public:
    void setLeft(int pwm){if(pwm>=0&&pwm<=100)left=pwm;};
    void setRight(int pwm){if(pwm>=0&&pwm<=100)right=pwm;};
    void setMotors(int l,int r){setLeft(l);setRight(r);};
    int getLeft()const{return left;};
    int getRight()const{return right;};
};









#endif
