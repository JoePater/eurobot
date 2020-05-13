#ifndef CONSTANTS_H
#define CONSTANTS_H

enum ErrorCode{
    NO_ERROR,
    //routing problem
    NO_VIABLE_ROUTE,

};

enum TeamColor{UNDEFINED,BLUE,YELLOW};
extern TeamColor team;
extern const double distance_to_claw;

#endif
