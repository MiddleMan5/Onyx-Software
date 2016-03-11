/* 
 * File:   Power.cpp
 * Author: Quinn
 * 
 * Created on September 4, 2015, 1:32 PM
 */

#include "Power.h"
//Power::Power() {
//}
//
//Power::Power(const Power& orig) {
//}
//
//Power::~Power() {
//}
Power::Power() {
    
}

void Power::setMainPin(int pin, bool invert = 0) {
    Main_Relay = pin;
    _invertMain = invert;
    _powerMain = OFF;
    ToggleMain(OFF);
}

void Power::ToggleMain(int state) {
    if (state != _powerMain) {
        if (state == TOGGLE)ToggleMain(!_powerMain);
        else {
            bool val = state;
            val = _invertMain ? !val : val;
                //Target.GPIO(Main_Relay, val);
            _powerMain = state;
        }
    } else return;

}
