#include "can_test/motor_driver.h"


void md_driver::initialize_md_driver(void){
  CAN_initialize(_250k);  //CAN bit_rate : 250kbps
}

void md_driver::send_RPM(short R_RPM, short L_RPM){

  BYTE RPM_vel_arr[8]={PID_PNT_VEL_CMD,1,0,0,1,0,0,0};

  //오른쪽 모터 : 1번모터(D2,D3) ,왼쪽모터 2번 모터(D5,D6)
  //int8_t D1 =1;         //2ch 제어기는 D1,D4가 0이 아니면 두 채널 모두 구동(??)

  BYTE D2=R_RPM & 0xff;        //Low data
  BYTE D3=R_RPM>>8 & 0xff;     //high data

  //BYTE D4=1;
  BYTE D5=L_RPM & 0xff;        //Low data
  BYTE D6=L_RPM>>8 & 0xff;     //high data
  //BYTE D7;

  RPM_vel_arr[2]=D2;
  RPM_vel_arr[3]=D3;
  RPM_vel_arr[5]=D5;
  RPM_vel_arr[6]=D6;

  CAN_write(RPM_vel_arr);

  r_rpm_g = R_RPM;
  l_rpm_g = L_RPM;
  //ROS_INFO("CAN_write");
}

/*
void read_Encoder(int *left_value, int *right_value){

  int r_enc,l_enc;
  BYTE *read_arr=nullptr;

  //CAN_REQ(PID_MONITOR,read_arr);   //오른쪽모터 = 1번모터일경우
  //r_enc = Byte2Int32(read_arr[4],read_arr[5],read_arr[6],read_arr[7]);

  //CAN_REQ(PID_MONITOR2,read_arr);  //왼쪽모터 = 1번모터일경우
  //l_enc = Byte2Int32(read_arr[4],read_arr[5],read_arr[6],read_arr[7]);

  //*left_value = l_enc;
  //*right_value = r_enc;

}

*/

void md_driver::Encoder_REQ(void){
    CAN_REQ(PID_MONITOR);
    CAN_REQ(PID_MONITOR2);
}
/*
struct Encoder_data read_Encoder(void){

  struct Encoder_data enc_data;


 // enc_data.R_posi=read_R_Encoder();
//enc_data.L_posi=read_L_Encoder();
  return enc_data;
}*/

struct Encoder_data md_driver::read_Encoder(void){

  struct CAN_data can_data;
  struct Encoder_data enc_data;
  can_data = CAN_read();

  if(can_data.pid == PID_MONITOR){

     enc_data.L_posi = Byte2Int32(can_data.data[4],can_data.data[5],can_data.data[6],can_data.data[7]);
  }
  else if(can_data.pid == PID_MONITOR2){

     enc_data.R_posi = Byte2Int32(can_data.data[4],can_data.data[5],can_data.data[6],can_data.data[7]);
  }
  else{
    ROS_WARN("Read_Encoder_failed 1");
    //return 0;
  }

  can_data = CAN_read();

  if(can_data.pid == PID_MONITOR){
     enc_data.L_posi = Byte2Int32(can_data.data[4],can_data.data[5],can_data.data[6],can_data.data[7]);
  }
  else if(can_data.pid == PID_MONITOR2){
     enc_data.R_posi = Byte2Int32(can_data.data[4],can_data.data[5],can_data.data[6],can_data.data[7]);
  }
  else{
    ROS_WARN("Read_Encoder_failed 2");
    //return 0;
  }

  return enc_data;

}

void md_driver::Torque_OFF(void){
  BYTE TQ_OFF[8]={5,0,0,0,0,0,0,0};  //자연정지 PID_TQ_OFF(5번) ,private??
  CAN_write(TQ_OFF);
  ROS_INFO("Motor_Torque_OFF");
}

void md_driver::Reset_ENC(void){
  BYTE reset[8] = {13,0,0,0,0,0,0,0};
  CAN_write(reset);
}



/*****************************************************************************
 * value(cmd_vel)값을 받아서 RPM값으로 변환하고 send_RPM 함수를 실행
 * 실제 사용하는 모터구동함수
 ****************************************************************************/

void md_driver::contol_vel(float *cmd_vel){
  float lin_vel = cmd_vel[LINEAR];
  float ang_vel = cmd_vel[ANGULAR];

  short R_wheel_RPM=0,L_wheel_RPM=0;  //signed 16 bit

  R_wheel_RPM = -1*(short)(GEAR_RATIO*30.0*((2*lin_vel) + (wheel_separation*ang_vel))/(2*wheel_radius*PI));
  L_wheel_RPM = (short)(GEAR_RATIO*30.0*((2*lin_vel) - (wheel_separation*ang_vel))/(2*wheel_radius*PI));

  send_RPM(R_wheel_RPM,L_wheel_RPM);

}
