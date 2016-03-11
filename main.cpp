// Smart Servo Framework
#include "../SmartServoFramework-master/src/DynamixelController.h"

// C++ standard libraries
#include <iostream>
#include <fstream> //write/read file
#include <string> //Manipulate strings
#include <thread>
#include <unistd.h> //usleep function
#include <math.h> // atan2
#include <ncurses.h>
/* ************************************************************************** */

// ID of the servos you want to use in this test program:
#define ID_SERVO_1       30
#define ID_SERVO_2       31
#define ID_SERVO_3       32
#define ID_SERVO_4       40
#define ID_SERVO_5       41
#define ID_SERVO_6       42


#define L1 80
#define L2 133 

#define pi 3.14159

using namespace std;

// -------Special Functions---------
float start_target[2]={0,L1+L2};
float target[2]={0,L1+L2};
float targetangle[2]={0,0};
bool mirror_leg=0;

int pos_array[3][10000];
int pos_array2[3][10000];
bool mirrorMode=0;
bool run=1;

/* ************************************************************************** */
int checktarget(){
if(sqrt(pow(target[0],2)+pow(target[1],2)) > L1 + L2)return 0;
else return 1;
}

int storeData(int pos){
ofstream data;
data.open("Position_Data",ofstream::app);
if(data.fail())cout << "Error Opening File." << endl;
    data << pos << endl;
}

int readData(int lineNum){
ifstream data;
data.open("Position_Data");
if(data.fail())cout << "Error Opening File." << endl;
string inString;
  if (data.is_open())
  {
  data.seekg(ios::beg);
    for(int i=0; i < lineNum - 1; ++i){
        data.ignore(10,'\n');
    }
  data >> inString;
  }
  int returnVal = stoi(inString);
  //cout << "The Value of " << lineNum << " is " << inString << endl;
  return(returnVal);
}

void IK(float x, float y){
// Based on Robot_Arm_Designer.xls
if(checktarget() == 1){
  float c2, s2, k1, k2;
  c2=(pow(x,2)+pow(y,2)-pow(L1,2)-pow(L2,2))/(2*L1*L2);
  s2=sqrt(1-pow(c2,2));
  k1=L1+L2*c2;
  k2=L2*s2;
  float gTheta1=atan2(y,x)-atan2(k2,k1);
  float gTheta2=atan2(s2,c2);

  if (gTheta1<0) gTheta1 = 2 * pi + gTheta1;
  if (gTheta2<0) gTheta2 = 2 * pi + gTheta2;
  if(mirror_leg==1){
  gTheta1 = 90 - gTheta1;
  gTheta2 = 90 - gTheta2;
  }

  gTheta2 =(gTheta2 * (180.0/pi)+90);
  gTheta1 = (gTheta1 * (180.0/pi));
  cout << "\r";
  cout << "\r";
  cout << "X = " << x << "\n\r" << "Y = " << y << "\n\r" << endl;
  cout << "Theta 1 = " << gTheta1 << "\n\r" << "Theta 2 = " << gTheta2 << "\n\r" << endl;

  targetangle[0] = 1023-(-3.422*gTheta1+821); // 1023 reverses servo posistions to allow backwards knee, 3.422 steps = 1 degree
  targetangle[1] = 1023-(3.422*gTheta2+205);
  cout << "Servo 1 = " << targetangle[0] << "\n\r" << "Servo 2 = " << targetangle[1] << "\n\r" << endl;
  }
}


int main(int argc, char *argv[])
{
    cout << endl << "======== ONYX TEST SOFTWARE ========" << endl;
    DynamixelController ctrl(70,SERVO_AX12A);
    string deviceName = "auto";
    if (ctrl.connect(deviceName, 1) == 0)
    {
       cerr << "> Failed to open a serial link for our ControllerAPI! Exiting..." << endl;
        
        exit(EXIT_FAILURE);
    }

    cout << endl << "======== INITIALIZING SERVOS ========" << endl;
    ServoAX s1(ID_SERVO_1, 12);
    ServoAX s2(ID_SERVO_2, 12);
    ServoAX s3(ID_SERVO_3, 12);
    ServoAX s4(ID_SERVO_4, 12);
    ServoAX s5(ID_SERVO_5, 12);
    ServoAX s6(ID_SERVO_6, 12);
    // Register them to the controller
    ctrl.registerServo(&s1);
    ctrl.registerServo(&s2);
    ctrl.registerServo(&s3);
    ctrl.registerServo(&s4);
    ctrl.registerServo(&s5);
    ctrl.registerServo(&s6);
    ctrl.waitUntilReady();
	usleep(100000);
	s1.setMovingSpeed(100);
	s2.setMovingSpeed(100);
	s3.setMovingSpeed(100);
	s4.setMovingSpeed(100);
	s5.setMovingSpeed(100);
	s6.setMovingSpeed(100);
	usleep(100000);
	s1.setGoalPosition(512);
	s2.setGoalPosition(276);
	s3.setGoalPosition(160);
	s4.setGoalPosition(512);
	s5.setGoalPosition(1023-276);
	s6.setGoalPosition(1023-160);
	usleep(2000000);
	s1.setMovingSpeed(1023);
	s2.setMovingSpeed(1023);
	s3.setMovingSpeed(1023);
	s4.setMovingSpeed(1023);
	s5.setMovingSpeed(1023);
	s6.setMovingSpeed(1023);
	usleep(100000);
    IK(target[0],target[1]);
   cout << endl << "======== MAIN LOOP ========" << endl;

  cout << endl << "> Use the keypad to move (press ESC or any other key to exit)" << endl;
    IK(target[0],target[1]);
    system("/bin/stty raw");
int sleepFactor = 100;
 bool running = true;
    while (running)
    {
   usleep(sleepFactor);
        // Always get CLI cursor back to column 0
       cout << "\r";
       int key = getchar();
        // Move order?
        
        switch (key)
        {
        case 52: // kp 4
            target[0]-=1;
            IK(target[0],target[1]);
            s2.setGoalPosition(targetangle[0]);
            s3.setGoalPosition(targetangle[1]);
            break;
        case 54: // kp 6
            target[0]+=1;
            IK(target[0],target[1]);
            s2.setGoalPosition(targetangle[0]);
            s3.setGoalPosition(targetangle[1]);
            break;

        case 50: // kp 2
            //s2.moveGoalPosition(-32);
            target[1]+=1;
            IK(target[0],target[1]);
            s2.setGoalPosition(targetangle[0]);
            s3.setGoalPosition(targetangle[1]);
            break;
        case 56: // kp 8
            //s2.moveGoalPosition(+32);
            target[1]-=1;
            IK(target[0],target[1]);
            s2.setGoalPosition(targetangle[0]);
            s3.setGoalPosition(targetangle[1]);
            break;

        case 51: // kp 3
            s3.moveGoalPosition(-32);
            break;
        case 55: // kp 7
            s3.moveGoalPosition(+32);
            break;

        case 49: // kp 1
            s4.moveGoalPosition(-32);
            break;
        case 57: // kp 9
            s4.moveGoalPosition(+32);
            break;

        case 53: // kp 5
            IK(start_target[0],start_target[1]);
            target[0] = start_target[0];
            target[1] = start_target[1];
            s2.setGoalPosition(targetangle[0]);
            s3.setGoalPosition(targetangle[1]);

            break;
            
            case 43: // kp +
            mirrorMode = 1;

            break;
            
            case 45: // kp -
            mirrorMode = 0;

            break;
            
            case 42: // kp *
            cout << "\r Begin Capture \r" << endl;
            s1.setTorqueEnabled(0);
            s2.setTorqueEnabled(0);
            s3.setTorqueEnabled(0);
            s4.setTorqueEnabled(0);
            s5.setTorqueEnabled(0);
            s6.setTorqueEnabled(0);
            for(int i=0;i<10000;i++){
            storeData(s1.getCurrentPosition());
            pos_array[0][i]=s1.getCurrentPosition();
            pos_array[1][i]=s2.getCurrentPosition();
            pos_array[2][i]=s3.getCurrentPosition();
            pos_array2[0][i]=s4.getCurrentPosition();
            pos_array2[1][i]=s5.getCurrentPosition();
            pos_array2[2][i]=s6.getCurrentPosition();
            usleep(1000);
            }
            cout << "\r Capture Finish \r" << endl;
            break;
            
            case 47: // kp /
            s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
            for(int i=0;i<10000;i++){
            int input = readData(i);
            s1.setGoalPosition(input);
            s2.setGoalPosition(pos_array[1][i]);
            s3.setGoalPosition(pos_array[2][i]);
            usleep(1000);
            }
            
            for(int i=0;i<10000;i++){
            s4.setGoalPosition(1023-pos_array[0][i]);
            s5.setGoalPosition(1023-pos_array[1][i]);
            s6.setGoalPosition(1023-pos_array[2][i]);
            usleep(1000);
            }
            break;
            
            case 91: // kp [
            s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
            for(int i=0;i<10000;i++){
            s1.setGoalPosition(pos_array[0][i]);
            s2.setGoalPosition(pos_array[1][i]);
            s3.setGoalPosition(pos_array[2][i]);
            if(!mirrorMode){
            s4.setGoalPosition(pos_array2[0][i]);
            s5.setGoalPosition(pos_array2[1][i]);
            s6.setGoalPosition(pos_array2[2][i]);
            }
            else {
            s4.setGoalPosition(1023-pos_array[0][i]);
            s5.setGoalPosition(1023-pos_array[1][i]);
            s6.setGoalPosition(1023-pos_array[2][i]);
            }
            usleep(1000);
            }
            break;
            
            
            case 93: // kp ]
            s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
            for(int i=10000;i>=0;i--){
            s1.setGoalPosition(pos_array[0][i]);
            s2.setGoalPosition(pos_array[1][i]);
            s3.setGoalPosition(pos_array[2][i]);
            if(!mirrorMode){
            s4.setGoalPosition(pos_array2[0][i]);
            s5.setGoalPosition(pos_array2[1][i]);
            s6.setGoalPosition(pos_array2[2][i]);
            }
            else {
            s4.setGoalPosition(1023-pos_array[0][i]);
            s5.setGoalPosition(1023-pos_array[1][i]);
            s6.setGoalPosition(1023-pos_array[2][i]);
            }
            usleep(1000);
            }
            break;
            
            case 92: // kp \
            
            {
              s1.setTorqueEnabled(0);
              s2.setTorqueEnabled(0);
              s3.setTorqueEnabled(0);
              s4.setTorqueEnabled(1);
              s5.setTorqueEnabled(1);
              s6.setTorqueEnabled(1);
              for(int i=0; i <10000;i++){
              int q = s1.getCurrentPosition();
              int w = s2.getCurrentPosition();
              int e = s3.getCurrentPosition();
                s4.setGoalPosition(1023-q);
                s5.setGoalPosition(1023-w);
                s6.setGoalPosition(1023-e);
                usleep(1000);
                }
                }
            break;
            
            case 'q':
                cout << s1.getCurrentPosition() << " " << s2.getCurrentPosition() << " " << s3.getCurrentPosition() << endl;
				cout << s4.getCurrentPosition() << " " << s5.getCurrentPosition() << " " << s6.getCurrentPosition() << endl;
            break;
            
            case 'w':
            if(readData(10)==2)cout << " YES " << endl;
            break;
            
			case 'e': // graceful exit
			s1.setMovingSpeed(100);
			s2.setMovingSpeed(100);
			s3.setMovingSpeed(100);
			s4.setMovingSpeed(100);
			s5.setMovingSpeed(100);
			s6.setMovingSpeed(100);
			usleep(100000);
            s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
			usleep(100000);
			s1.setGoalPosition(220);
            s2.setGoalPosition(845);
            s3.setGoalPosition(430);
			s4.setGoalPosition(800);
            s5.setGoalPosition(180);
            s6.setGoalPosition(590);
			usleep(3000000);
			system("/bin/stty cooked");
            cout << "\r"; // Get CLI cursor back to column 0
            cout << "> EXIT (key was: " << key << ")" << endl;
			s1.setTorqueEnabled(0);
            s2.setTorqueEnabled(0);
            s3.setTorqueEnabled(0);
            s4.setTorqueEnabled(0);
            s5.setTorqueEnabled(0);
            s6.setTorqueEnabled(0);
			usleep(100000);
            running = false;
            break;
			
            case 'a':
			s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
			usleep(100000);
			s1.setGoalPosition(512);
            s2.setGoalPosition(140);
            s3.setGoalPosition(150);
			s4.setGoalPosition(512);
            s5.setGoalPosition(512);
            s6.setGoalPosition(512);
            break;
            
            case 's':
			s1.setTorqueEnabled(1);
            s2.setTorqueEnabled(1);
            s3.setTorqueEnabled(1);
            s4.setTorqueEnabled(1);
            s5.setTorqueEnabled(1);
            s6.setTorqueEnabled(1);
			usleep(100000);
			s1.setGoalPosition(512);
            s2.setGoalPosition(512);
            s3.setGoalPosition(512);
			s4.setGoalPosition(512);
            s5.setGoalPosition(870);
            s6.setGoalPosition(880);
            break;
            
            
            
            
            case ' ':
				for(int i=0;i<=5;i++){
					for(int i=-125;i<=0;i+=3){
					IK(i,140);
					s2.setGoalPosition(targetangle[0]);
					s3.setGoalPosition(targetangle[1]);
					usleep(6000);
					}
					for(int i=0;i>=-40;i-=3){
					IK(i,.5*i+130);
					s2.setGoalPosition(targetangle[0]);
					s3.setGoalPosition(targetangle[1]);
					usleep(6000);
					}
					for(int i=-40;i>=-115;i-=3){
					IK(i,110);
					s2.setGoalPosition(targetangle[0]);
					s3.setGoalPosition(targetangle[1]);
					usleep(6000);
					}
					for(int i=-115;i>=-126;i-=3){
					IK(i,-2.6*i-193);
					s2.setGoalPosition(targetangle[0]);
					s3.setGoalPosition(targetangle[1]);
					usleep(6000);
					}
				}
             break;

        case 0x1b: // Escape
        default:
            system("/bin/stty cooked");
            cout << "\r"; // Get CLI cursor back to column 0
            cout << "> EXIT (key was: " << key << ")" << endl;
			s1.setTorqueEnabled(0);
            s2.setTorqueEnabled(0);
            s3.setTorqueEnabled(0);
            s4.setTorqueEnabled(0);
            s5.setTorqueEnabled(0);
            s6.setTorqueEnabled(0);
			usleep(100000);
            running = false;
            break;
            }
           }
    cout << endl << "======== EXITING ========" << endl;
			
        ctrl.disconnect();
        return EXIT_SUCCESS;
        }


/* ************************************************************************** */
