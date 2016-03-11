#include "Onyx.h"
#include <stdio.h>

Power Actuators;

Onyx::Onyx() {
    //Assembly Leg[4](2);
    for(int i=0;i<4;i++){
        //Leg[i].setLinkLength(1,_L1);
        //Leg[i].setLinkLength(2,_L2);
        
        
        //Leg[i].setQuadrant(i+1); //Quadrants 1-4
    }
    
    
    
    Actuators.setMainPin(relayServos, true);
    setAttitude(200);
    setMode(LOWPOWER);
    
}

void Onyx::setMode(int mode) {
    _mode = mode;
    if (mode == STARTUP)startup();
    if (mode == STANDBY)listen();
    if (mode == SHUTDOWN)Actuators.ToggleMain(OFF);
    if (mode == LOWPOWER)setMode(SHUTDOWN);
    if (mode == REBOOT)startup();
}

void Onyx::startup() {
    Actuators.ToggleMain(ON);
    move(SIT);
    setMode(STANDBY);
    rotate(Z, 0);
    rotate(X, 0);
    rotate(Y, 0);
}

void Onyx::setAttitude(int attitude) {
//    _attitude = constrain(attitude, 0, 255);
    for (int i = 0; i < 4; i++) {
        //Legs[i].setSpeed(_attitude);
    }

}

void Onyx::moveScope(int scope) {

    if (scope != SERVO) {
        //SSC.newGroup(MOVE);
    } else return;
}

void Onyx::move(int type) {

    if (type == STAND) {
        moveScope(BODY);
        for (int i = 0; i < 4; i++) {
            //Legs[i].moveFootTo(L1, L2);
        }

    }
    if (type == SIT) {
        moveScope(BODY);
        for (int i = 0; i < 4; i++) {
           // Legs[i].moveFootTo(114.3, 25.4);
        }
    }

    if (type == UP) {
        moveScope(BODY);

    }

    if (type == DOWN) {
        moveScope(BODY);

    }

}

void Onyx::rotate(int axis, int degree) {

    if (axis == X) {
//        _X_rotation = constrain(degree, -45, 45);

        for (int i = 0; i < 4; i++) {
            //Legs[i].moveFootTo(L1, L2 + _X_rotation);
        }


        //Legs[3].moveFootTo(L1,L2-(L1*tan(_X_rotation)));

    }

    /*if(axis == Y){
            _Y_rotation = constrain(degree,-45,45);
                    Legs[0].move();
                    Legs[1].move();
                    Legs[2].move();
                    Legs[3].move();
			
    }*/

    if (axis == Z) {
        //_Z_rotation = constrain(degree, -45, 45);
        for (int i = 0; i < 4; i++) {
            //Legs[i].rotate(_Z_rotation);
        }
    }

}

void Onyx::listen() {
   
}
