#include "can_test/can.h"

CAN::CAN(string port_name_, string device_name_){
  this->port_name = port_name_;
  this->device_name = device_name_;
}

CAN::~CAN(){
  close_port();
}


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

int CAN::close_port()
{
    close(soc);
    return 0;
}

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


void CAN::set_can_frame(struct can_frame &canframe, u_int32_t CAN_id, u_int8_t CAN_data_len, bool is_ext_mode){

  canframe.can_id = CAN_id;
  canframe.can_dlc = CAN_data_len;

  if(is_ext_mode){
    canframe.can_id |= CAN_EFF_FLAG;    //extended CAN mode FLAG
  }
}


void CAN::CAN_write(struct can_frame &frame, BYTE data_array[]){

  memcpy(frame.data,data_array,8); //copy (data_array)->(frame)

  if(send_port(&frame) == -1){
    ROS_WARN("CAN_write_fuction error");
  }
}


struct can_frame CAN::CAN_read(void){

  struct can_frame frame_rd;
  int result = read(soc, &frame_rd, sizeof(struct can_frame));
  if(result<1){
      ROS_WARN("CAN_read_failed");
  }

  return frame_rd;
}


