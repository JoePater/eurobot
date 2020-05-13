

#ifndef REVOLVER_H
#define REVOLVER_H
#include  <vector>


enum Holder{HOLDER_OPEN,HOLDER_WAITING,HOLDER_CLOSED,HOLDER_RED,HOLDER_GREEN};

class Revolver{
    std::vector<Holder> holders;
    //0 is initial position
    //positive is clockwise, negative is anticlockwise
    int current;
public:
    Revolver();
    int clockwiseLimit()const;
    int anticlockwiseLimit()const;
    void rotate(int count);
    int totalRed()const;
    int totalGreen()const;
    
    Holder currentBuoy()const;
    //bool isEmpty()const;
    bool isFull()const;
    int nextFree()const; //path to next free, 100 if empty
    void addCup(bool red);
    void openCurrent(){holders[current%5]=HOLDER_OPEN;};
    void setHolder(int n,Holder h); //-1 is current
    int currentPosition()const{return current%5;};
};
#endif
