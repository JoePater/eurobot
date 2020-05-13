/* 
 * File:   system.h
 * Author: joe
 *
 * Created on 11 December 2019, 13:27
 */
#include <xc.h>

#ifndef SYSTEM_H
#define	SYSTEM_H
struct claw{
    /* bit 7 = is moving?
     * bit 6 = is clamped/clamping?
     * bit 5 = clamped button pressed?
     * bit 4 = open button pressed?
     * bits 3:0 = approx. position (0=open,15=clamped)
     */
    unsigned char clamped;
    /* bit 7 = is moving?
     * bit 6 = is going towards normal orientation? (not edge cups)
     * bit 5 = normal orientation button pressed?
     * bit 4 = upside down button pressed?
     * bits 3:0 = approx. position (0=normal,15=upside down)
     */
    unsigned char orientation;
    /* bit 7 = is moving?
     * bit 6 = is going up?
     * bit 5 = up button pressed?
     * bit 4 = down button pressed?
     * bits 3:0 = approx. position (0=down)
     */
    unsigned char height;
    
    unsigned char routine;
    
    signed long time1; //approx. end of clamp
    signed long time2; //approx. end of rotate
    signed long time3; //approx. end of lift
    signed long time4;
};

/* bit 7 = is moving?
 * bit 6 = moving clockwise?
 * bit 5 = sensor activated?
 * bits 4:2 = most recent holder num
 * bits 1:0 = approx. position (0=at most recent)
 */
typedef unsigned char rev;

struct properties{
    struct claw claw1;
    rev revolver;
};

#endif	/* SYSTEM_H */

