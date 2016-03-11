/* 
 * File:   Servo.cpp
 * Author: Quinn
 * 
 * Created on September 4, 2015, 1:34 PM
 */

#include "Servo.h"

//Servo::Servo() {
//}
//
//Servo::Servo(const Servo& orig) {
//}
//
//Servo::~Servo() {
//}

static long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Servo::Servo() {
    _INVERT = false;
}

void Servo::SET_ID(int ID) {
    _ID = ID;
}

void Servo::invert() {
    _INVERT = !_INVERT;
}

bool Servo::GET_MOVE_STATUS() {
return(dxl_read_byte( _ID, P_MOVING ));
}

int Servo::GET_POSISTION() {
    int output;
    int PresentPos = dxl_read_word( _ID, P_PRESENT_POSITION_L );
	int CommStatus = dxl_get_result();

                if( CommStatus == COMM_RXSUCCESS )
                {
                        PrintErrorCode();
                        output = PresentPos;
                }
                else
                {
                        PrintCommStatus(CommStatus);
                        output = -1;
                }
        return(output);
}


void Servo::SET_GOAL(int goal) {
    if (_INVERT) _GOAL = map(goal, 0, 1023, 1023, 0);
    else _GOAL = goal;
    dxl_write_word( _ID, P_GOAL_POSITION_L, _GOAL );
    //SSC.servoMoveTime(_pin,_position,_time); //Greater time means slower speed (DUH!)
}

void Servo::PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now receiving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void Servo::PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}

void Servo::OPEN_CONTROLLER(int device, int baud){
    ///////// Open USB2Dynamixel ////////////
	if( dxl_initialize(device, baud) == 0 )
	{
		printf( "Failed to open Signal-Controller!\n" );
	}
	else
		printf( "Signal-Controller Link Established.\n" );
}
void Servo::CLOSE_CONTROLLER(){
    dxl_terminate();
}