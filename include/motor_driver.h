#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "can.h"

#define GEAR_RATIO 30

#define LINEAR 0
#define ANGULAR 1

#define PI 3.141593

#define wheel_separation 0.470
#define wheel_radius 0.150

#define PID_VER 1            //CAN통신 주고받기 가능 여부 테스트 위한 것
#define PID_PNT_VEL_CMD 207
#define PID_MONITOR 196      //모터 1의 모니터데이터(위치정보:D4,5,6,7)
#define PID_MONITOR2 201     //모터 2의 모니터데이터

struct Encoder_data{
  int R_posi;
  int L_posi;
};


void send_RPM(short R_RPM, short L_RPM);
struct Encoder_data read_Encoder(void);
void Encoder_REQ(void);
void Torque_OFF(void);
void contol_vel(float *cmd_vel);
void Reset_ENC(void);


#endif // MOTOR_DRIVER_H
