/* 
 * File:   Power.h
 * Author: Quinn
 *
 * Created on September 4, 2015, 1:32 PM
 */

#ifndef POWER_H
#define	POWER_H

#define OFF     0
#define ON      1
#define TOGGLE  2

class Power {
    public:
//    Power();
//    Power(const Power& orig);
//    virtual ~Power();
    
    Power();
    void setMainPin(int pin, bool invert);
    void ToggleMain(int state);
    void invertMain();
private:
    int Main_Relay;
    bool _powerMain; //Power State (ON,OFF)
    bool _invertMain; //Active low (True) vs. Active High (False)
};

#endif	/* POWER_H */

