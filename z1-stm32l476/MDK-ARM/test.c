#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_aoa.h"
#include "hw_config.h"
#include "task_ble.h"
#include "zb_common_data.h"
#include "hw_bd1.h"
#include "app_bd1.h"
#include "hw_bd2.h"
#include "app_bd2.h"
#include "app_nvram.h"

#define MAX_PACKET_LEN      242
#define XOR_HEAD_LEN      6
#define PACKET_DATALEN    5
#define PACKET_DATA         6
#define CMDERROR             -1


 struct Stu_Card_Packet{
	uint8_t uart_com;
	uint8_t packet_head ;
	uint8_t packet_prover;
	uint8_t rssi;
	uint8_t extpacket;
	uint8_t cmdtype;
	uint8_t *packet_data;
	uint8_t packet_xor;
	uint8_t pack_end;
	uint8_t extpacket_count ;
	uint8_t last_packet_len;
	uint16_t cmdlen;
} ;

struct Stu_Card_Packet stu_packet;




uint8_t  Stu_Card_Set_Packet_Prover(uint8_t version, uint8_t version2)
{
	return (version <<4) | version2;
}


uint8_t Stu_Card_Get_Xor_Date(uint8_t len,uint8_t *buf)
{

	uint8_t byteout = 0;
	uint16_t  i;
		for (i = 1; i <= len; i++) {
			byteout = (uint8_t) (byteout ^ buf[i]);
		}
		return byteout;	

}



int Stu_Card_Get_Date_Packet(uint8_t *src,uint8_t *dest)
{
	
	if(src[0]!= 0x86)
				return CMDERROR;
			



}
uint8_t Stu_Card_Set_Date_Packet(uint8_t *data)
{
		uint8_t packet_count = 0,totallen =0,i =0,totallen_bak=0;
		uint8_t extpacket =0;
		uint8_t sendbuf[MAX_PACKET_LEN+8];
		
		memset(sendbuf,0x00,MAX_PACKET_LEN+8);
		sendbuf[totallen] = stu_packet.packet_head;
		totallen+=1;
		sendbuf[totallen] = stu_packet.packet_prover;
		totallen+=1;
		sendbuf[totallen] = stu_packet.rssi;
		totallen+=1;
		printf("stu_packet.cmdlen = %d \r\n",stu_packet.cmdlen);	
	  totallen_bak = totallen;
		if(stu_packet.cmdlen<MAX_PACKET_LEN){
			stu_packet.extpacket_count =0;
		}else{
				if((stu_packet.cmdlen % MAX_PACKET_LEN) ==0){
					stu_packet.extpacket_count = stu_packet.cmdlen/MAX_PACKET_LEN;
					stu_packet.last_packet_len = 0;
				}else{
					stu_packet.extpacket_count = stu_packet.cmdlen/MAX_PACKET_LEN +1;
					stu_packet.last_packet_len = stu_packet.cmdlen % MAX_PACKET_LEN;
				}

		    }
	
				
		if(0 ==stu_packet.extpacket_count){
					
				
			sendbuf[totallen] = stu_packet.extpacket_count+1 ;
			totallen+=1;
			sendbuf[totallen] = stu_packet.cmdtype;
			totallen+=1;
			sendbuf[totallen] = stu_packet.cmdlen;
			printf("stu_packet.cmdlen = %d \r\n",stu_packet.cmdlen);
			totallen+=1;
			memcpy(&sendbuf[totallen],data,stu_packet.cmdlen);
			totallen+=stu_packet.cmdlen;
			stu_packet.packet_xor = Stu_Card_Get_Xor_Date(5+stu_packet.cmdlen,sendbuf);
			sendbuf[totallen] = stu_packet.packet_xor;
			totallen+=1;
			sendbuf[totallen] = stu_packet.pack_end;
			printf("sendbuf :%x %x %x %x %x %x %x %x %x %x %x %x %x\r\n",sendbuf[0],sendbuf[1],sendbuf[2],sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11]);
			// UART_WriteData(stu_packet.uart_com, sendbuf, totallen);
		}else{
				for(i = 0;i< stu_packet.extpacket_count;i++){
					totallen = totallen_bak;
				
					sendbuf[totallen] = 1<<7 | (i+1) ;
					totallen+=1;
					sendbuf[totallen] = stu_packet.cmdtype;
					totallen+=1;
					
			 if(0 == stu_packet.last_packet_len){
					sendbuf[totallen] = MAX_PACKET_LEN;
					totallen+=1;
					memcpy(&sendbuf[totallen],&data[i*MAX_PACKET_LEN],MAX_PACKET_LEN);
					totallen+=MAX_PACKET_LEN;
					stu_packet.packet_xor = Stu_Card_Get_Xor_Date(5+MAX_PACKET_LEN,sendbuf);
					sendbuf[totallen] = stu_packet.packet_xor;
					totallen+=1;
					sendbuf[totallen] = stu_packet.pack_end;
					totallen+=1;
				 printf("sendbuf00 :%x %x %x %x %x %x %x %x %x %x %x %x %x\r\n",sendbuf[0],sendbuf[1],sendbuf[2],sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11]);
				// UART_WriteData(stu_packet.uart_com, sendbuf, totallen);
					}else{
						 printf("i :%d \r\n",i);
							 if(i == (stu_packet.extpacket_count-1)){
							sendbuf[totallen] = stu_packet.last_packet_len;
							totallen+=1;
								 
								 
							memcpy(&sendbuf[totallen],&data[i*MAX_PACKET_LEN],stu_packet.last_packet_len);
							totallen+=stu_packet.last_packet_len;
								
							stu_packet.packet_xor = Stu_Card_Get_Xor_Date(5+stu_packet.last_packet_len,sendbuf);
							sendbuf[totallen] = stu_packet.packet_xor;
							totallen+=1;
							sendbuf[totallen] = stu_packet.pack_end;
							totallen+=1;
								
							printf("sendbuf11 :%x %x %x %x %x %x %x %x %x %x %x %x %x\r\n",sendbuf[0],sendbuf[1],sendbuf[2],sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11]);
							// UART_WriteData(stu_packet.uart_com, sendbuf, totallen);
		     }else{
				
					sendbuf[totallen] = MAX_PACKET_LEN;
					totallen+=1;
					memcpy(&sendbuf[totallen],&data[i*MAX_PACKET_LEN],MAX_PACKET_LEN);
					totallen+=MAX_PACKET_LEN;
					stu_packet.packet_xor = Stu_Card_Get_Xor_Date(5+MAX_PACKET_LEN,sendbuf);
					sendbuf[totallen] = stu_packet.packet_xor;
					totallen+=1;
					sendbuf[totallen] = stu_packet.pack_end;
					totallen+=1;
					printf("sendbuf22 :%x %x %x %x %x %x %x %x %x %x %x %x %x\r\n",sendbuf[0],sendbuf[1],sendbuf[2],sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11]);
				// UART_WriteData(stu_packet.uart_com, sendbuf, totallen);

		            }
		      }
		}
		
	}
}

void Stu_Card_init_packet(uint8_t hversion,uint8_t lversion,uint8_t rssi,uint8_t cmdtype,uint16_t datelen)
{
		stu_packet.uart_com = 2;
		stu_packet.packet_head = 0x86;
		stu_packet.packet_prover = Stu_Card_Set_Packet_Prover(hversion,lversion);
		stu_packet.rssi = rssi;
		stu_packet.cmdtype = cmdtype;
		stu_packet.cmdlen = datelen;
		//stu_packet.packet_data = data;
		stu_packet.pack_end = 0x76;

}

int Stu_Card_Power_On(void)
{
	uint8_t data;
	Stu_Card_init_packet(0x01,0x02,60,0x19,1);
	data = 0x00;
	Stu_Card_Set_Date_Packet(&data);

}

int Stu_Card_Power_On_ACK(void)
{
	uint8_t data;
	Stu_Card_init_packet(0x01,0x02,60,0x01,1);
	data = 0x00;
	Stu_Card_Set_Date_Packet(&data);

}

int Stu_Card_get_Server_Uid(uint8_t *src,uint8_t *dest)
{

   memcpy(dest,&src[6],src[5]);
	
}
int Stu_Card_Set_Server_Uid_Ack_To_Server(uint8_t *src,uint8_t *dest)
{

   memcpy(dest,&src[6],src[5]);
	
}
int Stu_Card_get_Uid_From_Server(uint8_t *src,uint8_t *dest)
{

   memcpy(dest,&src[6],src[5]);
	
}
int Stu_Card_Send_Uid_Ack_To_Server(uint8_t ackdata)
{
	uint8_t data;
	Stu_Card_init_packet(0x00,0x00,60,0x20,1);
	data = ackdata;
	Stu_Card_Set_Date_Packet(&data);

}
int Stu_Card_Set_Uid(void)
{
	uint8_t buf[4] = {0x1A,0xEC,0x3B,0x29};
	Stu_Card_init_packet(0x00,0x00,00,0x20,4);
  Stu_Card_Set_Date_Packet(buf);
	
}
int Stu_Card_Send_Uid_Ack(uint8_t ackdata)
{
	uint8_t data;
	Stu_Card_init_packet(0x01,0x02,60,0x20,1);
	data = ackdata;
	Stu_Card_Set_Date_Packet(&data);

}

int Stu_Card_Get_Version(void)
{
	//86 00 00 01 30 01 00 30 76
	uint8_t data;
	Stu_Card_init_packet(0x00,0x00,00,0x30,1);
	data = 0;
	Stu_Card_Set_Date_Packet(&data);
}
int Stu_Card_Set_Manual_Pair(void)
{
	//86 00 00 01 40 01 01 41 76
	uint8_t data;
	Stu_Card_init_packet(0x00,0x00,00,0x40,1);
	data = 1;
	Stu_Card_Set_Date_Packet(&data);
}
//86 00 00 01 01 01 00 01 76
int Stu_Card_Send_Pin_Ack(void)
{
	uint8_t data;
	Stu_Card_init_packet(0x00,0x00,00,0x01,1);
	data = 0;
	Stu_Card_Set_Date_Packet(&data);

}
//86 00 00 01 01 01 00 01 76
int Stu_Card_Stop_Answer_Ack(void)
{
	uint8_t data;
	Stu_Card_init_packet(0x00,0x00,00,0x01,1);
	data = 0;
	Stu_Card_Set_Date_Packet(&data);

}
//86 14 27 00 09 01 00 3B 76
int Stu_Card_Start_Answer(void)
{
	uint8_t data;
	Stu_Card_init_packet(0x01,0x04,0x27,0x09,1);
	data = 0;
	Stu_Card_Set_Date_Packet(&data);

}

int Stu_Card_Start_Answer_Data(void)
{
	uint8_t data[500];
	memset(data,0xf1,500);
	Stu_Card_init_packet(0x01,0x04,0x27,0x09,500);
	
	Stu_Card_Set_Date_Packet(data);

}

void Gps_Data_Resolution(uint8_t *data)
{
		uint32_t i =0,j=0;
		uint8_t temp[10];
		uint8_t temp1[11];
		for(i =0;i < 1024;i++){
			if(data[i] == '$'){
			    if(0 ==memcmp(&data[i+1],"GNGGA",5)){
					j=i+6;
					while(data[j]){
						if(data[j] == 'N'){
						  memcpy(temp,&data[j-11],10);
							printf("temp %s \r\n",temp);
						  //Get_Gps_Latitude(temp);
						 // UART_WriteData(0, temp, 10);
						  //break;
						  
						}else if(data[j] == 'E'){
						   memcpy(temp1,&data[j-12],11);
							printf("temp1 %s \r\n",temp1);
						   //UART_WriteData(0, tmp_buf, 11);
						   //Get_Gps_Longitude(temp);
						   //break;
						   
						}else if(data[j] == '\n' ||data[j] == '\r'){
							j = 0;
							break;
						}
						j++;
					}

				}

			}else{
				continue;
			}
		

		}

}