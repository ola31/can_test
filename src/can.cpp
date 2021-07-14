#include "can_test/can.h"

int soc;
int read_can_port;

int open_port(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(soc < 0)
    {
        return (-1);
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);

    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
    {

        return (-1);
    }

    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {

        return (-1);
    }

    return 0;
}

int send_port(struct can_frame *frame)
{
    int retval;
   retval = write(soc, frame, sizeof(struct can_frame));
    if (retval != sizeof(struct can_frame))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

/* this is just an example, run in a thread */
/* not use this function*/
void read_port()
{
    struct can_frame frame_rd;
    int recvbytes = 0;

    read_can_port = 1;
    while(read_can_port)
    {
        struct timeval timeout = {1, 0};  //s, us
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(soc, &readSet);

        if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (!read_can_port)
            {
                break;
            }
            if (FD_ISSET(soc, &readSet))
            {
                recvbytes = read(soc, &frame_rd, sizeof(struct can_frame));
                if(recvbytes)
                {
                    printf("dlc = %d, data = %s\n", frame_rd.can_dlc,frame_rd.data);

                }
            }
        }

    }

}

int close_port()
{
    close(soc);
    return 0;
}

void CAN_initialize(int bit_rate_mode){
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
  string command_0 = "sudo slcand -o -c -s /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_1 = "sudo slcand -o -c -s1 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_2 = "sudo slcand -o -c -s2 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_3 = "sudo slcand -o -c -s3 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_4 = "sudo slcand -o -c -s4 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_5 = "sudo slcand -o -c -s5 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_6 = "sudo slcand -o -c -s6 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_7 = "sudo slcand -o -c -s7 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";
  string command_8 = "sudo slcand -o -c -s8 /dev/CAN0 can0 && sudo ifconfig can0 up && sudo ifconfig can0 txqueuelen 1000";

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

  for(i=0;i<5;i++){
    if(system(c) == 0){       //터미널에 명령 전달
      ROS_INFO("Set bit_rate %s",c2); //-s5
      break;
    }
    else
      ROS_WARN("CAN init Setting Failed");

  }

  for(i=0;i<5;i++){
    if(open_port("can0")==-1)
      ROS_WARN("CAN_initalize_Failed");

    else{
      ROS_INFO("initailze_CAN");
      break;
    }
  }
}



void CAN_write(BYTE data_array[]){

  struct can_frame frame;
  frame.can_id = 0xB7AC01 ;        //32bit
  frame.can_id |= CAN_EFF_FLAG;    //extended CAN mode FLAG
  frame.can_dlc=8;                 //8bit
  memcpy(frame.data,data_array,8); //copy (data_array)->(frame)

  if(send_port(&frame) == -1){
    ROS_WARN("CAN_write_fuction error");
  }
}


void CAN_REQ(BYTE R_PID){


//****************REQ***********************************//
  BYTE REQ_Arr[8] = {PID_REQ_PID_DATA, R_PID,0,0,0,0,0,0};
  CAN_write(REQ_Arr);
//****************REQ***********************************//

}

struct CAN_data CAN_read(void){

  struct can_frame frame_rd;
  struct CAN_data can_data;
  int result = read(soc, &frame_rd, sizeof(struct can_frame));
  if(result<1){
      ROS_WARN("CAN_read_failed");
      //return;
  }

  can_data.pid=frame_rd.data[0];
  int i;
  for(i=1;i<8;i++){
    can_data.data[i]=frame_rd.data[i];
  }
  return can_data;

}




/***********************************************************
 * 1바이트 D4,D5,D6,D7 데이터를 받아 원 데이터인 4바이트 정수를 만듬
 ***********************************************************/
int Byte2Int32(BYTE d4, BYTE d5, BYTE d6, BYTE d7)
{
  return ((int)d4 | (int)d5<<8 | (int)d6<<16 | (int)d7<<24);
}
