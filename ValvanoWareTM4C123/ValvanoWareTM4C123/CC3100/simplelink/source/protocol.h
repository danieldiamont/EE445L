/*
 * protocol.h - CC31xx/CC32xx Host Driver Implementation
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/*******************************************************************************\
*
*   FILE NAME:      protocol.h
*
*   DESCRIPTION:    Constant and data structure definitions and function
*                   prototypes for the SL protocol module, which implements
*                   processing of SimpleLink Commands.
*
*   AUTHOR:
*
\*******************************************************************************/

#ifndef _SL_PROTOCOL_TYPES_H_
#define _SL_PROTOCOL_TYPES_H_

/****************************************************************************
**
**  User I/F pools definitions
**
****************************************************************************/

/****************************************************************************
**
**  Definitions for SimpleLink Commands
**
****************************************************************************/
#include <stdint.h>

/* pattern for LE 8/16/32 or BE*/
#define H2N_SYNC_PATTERN     {0xBBDDEEFF,0x4321,0x34,0x12}
#define H2N_CNYS_PATTERN     {0xBBDDEEFF,0x8765,0x78,0x56}

#define H2N_DUMMY_PATTERN    (int32_t)0xFFFFFFFF
#define N2H_SYNC_PATTERN     (int32_t)0xABCDDCBA
#define SYNC_PATTERN_LEN     (int32_t)sizeof(int32_t)
#define UART_SET_MODE_MAGIC_CODE    (int32_t)0xAA55AA55
#define SPI_16BITS_BUG(pattern)     (int32_t)((int32_t)pattern & (int32_t)0xFFFF7FFF)
#define SPI_8BITS_BUG(pattern)      (int32_t)((int32_t)pattern & (int32_t)0xFFFFFF7F)



typedef struct
{
	int16_t Opcode;
	int16_t Len;
}_SlGenericHeader_t;


typedef struct
{
    int32_t  Long;
    int16_t  Short;
    int8_t  Byte1;
    int8_t  Byte2;
}_SlSyncPattern_t;

typedef _SlGenericHeader_t _SlCommandHeader_t;

typedef struct
{
    _SlGenericHeader_t  GenHeader;
    int8_t               TxPoolCnt;
    int8_t               DevStatus;
    int8_t               SocketTXFailure;
    int8_t               SocketNonBlocking;
}_SlResponseHeader_t;

#define _SL_RESP_SPEC_HDR_SIZE (sizeof(_SlResponseHeader_t) - sizeof(_SlGenericHeader_t))
#define _SL_RESP_HDR_SIZE       sizeof(_SlResponseHeader_t)
#define _SL_CMD_HDR_SIZE        sizeof(_SlCommandHeader_t)

#define _SL_RESP_ARGS_START(_pMsg) (((_SlResponseHeader_t *)(_pMsg)) + 1)

/* Used only in NWP! */
typedef struct
{
    _SlCommandHeader_t  sl_hdr;
    int8_t   func_args_start;
} T_SCMD;


#define WLAN_CONN_STATUS_BIT 0x01
#define EVENTS_Q_STATUS_BIT  0x02
#define PENDING_RCV_CMD_BIT  0x04
#define FW_BUSY_PACKETS_BIT  0x08

#define INIT_STA_OK   0x11111111
#define INIT_STA_ERR  0x22222222
#define INIT_AP_OK    0x33333333
#define INIT_AP_ERR   0x44444444
#define INIT_P2P_OK   0x55555555
#define INIT_P2P_ERR  0x66666666

/****************************************************************************
**  OPCODES
****************************************************************************/
#define SL_IPV4_IPV6_OFFSET                            ( 9 )
#define SL_OPCODE_IPV4							       ( 0x0 << SL_IPV4_IPV6_OFFSET )
#define SL_OPCODE_IPV6							       ( 0x1 << SL_IPV4_IPV6_OFFSET )

#define SL_SYNC_ASYNC_OFFSET                           ( 10 )
#define SL_OPCODE_SYNC							       (0x1 << SL_SYNC_ASYNC_OFFSET )
#define SL_OPCODE_SILO_OFFSET                           ( 11 )
#define SL_OPCODE_SILO_MASK                             ( 0xF << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_DEVICE                           ( 0x0 << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_WLAN                             ( 0x1 << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_SOCKET                           ( 0x2 << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_NETAPP                           ( 0x3 << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_NVMEM                            ( 0x4 << SL_OPCODE_SILO_OFFSET )
#define SL_OPCODE_SILO_NETCFG                           ( 0x5 << SL_OPCODE_SILO_OFFSET )

#define SL_FAMILY_SHIFT                            (0x4)
#define SL_FLAGS_MASK                              (0xF)

#define SL_OPCODE_DEVICE_INITCOMPLETE                               	0x0008
#define SL_OPCODE_DEVICE_STOP_COMMAND                               	0x8473
#define SL_OPCODE_DEVICE_STOP_RESPONSE                              	0x0473
#define SL_OPCODE_DEVICE_STOP_ASYNC_RESPONSE                        	0x0073
#define SL_OPCODE_DEVICE_DEVICEASYNCDUMMY                           	0x0063

#define SL_OPCODE_DEVICE_VERSIONREADCOMMAND	                            0x8470
#define SL_OPCODE_DEVICE_VERSIONREADRESPONSE	                        0x0470
#define SL_OPCODE_DEVICE_DEVICEASYNCFATALERROR                      	0x0078
#define SL_OPCODE_WLAN_WLANCONNECTCOMMAND                           	0x8C80
#define SL_OPCODE_WLAN_WLANCONNECTRESPONSE                          	0x0C80
#define SL_OPCODE_WLAN_WLANASYNCCONNECTEDRESPONSE                   	0x0880
#define SL_OPCODE_WLAN_P2P_DEV_FOUND                                    0x0830
#define SL_OPCODE_WLAN_CONNECTION_FAILED                                0x0831
#define SL_OPCODE_WLAN_P2P_NEG_REQ_RECEIVED                             0x0832

#define SL_OPCODE_WLAN_WLANDISCONNECTCOMMAND                        	0x8C81
#define SL_OPCODE_WLAN_WLANDISCONNECTRESPONSE                       	0x0C81
#define SL_OPCODE_WLAN_WLANASYNCDISCONNECTEDRESPONSE                	0x0881
#define SL_OPCODE_WLAN_WLANCONNECTEAPCOMMAND                        	0x8C82
#define SL_OPCODE_WLAN_WLANCONNECTEAPCRESPONSE                      	0x0C82
#define SL_OPCODE_WLAN_PROFILEADDCOMMAND                            	0x8C83
#define SL_OPCODE_WLAN_PROFILEADDRESPONSE                           	0x0C83
#define SL_OPCODE_WLAN_PROFILEGETCOMMAND                            	0x8C84
#define SL_OPCODE_WLAN_PROFILEGETRESPONSE                           	0x0C84
#define SL_OPCODE_WLAN_PROFILEDELCOMMAND                            	0x8C85
#define SL_OPCODE_WLAN_PROFILEDELRESPONSE                           	0x0C85
#define SL_OPCODE_WLAN_POLICYSETCOMMAND                             	0x8C86
#define SL_OPCODE_WLAN_POLICYSETRESPONSE                            	0x0C86
#define SL_OPCODE_WLAN_POLICYGETCOMMAND                             	0x8C87
#define SL_OPCODE_WLAN_POLICYGETRESPONSE                            	0x0C87
#define SL_OPCODE_WLAN_FILTERADD                                    	0x8C88
#define SL_OPCODE_WLAN_FILTERADDRESPONSE                            	0x0C88
#define SL_OPCODE_WLAN_FILTERGET                                    	0x8C89
#define SL_OPCODE_WLAN_FILTERGETRESPONSE                            	0x0C89
#define SL_OPCODE_WLAN_FILTERDELETE                                 	0x8C8A
#define SL_OPCODE_WLAN_FILTERDELETERESPOSNE                         	0x0C8A
#define SL_OPCODE_WLAN_WLANGETSTATUSCOMMAND                         	0x8C8F
#define SL_OPCODE_WLAN_WLANGETSTATUSRESPONSE                        	0x0C8F
#define SL_OPCODE_WLAN_STARTTXCONTINUESCOMMAND                      	0x8CAA
#define SL_OPCODE_WLAN_STARTTXCONTINUESRESPONSE                     	0x0CAA
#define SL_OPCODE_WLAN_STOPTXCONTINUESCOMMAND                       	0x8CAB
#define SL_OPCODE_WLAN_STOPTXCONTINUESRESPONSE                      	0x0CAB
#define SL_OPCODE_WLAN_STARTRXSTATCOMMAND                           	0x8CAC
#define SL_OPCODE_WLAN_STARTRXSTATRESPONSE                          	0x0CAC
#define SL_OPCODE_WLAN_STOPRXSTATCOMMAND                            	0x8CAD
#define SL_OPCODE_WLAN_STOPRXSTATRESPONSE                           	0x0CAD
#define SL_OPCODE_WLAN_GETRXSTATCOMMAND                             	0x8CAF
#define SL_OPCODE_WLAN_GETRXSTATRESPONSE                            	0x0CAF
#define SL_OPCODE_WLAN_POLICYSETCOMMANDNEW                          	0x8CB0
#define SL_OPCODE_WLAN_POLICYSETRESPONSENEW                         	0x0CB0
#define SL_OPCODE_WLAN_POLICYGETCOMMANDNEW                          	0x8CB1
#define SL_OPCODE_WLAN_POLICYGETRESPONSENEW                         	0x0CB1

#define SL_OPCODE_WLAN_SMART_CONFIG_START_COMMAND                   	0x8CB2
#define SL_OPCODE_WLAN_SMART_CONFIG_START_RESPONSE                  	0x0CB2
#define SL_OPCODE_WLAN_SMART_CONFIG_START_ASYNC_RESPONSE            	0x08B2
#define SL_OPCODE_WLAN_SMART_CONFIG_STOP_COMMAND                    	0x8CB3
#define SL_OPCODE_WLAN_SMART_CONFIG_STOP_RESPONSE                   	0x0CB3
#define SL_OPCODE_WLAN_SMART_CONFIG_STOP_ASYNC_RESPONSE             	0x08B3
#define SL_OPCODE_WLAN_SET_MODE                                         0x8CB4
#define SL_OPCODE_WLAN_SET_MODE_RESPONSE                                0x0CB4
#define SL_OPCODE_WLAN_CFG_SET                                          0x8CB5
#define SL_OPCODE_WLAN_CFG_SET_RESPONSE                                 0x0CB5
#define SL_OPCODE_WLAN_CFG_GET                                          0x8CB6
#define SL_OPCODE_WLAN_CFG_GET_RESPONSE                                 0x0CB6
#define SL_OPCODE_WLAN_STA_CONNECTED                                	0x082E
#define SL_OPCODE_WLAN_STA_DISCONNECTED                             	0x082F
#define SL_OPCODE_WLAN_EAP_PROFILEADDCOMMAND                            0x8C67
#define SL_OPCODE_WLAN_EAP_PROFILEADDCOMMAND_RESPONSE                   0x0C67 

#define SL_OPCODE_SOCKET_SOCKET                                     	0x9401
#define SL_OPCODE_SOCKET_SOCKETRESPONSE                             	0x1401
#define SL_OPCODE_SOCKET_CLOSE                                      	0x9402
#define SL_OPCODE_SOCKET_CLOSERESPONSE                              	0x1402
#define SL_OPCODE_SOCKET_ACCEPT                                     	0x9403
#define SL_OPCODE_SOCKET_ACCEPTRESPONSE                             	0x1403
#define SL_OPCODE_SOCKET_ACCEPTASYNCRESPONSE                        	0x1003
#define SL_OPCODE_SOCKET_ACCEPTASYNCRESPONSE_V6                     	0x1203
#define SL_OPCODE_SOCKET_BIND                                       	0x9404
#define SL_OPCODE_SOCKET_BIND_V6                                    	0x9604
#define SL_OPCODE_SOCKET_BINDRESPONSE                               	0x1404
#define SL_OPCODE_SOCKET_LISTEN                                     	0x9405
#define SL_OPCODE_SOCKET_LISTENRESPONSE                             	0x1405
#define SL_OPCODE_SOCKET_CONNECT                                    	0x9406
#define SL_OPCODE_SOCKET_CONNECT_V6                                 	0x9606
#define SL_OPCODE_SOCKET_CONNECTRESPONSE                            	0x1406
#define SL_OPCODE_SOCKET_CONNECTASYNCRESPONSE                       	0x1006
#define SL_OPCODE_SOCKET_SELECT                                     	0x9407
#define SL_OPCODE_SOCKET_SELECTRESPONSE                             	0x1407
#define SL_OPCODE_SOCKET_SELECTASYNCRESPONSE                        	0x1007
#define SL_OPCODE_SOCKET_SETSOCKOPT                                 	0x9408
#define SL_OPCODE_SOCKET_SETSOCKOPTRESPONSE                         	0x1408
#define SL_OPCODE_SOCKET_GETSOCKOPT                                 	0x9409
#define SL_OPCODE_SOCKET_GETSOCKOPTRESPONSE                         	0x1409
#define SL_OPCODE_SOCKET_RECV                                       	0x940A
#define SL_OPCODE_SOCKET_RECVASYNCRESPONSE                          	0x100A
#define SL_OPCODE_SOCKET_RECVFROM                                   	0x940B
#define SL_OPCODE_SOCKET_RECVFROMASYNCRESPONSE                      	0x100B
#define SL_OPCODE_SOCKET_RECVFROMASYNCRESPONSE_V6                   	0x120B
#define SL_OPCODE_SOCKET_SEND                                       	0x940C
#define SL_OPCODE_SOCKET_SENDTO                                     	0x940D
#define SL_OPCODE_SOCKET_SENDTO_V6                                  	0x960D
#define SL_OPCODE_SOCKET_TXFAILEDASYNCRESPONSE                      	0x100E
#define SL_OPCODE_SOCKET_SOCKETASYNCEVENT                               0x100F
#define SL_OPCODE_NETAPP_START_COMMAND                                  0x9C0A
#define SL_OPCODE_NETAPP_START_RESPONSE                                	0x1C0A
#define SL_OPCODE_NETAPP_NETAPPSTARTRESPONSE                        	0x1C0A
#define SL_OPCODE_NETAPP_STOP_COMMAND                               	0x9C61
#define SL_OPCODE_NETAPP_STOP_RESPONSE                              	0x1C61
#define SL_OPCODE_NETAPP_NETAPPSET                            	        0x9C0B
#define SL_OPCODE_NETAPP_NETAPPSETRESPONSE                    	        0x1C0B
#define SL_OPCODE_NETAPP_NETAPPGET                            	        0x9C27
#define SL_OPCODE_NETAPP_NETAPPGETRESPONSE                    	        0x1C27
#define SL_OPCODE_NETAPP_DNSGETHOSTBYNAME                           	0x9C20
#define SL_OPCODE_NETAPP_DNSGETHOSTBYNAMERESPONSE                   	0x1C20
#define SL_OPCODE_NETAPP_DNSGETHOSTBYNAMEASYNCRESPONSE              	0x1820
#define SL_OPCODE_NETAPP_DNSGETHOSTBYNAMEASYNCRESPONSE_V6           	0x1A20
#define SL_OPCODE_NETAPP_NETAPP_MDNS_LOOKUP_SERVICE                     0x9C71
#define SL_OPCODE_NETAPP_NETAPP_MDNS_LOOKUP_SERVICE_RESPONSE            0x1C72
#define SL_OPCODE_NETAPP_MDNSREGISTERSERVICE                            0x9C34
#define SL_OPCODE_NETAPP_MDNSREGISTERSERVICERESPONSE                    0x1C34
#define SL_OPCODE_NETAPP_MDNSGETHOSTBYSERVICE                           0x9C35
#define SL_OPCODE_NETAPP_MDNSGETHOSTBYSERVICERESPONSE                   0x1C35
#define SL_OPCODE_NETAPP_MDNSGETHOSTBYSERVICEASYNCRESPONSE              0x1835
#define SL_OPCODE_NETAPP_MDNSGETHOSTBYSERVICEASYNCRESPONSE_V6           0x1A35
#define SL_OPCODE_NETAPP_DNSGETHOSTBYADDR                           	0x9C26
#define SL_OPCODE_NETAPP_DNSGETHOSTBYADDR_V6                        	0x9E26
#define SL_OPCODE_NETAPP_DNSGETHOSTBYADDRRESPONSE                   	0x1C26
#define SL_OPCODE_NETAPP_DNSGETHOSTBYADDRASYNCRESPONSE              	0x1826
#define SL_OPCODE_NETAPP_PINGSTART                                  	0x9C21
#define SL_OPCODE_NETAPP_PINGSTART_V6                               	0x9E21
#define SL_OPCODE_NETAPP_PINGSTARTRESPONSE                          	0x1C21
#define SL_OPCODE_NETAPP_PINGREPORTREQUEST                          	0x9C22
#define SL_OPCODE_NETAPP_PINGREPORTREQUESTRESPONSE                  	0x1822
#define SL_OPCODE_NETAPP_PINGSTOP                                   	0x9C23
#define SL_OPCODE_NETAPP_PINGSTOPRESPONSE                           	0x1C23
#define SL_OPCODE_NETAPP_ARPFLUSH                                   	0x9C24
#define SL_OPCODE_NETAPP_ARPFLUSHRESPONSE                           	0x1C24
#define SL_OPCODE_NETAPP_IPACQUIRED                                 	0x1825
#define SL_OPCODE_NETAPP_IPACQUIRED_V6                              	0x1A25
#define SL_OPCODE_NETAPP_IPERFSTARTCOMMAND                          	0x9C28
#define SL_OPCODE_NETAPP_IPERFSTARTRESPONSE                         	0x1C28
#define SL_OPCODE_NETAPP_IPERFSTOPCOMMAND                           	0x9C29
#define SL_OPCODE_NETAPP_IPERFSTOPRESPONSE                          	0x1C29
#define SL_OPCODE_NETAPP_CTESTSTARTCOMMAND                          	0x9C2A
#define SL_OPCODE_NETAPP_CTESTSTARTRESPONSE                         	0x1C2A
#define SL_OPCODE_NETAPP_CTESTASYNCRESPONSE                         	0x182A
#define SL_OPCODE_NETAPP_CTESTSTOPCOMMAND                           	0x9C2B
#define SL_OPCODE_NETAPP_CTESTSTOPRESPONSE                          	0x1C2B
#define SL_OPCODE_NETAPP_IP_LEASED                                  	0x182C
#define SL_OPCODE_NETAPP_IP_RELEASED                                	0x182D
#define SL_OPCODE_NETAPP_HTTPGETTOKENVALUE                          	0x182E
#define SL_OPCODE_NETAPP_HTTPSENDTOKENVALUE                         	0x9C2F
#define SL_OPCODE_NETAPP_HTTPPOSTTOKENVALUE                         	0x1830
#define SL_OPCODE_NVMEM_FILEOPEN                                    	0xA43C
#define SL_OPCODE_NVMEM_FILEOPENRESPONSE                             	0x243C
#define SL_OPCODE_NVMEM_FILECLOSE                                    	0xA43D
#define SL_OPCODE_NVMEM_FILECLOSERESPONSE                           	0x243D
#define SL_OPCODE_NVMEM_FILEREADCOMMAND                              	0xA440
#define SL_OPCODE_NVMEM_FILEREADRESPONSE                            	0x2440
#define SL_OPCODE_NVMEM_FILEWRITECOMMAND                            	0xA441
#define SL_OPCODE_NVMEM_FILEWRITERESPONSE                           	0x2441
#define SL_OPCODE_NVMEM_FILEGETINFOCOMMAND                          	0xA442
#define SL_OPCODE_NVMEM_FILEGETINFORESPONSE                         	0x2442
#define SL_OPCODE_NVMEM_FILEDELCOMMAND                              	0xA443
#define SL_OPCODE_NVMEM_FILEDELRESPONSE                             	0x2443
#define SL_OPCODE_NVMEM_NVMEMFORMATCOMMAND                          	0xA444
#define SL_OPCODE_NVMEM_NVMEMFORMATRESPONSE                         	0x2444

#define SL_OPCODE_DEVICE_SETDEBUGLEVELCOMMAND                       	0x846A
#define SL_OPCODE_DEVICE_SETDEBUGLEVELRESPONSE                      	0x046A

#define SL_OPCODE_DEVICE_NETCFG_SET_COMMAND                 	        0x8432
#define SL_OPCODE_DEVICE_NETCFG_SET_RESPONSE                	        0x0432
#define SL_OPCODE_DEVICE_NETCFG_GET_COMMAND                 	        0x8433
#define SL_OPCODE_DEVICE_NETCFG_GET_RESPONSE                	        0x0433
/*  */
#define SL_OPCODE_DEVICE_SETUARTMODECOMMAND                         	0x846B
#define SL_OPCODE_DEVICE_SETUARTMODERESPONSE                        	0x046B
#define SL_OPCODE_DEVICE_SSISIZESETCOMMAND	                            0x846B
#define SL_OPCODE_DEVICE_SSISIZESETRESPONSE	                            0x046B

/*  */
#define SL_OPCODE_DEVICE_EVENTMASKSET                               	0x8464
#define SL_OPCODE_DEVICE_EVENTMASKSETRESPONSE                       	0x0464
#define SL_OPCODE_DEVICE_EVENTMASKGET                               	0x8465
#define SL_OPCODE_DEVICE_EVENTMASKGETRESPONSE                       	0x0465

#define SL_OPCODE_DEVICE_DEVICEGET                                  	0x8466
#define SL_OPCODE_DEVICE_DEVICEGETRESPONSE                              0x0466
#define SL_OPCODE_DEVICE_DEVICESET										0x84B7
#define SL_OPCODE_DEVICE_DEVICESETRESPONSE								0x04B7

#define SL_OPCODE_WLAN_SCANRESULTSGETCOMMAND                        	0x8C8C
#define SL_OPCODE_WLAN_SCANRESULTSGETRESPONSE                       	0x0C8C
#define SL_OPCODE_WLAN_SMARTCONFIGOPTSET                            	0x8C8D
#define SL_OPCODE_WLAN_SMARTCONFIGOPTSETRESPONSE                    	0x0C8D
#define SL_OPCODE_WLAN_SMARTCONFIGOPTGET                            	0x8C8E
#define SL_OPCODE_WLAN_SMARTCONFIGOPTGETRESPONSE                    	0x0C8E

#define SL_OPCODE_FREE_BSD_RECV_BUFFER                                  0xCCCB
#define SL_OPCODE_FREE_NON_BSD_READ_BUFFER                              0xCCCD


/* Rx Filters opcodes */
#define SL_OPCODE_WLAN_WLANRXFILTERADDCOMMAND                           0x8C6C
#define SL_OPCODE_WLAN_WLANRXFILTERADDRESPONSE                          0x0C6C
#define SL_OPCODE_WLAN_WLANRXFILTERSETCOMMAND                           0x8C6D
#define SL_OPCODE_WLAN_WLANRXFILTERSETRESPONSE                          0x0C6D
#define SL_OPCODE_WLAN_WLANRXFILTERGETSTATISTICSINFOCOMMAND             0x8C6E
#define SL_OPCODE_WLAN_WLANRXFILTERGETSTATISTICSINFORESPONSE            0x0C6E
#define SL_OPCODE_WLAN_WLANRXFILTERGETCOMMAND                           0x8C6F
#define SL_OPCODE_WLAN_WLANRXFILTERGETRESPONSE                          0x0C6F
#define SL_OPCODE_WLAN_WLANRXFILTERGETINFO                              0x8C70
#define SL_OPCODE_WLAN_WLANRXFILTERGETINFORESPONSE                      0x0C70

#include <stdint.h>
/******************************************************************************************/
/*   Device structs  */
/******************************************************************************************/
typedef int32_t InitStatus_t;


typedef struct
{
    int32_t Status;
}InitComplete_t;

typedef struct
{
  int16_t status;
  int16_t padding;

}_BasicResponse_t;

typedef struct
{
  int16_t Timeout;
  int16_t padding;
}_DevStopCommand_t;

typedef struct
{
  int32_t group;
  int32_t mask;
}_DevMaskEventSetCommand_t;

typedef _BasicResponse_t _DevMaskEventSetResponse_t;


typedef struct
{
  int32_t group;
}_DevMaskEventGetCommand_t;


typedef struct
{
  int32_t group;
  int32_t mask;
}_DevMaskEventGetResponse_t;


typedef struct
{
  int32_t group;
}_DevStatusGetCommand_t;


typedef struct
{
  int32_t group;
  int32_t status;
}_DevStatusGetResponse_t;

typedef struct
{
    int32_t  ChipId;
    int32_t  FwVersion[4];
    int8_t   PhyVersion[4];
}_Device_VersionReadResponsePart_t;

typedef struct
{
    _Device_VersionReadResponsePart_t part;
    int32_t                            NwpVersion[4];
    int16_t                            RomVersion;
    int16_t                            Padding;
}_Device_VersionReadResponseFull_t;


typedef struct
{
	int32_t BaudRate;
	int8_t  FlowControlEnable;
}_DevUartSetModeCommand_t;

typedef _BasicResponse_t _DevUartSetModeResponse_t;

/******************************************************/

typedef struct
{
    int8_t SsiSizeInBytes;
    int8_t Padding[3];
}_StellarisSsiSizeSet_t;

/*****************************************************************************************/
/*   WLAN structs */
/*****************************************************************************************/
#define MAXIMAL_PASSWORD_LENGTH					(64)

typedef struct{
	int8_t	SecType;
	int8_t	SsidLen;
    int8_t	Bssid[6];
	int8_t	PasswordLen;
}_WlanConnectCommon_t;

#define SSID_STRING(pCmd)       (char *)((_WlanConnectCommon_t *)(pCmd) + 1)
#define PASSWORD_STRING(pCmd)   (SSID_STRING(pCmd) + ((_WlanConnectCommon_t *)(pCmd))->SsidLen)

typedef struct{
	_WlanConnectCommon_t            Common;
	int8_t							UserLen;
	int8_t							AnonUserLen;
    int8_t   						CertIndex;
    int32_t  						EapBitmask;
}_WlanConnectEapCommand_t;

#define EAP_SSID_STRING(pCmd)       (char *)((_WlanConnectEapCommand_t *)(pCmd) + 1)
#define EAP_PASSWORD_STRING(pCmd)   (EAP_SSID_STRING(pCmd) + ((_WlanConnectEapCommand_t *)(pCmd))->Common.SsidLen)
#define EAP_USER_STRING(pCmd)       (EAP_PASSWORD_STRING(pCmd) + ((_WlanConnectEapCommand_t *)(pCmd))->Common.PasswordLen)
#define EAP_ANON_USER_STRING(pCmd)  (EAP_USER_STRING(pCmd) + ((_WlanConnectEapCommand_t *)(pCmd))->UserLen)


typedef struct
{
    int8_t	PolicyType;
    int8_t       Padding;
    int8_t	PolicyOption;
    int8_t	PolicyOptionLen;
}_WlanPoliciySetGet_t;


typedef struct{
	int32_t  minDwellTime;
	int32_t  maxDwellTime;
	int32_t  numProbeResponse;
	int32_t  G_Channels_mask;
	int32_t   rssiThershold;
	int32_t   snrThershold;
	int32_t   defaultTXPower;
	int16_t  intervalList[16];
}_WlanScanParamSetCommand_t;


typedef struct{
	int8_t	SecType;
	int8_t	SsidLen;
	int8_t	Priority;
	int8_t	Bssid[6];
    int8_t   PasswordLen;
    int8_t   WepKeyId;
}_WlanAddGetProfile_t;


typedef struct{
       _WlanAddGetProfile_t              Common;
       int8_t                             UserLen;
       int8_t                             AnonUserLen;
       int8_t                             CertIndex;
       int16_t                            padding;
       int32_t                            EapBitmask;
}_WlanAddGetEapProfile_t;




#define PROFILE_SSID_STRING(pCmd)       ((char *)((_WlanAddGetProfile_t *)(pCmd) + 1))
#define PROFILE_PASSWORD_STRING(pCmd)   (PROFILE_SSID_STRING(pCmd) + ((_WlanAddGetProfile_t *)(pCmd))->SsidLen)

#define EAP_PROFILE_SSID_STRING(pCmd)       (char *)((_WlanAddGetEapProfile_t *)(pCmd) + 1)
#define EAP_PROFILE_PASSWORD_STRING(pCmd)   (EAP_PROFILE_SSID_STRING(pCmd) + ((_WlanAddGetEapProfile_t *)(pCmd))->Common.SsidLen)
#define EAP_PROFILE_USER_STRING(pCmd)       (EAP_PROFILE_PASSWORD_STRING(pCmd) + ((_WlanAddGetEapProfile_t *)(pCmd))->Common.PasswordLen)
#define EAP_PROFILE_ANON_USER_STRING(pCmd)  (EAP_PROFILE_USER_STRING(pCmd) + ((_WlanAddGetEapProfile_t *)(pCmd))->UserLen)



typedef struct
{
	int8_t	index;
	int8_t	padding[3];
}_WlanProfileDelGetCommand_t;

typedef _BasicResponse_t _WlanGetNetworkListResponse_t;

typedef struct
{
	int8_t 	index;
	int8_t 	count;
	char 	padding[2];
}_WlanGetNetworkListCommand_t;




typedef struct
{
    int32_t  						groupIdBitmask;
    int8_t                           cipher;
    int8_t                           publicKeyLen;
    int8_t                           group1KeyLen;
    int8_t                           group2KeyLen;
}_WlanSmartConfigStartCommand_t;

#define SMART_CONFIG_START_PUBLIC_KEY_STRING(pCmd)       ((char *)((_WlanSmartConfigStartCommand_t *)(pCmd) + 1))
#define SMART_CONFIG_START_GROUP1_KEY_STRING(pCmd)       ((char *) (SMART_CONFIG_START_PUBLIC_KEY_STRING(pCmd) + ((_WlanSmartConfigStartCommand_t *)(pCmd))->publicKeyLen))
#define SMART_CONFIG_START_GROUP2_KEY_STRING(pCmd)       ((char *) (SMART_CONFIG_START_GROUP1_KEY_STRING(pCmd) + ((_WlanSmartConfigStartCommand_t *)(pCmd))->group1KeyLen))



typedef	struct
{
	int8_t	mode;
    int8_t   padding[3];
}_WlanSetMode_t;




typedef struct
{
    int16_t  Status;
    int16_t  ConfigId;
    int16_t  ConfigOpt;
    int16_t  ConfigLen;
}_WlanCfgSetGet_t;


/* ******************************************************************************/
/*     RX filters - Start  */
/* ******************************************************************************/
/*  -- 80 bytes */
typedef struct _WlanRxFilterAddCommand_t
{
	/*  -- 1 byte */
	SlrxFilterRuleType_t RuleType;
	/*  -- 1 byte */
	SlrxFilterFlags_t FilterFlags;
	/*  --  1 byte */
	SlrxFilterID_t FilterId;
	/*  --  1 byte */
	int8_t Padding;
	/*  -- 56 byte */
	SlrxFilterRule_t Rule;
	/*  --  12 byte ( 3 padding ) */
	SlrxFilterTrigger_t Trigger;
	/*  --  8 byte */
	SlrxFilterAction_t Action;
}_WlanRxFilterAddCommand_t;



/* -- 4 bytes */
typedef struct l_WlanRxFilterAddCommandReponse_t
{
	/*  -- 1 byte */
	SlrxFilterID_t FilterId;
	/* -- 1 Byte */
	int8_t          Status;
	/*  -- 2 byte */
	int8_t  Padding[2];

}_WlanRxFilterAddCommandReponse_t;



/*
 * \struct _WlanRxFilterSetCommand_t
 */
typedef struct _WlanRxFilterSetCommand_t
{
	int16_t InputBufferLength;
	/* 1 byte */
	SLrxFilterOperation_t RxFilterOperation;
	unsigned char Padding[1];
}_WlanRxFilterSetCommand_t;

/**
 * \struct _WlanRxFilterSetCommandReponse_t
 */
typedef struct _WlanRxFilterSetCommandReponse_t
{
	/* 1 byte */
	int8_t  Status;
	/* 3 bytes  */
	unsigned char Padding[3];

}_WlanRxFilterSetCommandReponse_t;

/**
 * \struct _WlanRxFilterGetCommand_t
 */
typedef struct _WlanRxFilterGetCommand_t
{
	int16_t OutputBufferLength;
	/* 1 byte  */
	SLrxFilterOperation_t RxFilterOperation;
	unsigned char Padding[1];
}_WlanRxFilterGetCommand_t;

/**
 * \struct _WlanRxFilterGetCommandReponse_t
 */
typedef struct _WlanRxFilterGetCommandReponse_t
{
	/* 1 byte  */
	int8_t  Status;
	/* 1 bytes  */
	unsigned char Padding;
	/* 2 byte  */
	int16_t OutputBufferLength;

}_WlanRxFilterGetCommandReponse_t;



/* ******************************************************************************/
/*     RX filters -- End  */
/* ******************************************************************************/

typedef struct
{
    int16_t status;
    int8_t  WlanRole;     /* 0 = station, 2 = AP */
    int8_t  Ipv6Enabled;
    int8_t  Ipv6DhcpEnabled;

    int32_t ipV6Global[4];
    int32_t ipV6Local[4];
    int32_t ipV6DnsServer[4];
    int8_t  Ipv6DhcpState;

}_NetappIpV6configRetArgs_t;


typedef struct
{
    int8_t  ipV4[4];
    int8_t  ipV4Mask[4];
    int8_t  ipV4Gateway[4];
    int8_t  ipV4DnsServer[4];
	int8_t  ipV4Start[4];
	int8_t  ipV4End[4];
}_NetCfgIpV4AP_Args_t;



typedef struct
{
  int16_t status;
  int8_t  MacAddr[6];
} _MAC_Address_SetGet_t;


typedef struct
{
    int16_t  Status;
    int16_t	ConfigId;
	int16_t	ConfigOpt;
    int16_t	ConfigLen;
}_NetCfgSetGet_t;

typedef struct
{
	int16_t  Status;
	int16_t  DeviceSetId;
	int16_t  Option;
	int16_t  ConfigLen;
}_DeviceSetGet_t;




/******************************************************************************************/
/*   Socket structs  */
/******************************************************************************************/

typedef struct
{
  int8_t Domain;
  int8_t Type;
  int8_t Protocol;
  int8_t Padding;
}_SocketCommand_t;


typedef struct
{
  int16_t statusOrLen;
  int8_t  sd;
  int8_t  padding;
}_SocketResponse_t;

typedef struct
{
  int8_t sd;
  int8_t family;
  int8_t padding1;
  int8_t padding2;
}_AcceptCommand_t;


typedef struct
{
  int16_t statusOrLen;
  int8_t sd;
  int8_t family;
  int16_t port;
  int16_t paddingOrAddr;
  int32_t address;
}_SocketAddrAsyncIPv4Response_t;

typedef struct
{
  int16_t statusOrLen;
  int8_t sd;
  int8_t family;
  int16_t port;
  int8_t address[6];
}_SocketAddrAsyncIPv6EUI48Response_t;
typedef struct
{
  int16_t statusOrLen;
  int8_t sd;
  int8_t family;
  int16_t port;
  int16_t paddingOrAddr;
  int32_t address[4];
}_SocketAddrAsyncIPv6Response_t;


typedef struct
{
  int16_t lenOrPadding;
  int8_t sd;
  int8_t FamilyAndFlags;
  int16_t port;
  int16_t paddingOrAddr;
  int32_t address;
}_SocketAddrIPv4Command_t;

typedef struct
{
  int16_t lenOrPadding;
  int8_t sd;
  int8_t FamilyAndFlags;
  int16_t port;
  int8_t address[6];
}_SocketAddrIPv6EUI48Command_t;
typedef struct
{
  int16_t lenOrPadding;
  int8_t sd;
  int8_t FamilyAndFlags;
  int16_t port;
  int16_t paddingOrAddr;
  int32_t address[4];
}_SocketAddrIPv6Command_t;

typedef union {
    _SocketAddrIPv4Command_t IpV4;
    _SocketAddrIPv6EUI48Command_t IpV6EUI48;
#ifdef SL_SUPPORT_IPV6
    _SocketAddrIPv6Command_t IpV6;
#endif
} _SocketAddrCommand_u;

typedef union {
    _SocketAddrAsyncIPv4Response_t IpV4;
    _SocketAddrAsyncIPv6EUI48Response_t IpV6EUI48;
#ifdef SL_SUPPORT_IPV6
    _SocketAddrAsyncIPv6Response_t IpV6;
#endif
} _SocketAddrResponse_u;

typedef struct
{
  int8_t sd;
  int8_t backlog;
  int8_t padding1;
  int8_t padding2;
}_ListenCommand_t;

typedef struct
{
  int8_t sd;
  int8_t padding0;
  int8_t padding1;
  int8_t padding2;
}_CloseCommand_t;


typedef struct
{
  int8_t nfds;
  int8_t readFdsCount;
  int8_t writeFdsCount;
  int8_t padding;
  int16_t readFds;
  int16_t writeFds;
  int16_t tv_usec;
  int16_t tv_sec;
}_SelectCommand_t;


typedef struct
{
  int16_t status;
  int8_t readFdsCount;
  int8_t writeFdsCount;
  int16_t readFds;
  int16_t writeFds;
}_SelectAsyncResponse_t;

typedef struct
{
  int8_t sd;
  int8_t level;
  int8_t optionName;
  int8_t optionLen;
}_setSockOptCommand_t;

typedef struct
{
  int8_t sd;
  int8_t level;
  int8_t optionName;
  int8_t optionLen;
}_getSockOptCommand_t;

typedef struct
{
  int16_t status;
  int8_t sd;
  int8_t optionLen;
}_getSockOptResponse_t;


typedef struct
{
  int16_t StatusOrLen;
  int8_t  sd;
  int8_t FamilyAndFlags;
}_sendRecvCommand_t;

/*****************************************************************************************
*   NETAPP structs
******************************************************************************************/


typedef _BasicResponse_t _NetAppStartStopResponse_t;

typedef struct
{
    int32_t  appId;
}_NetAppStartStopCommand_t;

typedef struct
{
    int16_t  Status;
    int16_t	AppId;
    int16_t	ConfigOpt;
    int16_t	ConfigLen;
}_NetAppSetGet_t;
typedef struct
{
    int16_t  port_number;
} _NetAppHttpServerGetSet_port_num_t;

typedef struct
{
    int8_t  auth_enable;
}_NetAppHttpServerGetSet_auth_enable_t;

typedef struct _sl_NetAppHttpServerGetToken_t
{
	int8_t	token_name_len;
	int8_t	padd1;
	int16_t	padd2;
}sl_NetAppHttpServerGetToken_t;

typedef struct _sl_NetAppHttpServerSendToken_t
{
	int8_t	token_value_len;
	int8_t	token_name_len;
	int8_t   token_name[MAX_TOKEN_NAME_LEN];
	int16_t	padd;
}sl_NetAppHttpServerSendToken_t;

typedef struct _sl_NetAppHttpServerPostToken_t
{
	int8_t post_action_len;
	int8_t token_name_len;
	int8_t token_value_len;
	int8_t padding;
}sl_NetAppHttpServerPostToken_t;


typedef struct
{
  int16_t Len;
  int8_t  family;
  int8_t  padding;
}_GetHostByNameCommand_t;

typedef struct
{
  int16_t status;
  int16_t padding;
  int32_t ip0;
  int32_t ip1;
  int32_t ip2;
  int32_t ip3;
}_GetHostByNameIPv6AsyncResponse_t;

typedef struct
{
  int16_t status;
  int8_t  padding1;
  int8_t  padding2;
  int32_t ip0;
}_GetHostByNameIPv4AsyncResponse_t;




typedef enum
{
    CTST_BSD_UDP_TX,
    CTST_BSD_UDP_RX,
    CTST_BSD_TCP_TX,
    CTST_BSD_TCP_RX,
    CTST_BSD_TCP_SERVER_BI_DIR,
    CTST_BSD_TCP_CLIENT_BI_DIR,
    CTST_BSD_UDP_BI_DIR,
    CTST_BSD_RAW_TX,
    CTST_BSD_RAW_RX,
    CTST_BSD_RAW_BI_DIR,
    CTST_BSD_SECURED_TCP_TX,
    CTST_BSD_SECURED_TCP_RX,
    CTST_BSD_SECURED_TCP_SERVER_BI_DIR,
    CTST_BSD_SECURED_TCP_CLIENT_BI_DIR
 }CommTest_e;

typedef struct _sl_protocol_CtestStartCommand_t
{
    int32_t Test;
    int16_t DestPort;
    int16_t SrcPort;
    int32_t DestAddr[4];
    int32_t PayloadSize;
    int32_t timeout;
    int32_t csEnabled;
    int32_t secure;
    int32_t rawProtocol;
    int8_t  reserved1[4];
}_CtestStartCommand_t;

typedef struct
{
  int8_t  test;
  int8_t  socket;
  int16_t  status;
  int32_t startTime;
  int32_t endTime;
  int16_t txKbitsSec;
  int16_t rxKbitsSec;
  int32_t outOfOrderPackets;
  int32_t missedPackets;
}_CtestAsyncResponse_t;

typedef struct
{
    int32_t pingIntervalTime;
    int16_t PingSize;
    int16_t pingRequestTimeout;
    int32_t totalNumberOfAttempts;
    int32_t flags;
    int32_t ip0;
    int32_t ip1OrPaadding;
    int32_t ip2OrPaadding;
    int32_t ip3OrPaadding;
}_PingStartCommand_t;

typedef struct
{
    int16_t status;
    int16_t rttMin;
    int16_t rttMax;
    int16_t rttAvg;
    int32_t numSuccsessPings;
    int32_t numSendsPings;
    int32_t testTime;
}_PingReportResponse_t;


typedef struct
{
    int32_t ip;
    int32_t gateway;
    int32_t dns;
}_IpV4AcquiredAsync_t;


typedef enum
{
  ACQUIRED_IPV6_LOCAL = 1,
  ACQUIRED_IPV6_GLOBAL
}IpV6AcquiredType_e;


typedef struct
{
    int32_t type;
    int32_t ip[4];
    int32_t gateway[4];
    int32_t dns[4];
}_IpV6AcquiredAsync_t;


typedef union
{
    _SocketCommand_t     EventMask;
    _sendRecvCommand_t   DeviceInit;
}_device_commands_t;

/*****************************************************************************************
*   FS structs
******************************************************************************************/

typedef struct
{
    int32_t FileHandle;
    int32_t Offset;
    int16_t Len;  
    int16_t Padding;  
}_FsReadCommand_t;

typedef struct
{
  int32_t Mode;
  int32_t Token;
}_FsOpenCommand_t;

typedef struct 
{
  int32_t FileHandle;
  int32_t Token;
}_FsOpenResponse_t;


typedef struct
{
  int32_t FileHandle;
  int32_t CertificFileNameLength;
  int32_t SignatureLen;
}_FsCloseCommand_t;


typedef _BasicResponse_t _FsReadResponse_t;
typedef _BasicResponse_t _FsDeleteResponse_t;
typedef _BasicResponse_t _FsCloseResponse_t;

typedef struct
{
    int16_t Status;
    int16_t flags;
    int32_t FileLen;
    int32_t AllocatedLen;
    int32_t Token[4];
}_FsGetInfoResponse_t;

typedef struct
{
    int8_t DeviceID;
    int8_t Padding[3];
}_FsFormatCommand_t;

typedef _BasicResponse_t _FsFormatResponse_t;

typedef struct
{
    int32_t Token;
}_FsDeleteCommand_t;

typedef   _FsDeleteCommand_t  _FsGetInfoCommand_t;

typedef struct
{
    int32_t FileHandle;
    int32_t Offset;
    int16_t Len;
    int16_t Padding;
}_FsWriteCommand_t;

typedef _BasicResponse_t _FsWriteResponse_t;



/*  Set Max Async Payload length depending on flavor (Tiny, Small, etc.)  */
#define SL_ASYNC_MAX_PAYLOAD_LEN        160  /* size must be aligned to 4  */
#define SL_ASYNC_MAX_MSG_LEN            (_SL_RESP_HDR_SIZE + SL_ASYNC_MAX_PAYLOAD_LEN)

#define RECV_ARGS_SIZE                  (sizeof(_SocketResponse_t))
#define RECVFROM_IPV4_ARGS_SIZE         (sizeof(_SocketAddrAsyncIPv4Response_t))
#define RECVFROM_IPV6_ARGS_SIZE         (sizeof(_SocketAddrAsyncIPv6Response_t))

#define SL_IPV4_ADDRESS_SIZE 			(sizeof(int32_t))
#define SL_IPV6_ADDRESS_SIZE 			(4 * sizeof(int32_t))

#endif /*  _SL_PROTOCOL_TYPES_H_  */
