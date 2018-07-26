#include "sys.h"

#ifndef __UDP_DATA_STRUCT_H
#define __UDP_DATA_STRUCT_H

#define UDP_DATA_LENGTH 100

typedef struct udp_data {
  u8 pc_ip_udp[4];
  u8 stm32_ip_udp[4];
  u8 hardware_id_udp[4];
  float position_send_udp;
  float speed_send_udp;
  float current_send_udp;
  float temperature_send_udp;

  float acceleration_1_send_udp;  //?????6byte
  float acceleration_2_send_udp;
  float acceleration_3_send_udp;

  float angular_speed_1_send_udp;  //?????6byte
  float angular_speed_2_send_udp;
  float angular_speed_3_send_udp;

  float rpy_angle_r_send_udp;  //???RPY??6byte
  float rpy_angle_p_send_udp;
  float rpy_angle_y_send_udp;

	float position_recv_udp;
  float speed_recv_udp;
  float current_recv_udp;	
	
} udp_data, *udp_data_ptr;

#endif