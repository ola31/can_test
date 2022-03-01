#include "can_test/can.h"

/* 클래스 생성자
 * 한 개의 USBtoCAN 장치에 대해 하나의 객체를 생성해주어야 한다. 즉 CANable 개수만큼 객체를 선언한다.   
 * port_name_   : 소켓 CAN 통신 포트이름 can0 등 원하는 이름으로 넣어주면 된다.  
 * device_name_ : usb_to_can 장치 포트이름. /dev 경로에서 인식된 포트 이름을 넣어준다. 예 : ttyACM0  
 */
CAN::CAN(string port_name_, string device_name_){
  this->port_name = port_name_;
  this->device_name = device_name_;
}

/* 클래스 소멸자
 */
CAN::~CAN(){
  close_port();
}

/* 소켓 CAN 통신의 통신 소켓을 생성하는 함수
 * port : can0 등 클래스 생성자에서 넣어준 포트 이름
 */
int CAN::open_port(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(soc < 0){
        return (-1);
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);

    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0){
        return (-1);
    }

    addr.can_ifindex = ifr.ifr_ifindex;
    fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        return (-1);
    }

    return 0;
}

/*
 * CAN 통신을 보내는(send) 함수
 * 이 함수에서 핵심은 write 함수 
 * *frame : CAN통신으로 send할 can_frame 구조체 변수의 포인터
 */
int CAN::send_port(struct can_frame *frame)
{
    int retval;
    retval = write(soc, frame, sizeof(struct can_frame));
    if (retval != sizeof(struct can_frame)){
      return (-1);
    }
    else{
      return (0);
    }
}

/* this is just an example, run in a thread */
/* not use this function*/
void CAN::read_port()
{
    struct can_frame frame_rd;
    int recvbytes = 0;

    read_can_port = 1;
    while(read_can_port){

      struct timeval timeout = {1, 0};  //s, us
      fd_set readSet;
      FD_ZERO(&readSet);
      FD_SET(soc, &readSet);

      if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0){

        if (!read_can_port){
          break;
        }
        if (FD_ISSET(soc, &readSet)){
          recvbytes = read(soc, &frame_rd, sizeof(struct can_frame));
          if(recvbytes){
             printf("dlc = %d, data = %s\n", frame_rd.can_dlc,frame_rd.data);
          }
        }
      }
    }

}

/* 통신 소켓을 닫는 함수(CAN 통신 종료)
 */
int CAN::close_port()
{
    close(soc);
    return 0;
}

/* USBtoCAN 장치를 사용해 CAN 통신을 하기 위한 초기 환경설정
 * 
 * 객체 생성시 초기화 한 포트 이름(can0 등)과 장치이름(ttyACM0 등)으로 
 * 터미널에 전달할 명령어 string을 만들어준다.
 * 
 * bit_rate_mode : 통신 속도 설정 관련 인자. 헤더파일에 각 통신 속도에 대한 enum이 정의되어 있다. 예 : _10k
 * 명령어의 앞부분은 111111 은 PC의 패스워드이다. sudo 명령을 사용하기 위해서 필요하다. (보안 측면에서 절대 좋은 방식은 아니지만...)  
 */
void CAN::CAN_initialize(int bit_rate_mode){
/*
  * (can bit-rate)
  * -s0 : 10k(bps)
  * -s1 : 20k
  * -s2 : 50k
  * -s3 : 100k
  * -s4 : 125k
  * -s5 : 250k
  * -s6 : 500k
  * -s7 : 750k
  * -s8 : 1M
  */

/*
 * Note
 * You have to replace '111111' to your linux Password.
 */

  string command_0 = "echo '111111' | sudo -S slcand -o -c -s0 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_1 = "echo '111111' | sudo -S slcand -o -c -s1 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";//"sudo slcand -o -c -s1 /dev/"+ port_name + " can1 && sudo ifconfig can1 up && sudo ifconfig can1 txqueuelen 1000";
  string command_2 = "echo '111111' | sudo -S slcand -o -c -s2 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_3 = "echo '111111' | sudo -S slcand -o -c -s3 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_4 = "echo '111111' | sudo -S slcand -o -c -s4 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_5 = "echo '111111' | sudo -S slcand -o -c -s5 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_6 = "echo '111111' | sudo -S slcand -o -c -s6 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_7 = "echo '111111' | sudo -S slcand -o -c -s7 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";
  string command_8 = "echo '111111' | sudo -S slcand -o -c -s8 /dev/"+ this->device_name + " " + this->port_name + " && sudo ifconfig "+this->port_name+" up && sudo ifconfig "+this->port_name+" txqueuelen 1000";

  string command = command_0; //초기화
  string bps_s = "10k"; //초기화

  switch (bit_rate_mode)
  {
  case 0:
    command = command_0;
    bps_s = "10k";
    break;
  case 1:
    command = command_1;
    bps_s = "20k";
    break;
  case 2:
    command = command_2;
    bps_s = "50k";
    break;
  case 3:
  command = command_3;
  bps_s = "100k";
  break;
  case 4:
    command = command_4;
    bps_s = "125k";
    break;
  case 5:
    command = command_5;
    bps_s = "250k";
    break;
  case 6:
    command = command_6;
    bps_s = "500k";
    break;
  case 7:
    command = command_7;
    bps_s = "750k";
    break;
  case 8:
    command = command_8;
    bps_s = "1M";
    break;

  default:
    command = command_5;
    bps_s = "10k";
    break;
  }

  const char *c = command.c_str();
  const char *c2 = bps_s.c_str();

  int i;
  int count = 0;

  for(i=0;i<5;i++){
    if(system(c) == 0){       //터미널에 명령 전달
      ROS_INFO("Set bit_rate %s",c2); //-s5
      ROS_INFO("%s",c);
      break;
    }
    else{
      count ++;
    }
  }
  if(count>4){
    ROS_ERROR("CAN init Setting Failed!!!");
  }


  const char *port_name_s = this->port_name.c_str();

  for(i=0;i<5;i++){

    if( open_port(port_name_s) == -1 ){            //port_s : can0, can1 etc...
      ROS_WARN("CAN_initalize_Failed");
    }
    else{
      ROS_INFO("initailze_CAN");
      break;
    }
  }

}


/* can_frame 구조체 변수의 id, 데이터 길이를 설정해주는 함수.
 * *can_frame   : id와 데이터 길이를 설정할 can_frame 구조체 변수의 포인터
 * CAN_id       : CAN 통신 id값
 * CAN_data_len : CAN 통신 데이터 길이(대부분 8)
 * is_ext_mode  : extended CAN 으로 보낼 것이라면 true를 넣어준다. 이 경우 CAN_id에 |= CAN_EFF_FLAG 를 붙여주도록 했다.  
 */
void CAN::set_can_frame(struct can_frame &canframe, u_int32_t CAN_id, u_int8_t CAN_data_len, bool is_ext_mode){

  canframe.can_id = CAN_id;
  canframe.can_dlc = CAN_data_len;

  if(is_ext_mode){
    canframe.can_id |= CAN_EFF_FLAG;    //extended CAN mode FLAG
  }
}

/* CAN 통신을 보내주는 함수. 
 * id와 데이터 길이를 설정한 can_frame 구조체 변수에, CAN 통신으로 보내고 싶은 데이터를 넣어서 send하는 함수
 * *frame       : CAN 통신으로 전송하고 싶은 can_frame 구조체 변수의 포인터
 * data_array[] : CAN 통신으로 보내고 싶은 데이터 배열. 주로 길이 8  
 */
void CAN::CAN_write(struct can_frame &frame, BYTE data_array[]){

  memcpy(frame.data,data_array,8); //copy (data_array)->(frame)

  if(send_port(&frame) == -1){
    ROS_WARN("CAN_write_fuction error");
  }
}

/* CAN 통신 받는 함수
 * 버퍼에 쌓인 CAN통신 패킷을 주워옴
 * 읽어들인 패킷을 can_frame 구조체로 반환(return)한다. 
 */
struct can_frame CAN::CAN_read(void){

  struct can_frame frame_rd;
  int result = read(soc, &frame_rd, sizeof(struct can_frame));
  if(result<1){
      ROS_WARN("CAN_read_failed");
  }

  return frame_rd;
}


