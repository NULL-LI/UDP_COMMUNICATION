#include "delay.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
//#include "key.h"
#include "LAN8720.h"
#include "lwip_comm.h"
#include "timer.h"
#include "usmart.h"
//#include "lcd.h"
#include "sram.h"
//#include "rtc.h"
//#include "beep.h"
#include "adc.h"
//#include "temperature.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "malloc.h"
#include "sram.h"
//#include "tcp_client_demo.h"
//#include "tcp_client_demo.h"
//#include "tcp_server_demo.h"
//#include "httpd.h"
#include "udp_demo.h"

#include "udp_data_struct.h"

// extern void Adc_Temperate_Init(void);	//??????????????

void lwip_test_ui(u8 mode) {
  u8 speed;
  u8 buf[30];
  // POINT_COLOR=RED;
  if (mode & 1 << 0) {
  }
  if (mode & 1 << 1) {
    //		LCD_Fill(30,110,lcddev.width,lcddev.height,WHITE);	//????
    //		LCD_ShowString(30,110,200,16,16,"lwIP Init Successed");
    printf("lwIP Init Successed\n");
    if (lwipdev.dhcpstatus == 2)
      sprintf((char *)buf, "DHCP IP:%d.%d.%d.%d \n", lwipdev.ip[0],
              lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);  //????IP??
    else
      sprintf((char *)buf, "Static IP:%d.%d.%d.%d \n", lwipdev.ip[0],
              lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);  //????IP??
    //		LCD_ShowString(30,130,210,16,16,buf);
    printf("%s\n", buf);
    speed = LAN8720_Get_Speed();  //????
    if (speed & 1 << 1)
      printf("Ethernet Speed:100M\n");
    else
      printf("Ethernet Speed:10M\n");
    //		if(speed&1<<1)LCD_ShowString(30,150,200,16,16,"Ethernet
    // Speed:100M"); 		else LCD_ShowString(30,150,200,16,16,"Ethernet
    // Speed:10M"); 		LCD_ShowString(30,170,200,16,16,"KEY0:TCP Server Test");
    //		LCD_ShowString(30,190,200,16,16,"KEY1:TCP Client Test");
    printf("UDP Test\n");
  }
}

int main(void) {
  u8 t;
  u8 key;
  delay_init(168);                                 //?????
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //???????????2
  uart_init(115200);                               //???????
  usmart_dev.init(84);                             //???USMART
  LED_Init();                                      // LED???
  //	KEY_Init();  			//?????
  //	LCD_Init(); 			//LCD???
  FSMC_SRAM_Init();  //不能删
  //	BEEP_Init();			//??????
  //	My_RTC_Init();  		//RTC???
  Adc_Init();  // ADC???
  //	Adc_Temperate_Init(); 	//??????????
  TIM3_Int_Init(999, 839);  // 100khz???,??1000?10ms
  mymem_init(SRAMIN);       //????????
  mymem_init(SRAMEX);       //????????
  mymem_init(SRAMCCM);      //???CCM???
  //	POINT_COLOR = RED; 		//????
  lwip_test_ui(1);  //??????UI

  while (lwip_comm_init() != 0) {
    delay_ms(1200);
  }

  lwip_test_ui(2);  //??????UI
                    //	httpd_init();	//HTTP???(????websever)
                    //  while (1) {
  printf("TEST\r\n\r\n\r\n");
  //    udp_demo_test();

  err_t err;
  struct udp_pcb *udppcb;    //定义一个TCP服务器控制块
  struct ip_addr rmtipaddr;  //远端ip地址

  u8 *tbuf;
  u8 res = 0;

  udp_demo_set_remoteip();  //先选择IP

  tbuf = mymalloc(SRAMIN, 200);  //申请内存
  if (tbuf == NULL) return 0;    //内存申请失败了,直接退出
  sprintf((char *)tbuf, "Local IP:%d.%d.%d.%d", lwipdev.ip[0], lwipdev.ip[1],
          lwipdev.ip[2], lwipdev.ip[3]);  //服务器IP
  printf("%s\n", tbuf);

  sprintf((char *)tbuf, "Remote IP:%d.%d.%d.%d", lwipdev.remoteip[0],
          lwipdev.remoteip[1], lwipdev.remoteip[2],
          lwipdev.remoteip[3]);  //远端IP
  printf("%s\n", tbuf);
  sprintf((char *)tbuf, "Remote Port:%d", UDP_DEMO_PORT);  //客户端端口号
  printf("%s\n", tbuf);
  printf("STATUS:Disconnected\n");
  udppcb = udp_new();
  if (udppcb)  //创建成功
  {
    IP4_ADDR(&rmtipaddr, lwipdev.remoteip[0], lwipdev.remoteip[1],
             lwipdev.remoteip[2], lwipdev.remoteip[3]);
    err = udp_connect(
        udppcb, &rmtipaddr,
        UDP_DEMO_PORT);  // UDP客户端连接到指定IP地址和端口号的服务器
    if (err == ERR_OK) {
      err = udp_bind(udppcb, IP_ADDR_ANY,
                     UDP_DEMO_PORT);  //绑定本地IP地址与端口号
      if (err == ERR_OK)              //绑定完成
      {
        udp_recv(udppcb, udp_demo_recv, NULL);  //注册接收回调函数
        printf("STATUS:Connected\n");
      } else
        res = 1;
    } else
      res = 1;
  } else
    res = 1;
  while (res == 0) {
    //		udp_demo_senddata(udppcb);
    //		printf("send\n");
    lwip_periodic_handle();
    delay_ms(1);
    t++;
    if (t == 200) {
      t = 0;
      LED0 = !LED0;
    }
  }
  udp_demo_connection_close(udppcb);
  myfree(SRAMIN, tbuf);

  printf("lwip_periodic_handle\r\n\r\n\r\n");
  lwip_periodic_handle();
  delay_ms(2);

	while(1)
	{
  lwip_periodic_handle();
  delay_ms(200);
	}
}
