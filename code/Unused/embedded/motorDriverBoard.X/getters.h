/* 
 * File:   getters.h
 * Author: root
 *
 * Created on 23 October 2019, 07:46
 */
#include <xc.h>

#ifndef GETTERS_H
#define	GETTERS_H

#ifdef SMT_REQUIRED
char getSMT(char recNum);
#endif

#ifdef CURRENT_REQUIRED
int getCurrent();
#endif

#endif	/* GETTERS_H */

