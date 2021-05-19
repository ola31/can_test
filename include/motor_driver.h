#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "can.h"

#define GEAR_RATIO 30

#define LINEAR 0
#define ANGULAR 1

#define PI 3.141593

#define wheel_separation 50.5
#define wheel_radius 14

#define PID_VER 1            //CAN통신 주고받기 가능 여부 테스트 위한 것
#define PID_PNT_VEL_CMD 207
#define PID_MONITOR 196      //모터 1의 모니터데이터(위치정보:D4,5,6,7)
#define PID_MONITOR2 201     //모터 2의 모니터데이터


void send_RPM(short R_RPM, short L_RPM);
void read_Encoder(int &left_value, int &right_value);
void Torque_OFF(void);
void contol_vel(float *cmd_vel);

#endif // MOTOR_DRIVER_H
