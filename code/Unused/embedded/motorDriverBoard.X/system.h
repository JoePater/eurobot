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
     * bit 3 = override?
     */
    unsigned char clamped;
    
    /* bit 7 = is moving?
     * bit 6 = is going towards normal orientation? (not edge cups)
     * bit 5 = normal orientation button pressed?
     * bit 4 = upside down button pressed?
     * bit 3 = override?
     */
    unsigned char orientation;
    
    /* bit 7 = is moving?
     * bit 6 = is going up?
     * bit 5 = up button pressed?
     * bit 4 = down button pressed?
     * bit 3 = override?
     */
    unsigned char height;
    
    /* bits 7:4 = routine
     * bits 3:0 = progress (0=complete)
     */
    unsigned char routine;
    
    
    signed long time1;
};


struct rev{
    /* bit 7 = is moving?
    * bit 6 = moving clockwise?
    * bit 5 = sensor activated?
    * bit 4 = override?
    * bits 3:0 = progress (0=complete)
    * 
    */
    unsigned char stat;
    
    /* bits 7:4 = routine
     * bits 3:0 = progress (0=complete)
     */
    //unsigned char routine;
    
    signed long time1;
};

struct properties{
    struct claw claw1;
    struct rev revolver;
    unsigned long time;
};



#endif	/* SYSTEM_H */

