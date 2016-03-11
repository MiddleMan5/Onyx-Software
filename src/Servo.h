/* 
 * File:   Servo.h
 * Author: Quinn
 *
 * Created on September 4, 2015, 1:34 PM
 */

#ifndef SERVO_H
#define	SERVO_H

#include <stdio.h>
#include "dynamixel.h"


#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Default setting
#define DEFAULT_BAUDNUM		1 // 1Mbps

//#include "utils.h"

class Servo {
    int _ID;
    int _GOAL;
    int _BAUD;
    bool _INVERT;
    
   public:
//    Servo();
//    Servo(const Servo& orig);
//    virtual ~Servo();
    
    Servo();
    
    
    
    
    void PrintCommStatus(int CommStatus);
    void PrintErrorCode();
    
    void SET_ID(int pin);
    void SET_GOAL(int goal);
    void SET_STATE();
    
    
    int GET_POSISTION();
    bool GET_MOVE_STATUS();
    
    void setTime(int input); //time it takes to move
    void setGroup(int groupNumber);
    void setMax(int Max);
    void setMin(int Min);
    void invert();
    
    void OPEN_CONTROLLER(int device, int baud);
    void CLOSE_CONTROLLER();
    
    
};


#endif	/* SERVO_H */

