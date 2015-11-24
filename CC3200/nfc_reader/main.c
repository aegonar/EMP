//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
// Alejandro Gonzalez Araya
//*****************************************************************************

//*****************************************************************************
//                      Global Variables for Vector Table
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hw_types.h"

//#include "types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "simplelink.h"
//#include "protocol.h"
//#ifdef ewarm
//#include "FreeRTOS.h"
//#include "task.h"
//#endif

// free-rtos/TI-rtos include
#include "osi.h"

#include "interrupt.h"
#include "pin.h"
//#include "spi.h"
//#include "spi_for_trf.h"
#include "prcm.h"
#include "pinmux.h"
#include "gpio_if.h"
#include "uart_if.h"
#include "uart.h"
#include "interrupt.h"
#include "button_if.h"
#include "gpio.h"
#include "gpio_if.h"
#include "hw_gpio.h"
#include "timer.h"
#include "hw_timer.h"
#include "timer_if.h"
//#include "trf7970.h"
//#include "iso15693.h"
//#include "iso14443a.h"
//#include "network_if.h"
#include "common.h"
//#include "wlan.h"

//#include "trf7970BoosterPack.h"
//#include "email.h"
#include "demo_config.h"
//*****************************************************************************
#include "LCD.h"
//	char newLine[16];
	char line1[16];
	char line2[16];
//*****************************************************************************

#include "parse.h"
ParseClient client;
char parseData[1024];
short parseSuccess=0;
short Authorized=0;
short EnteringParking=0;
short LeavingParking=0;
short parseRetries=0;
//*****************************************************************************

#include <stdio.h>
//#include <string.h>
#include "jsmn.h"
//*****************************************************************************
int TEST_PING=0;
int Wifi_Error=0;
//*****************************************************************************
#define UartGetCharDev()        MAP_UARTCharGet(DEVICE)
#define UartPutCharDev(c)       MAP_UARTCharPut(DEVICE,c)

//unsigned long UART_STATUS;
unsigned short UARTDataFlag=0;
unsigned char UARTDataBuffer[256];
unsigned char tagData[256];
unsigned char UARTData;
unsigned long UARTDataCount=0;
unsigned short TAGEnable=0;

unsigned long UARTError=0;
//*****************************************************************************
#define PING_INTERVAL       1000    /* In msecs */
#define PING_TIMEOUT        3000    /* In msecs */
#define PING_PKT_SIZE       20      /* In bytes */
#define NO_OF_ATTEMPTS      3
#define PING_FLAG           0
//*****************************************************************************
static volatile unsigned long g_ulSysTickValue;
static volatile unsigned long g_ulBase;
static volatile unsigned long g_ulRefBase;
static volatile unsigned long g_ulRefTimerInts = 0;
static volatile unsigned long g_ulIntClearVector;
unsigned long g_ulTimerInts;
//int time = 10;
int TimmerBaseFlag=0;
int TimmerRefFlag=0;
//*****************************************************************************
short Sensor1=0;
short Sensor2=0;
//*****************************************************************************

#define SERVICE_NAME            "SimpleLink._EPM._tcp.local"

// Application specific status/error codes
typedef enum{
	// Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
	LAN_CONNECTION_FAILED = -0x7D0,
	INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
	DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

//    SERVER_GET_TIME_FAILED = -0x7D0,
//    DNS_LOOPUP_FAILED = SERVER_GET_TIME_FAILED  -1,

	STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

#define UART_PRINT              	Report
#define SPAWN_TASK_PRIORITY     	9
#define OSI_STACK_SIZE				4096 /* 2048 */
#define CONNECTION_TIMEOUT_COUNT  	10  /* 10sec */
#define AP_SSID_LEN_MAX          	33
#define TOKEN_ARRAY_SIZE          	6
#define STRING_TOKEN_SIZE        	10
#define SCAN_TABLE_SIZE          	20
#define MASTER 					 	8


/* Configuration Data Access options */
#define SF_DELETE_DATA_RECORD		(1)
#define SF_CREATE_DATA_RECORD		(2)
#define SF_WRITE_DATA_RECORD		(3)
#define SF_READ_DATA_RECORD 		(4)
#define SF_TEST_DATA_RECORD 		(5)

/* configuration data file structure */
typedef struct
{
	char DestEmailAddress[40];
	char SrcEmailAddress[40];
	char Password[40];
	char CardID[40];
	unsigned char Role;

}ConfigData_t;

/* NFC reader application state */
typedef enum
{
	INIT = 0,
	CREATE_CONFIG,
	SAVE_CONFIG,
	RESTORE_CONFIG,
	DEL_CONFIG,
	CONNECT_sta,
	CONNECT_ap,
	SNTP_GET_TIME_sta,
	CONNECT_TO_PARSE_sta,
	NFC_READER_ON_sta,
	NFC_READER_OFF_ap,
	SEND_PARSE_sta,
	HANDLE_PARSE_sta
}NFCAppState_e;
//*****************************************************************************
short NTPServerError=0;
#include "network_if.h"
#include "udma_if.h"
#include "rom.h"
#include "rom_map.h"

#define TIME2013                3565987200u      /* 113 years + 28 days(leap) */
#define YEAR2013                2013
#define SEC_IN_MIN              60
#define SEC_IN_HOUR             3600
#define SEC_IN_DAY              86400

#define SERVER_RESPONSE_TIMEOUT 50
#define GMT_DIFF_TIME_HRS       -4		//timezone
#define GMT_DIFF_TIME_MINS      0
//long GMT_DIFF_TIME_HRS=       -4;
//long GMT_DIFF_TIME_MINS=      0;

//#define SLEEP_TIME              80000000
//#define OSI_STACK_SIZE          2048

// Application specific status/error codes
//typedef enum{
//    // Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
//    SERVER_GET_TIME_FAILED = -0x7D0,
//    DNS_LOOPUP_FAILED = SERVER_GET_TIME_FAILED  -1,
//
//    STATUS_CODE_MAX = -0xBB8
//}e_AppStatusCodes;

unsigned short g_usTimerInts;
SlSecParams_t SecurityParams = {0};

//!    ######################### list of SNTP servers ##################################
//!    ##
//!    ##          hostname         |        IP       |       location
//!    ## -----------------------------------------------------------------------------
//!    ##   nist1-nj2.ustiming.org  | 165.193.126.229 |  Weehawken, NJ
//!    ##   nist1-pa.ustiming.org   | 206.246.122.250 |  Hatfield, PA
//!    ##   time-a.nist.gov         | 129.6.15.28     |  NIST, Gaithersburg, Maryland
//!    ##   time-b.nist.gov         | 129.6.15.29     |  NIST, Gaithersburg, Maryland
//!    ##   time-c.nist.gov         | 129.6.15.30     |  NIST, Gaithersburg, Maryland
//!    ##   ntp-nist.ldsbc.edu      | 198.60.73.8     |  LDSBC, Salt Lake City, Utah
//!    ##   nist1-macon.macon.ga.us | 98.175.203.200  |  Macon, Georgia
//!
//!    ##   For more SNTP server link visit 'http://tf.nist.gov/tf-cgi/servers.cgi'
//!    ###################################################################################
const char g_acSNTPserver[30] = "time-nw.nist.gov"; //Add any one of the above servers

// Tuesday is the 1st day in 2013 - the relative year
const char g_acDaysOfWeek2013[7][3] = {{"Tue"},
                                    {"Wed"},
                                    {"Thu"},
                                    {"Fri"},
                                    {"Sat"},
                                    {"Sun"},
                                    {"Mon"}};

const char g_acMonthOfYear[12][3] = {{"Jan"},
                                  {"Feb"},
                                  {"Mar"},
                                  {"Apr"},
                                  {"May"},
                                  {"Jun"},
                                  {"Jul"},
                                  {"Aug"},
                                  {"Sep"},
                                  {"Oct"},
                                  {"Nov"},
                                  {"Dec"}};

const char g_acNumOfDaysPerMonth[12] = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

const char g_acDigits[] = "0123456789";

struct
{
    unsigned long ulDestinationIP;
    int iSockID;
    unsigned long ulElapsedSec;
    short isGeneralVar;
    unsigned long ulGeneralVar;
    unsigned long ulGeneralVar1;
    char acTimeStore[30];
    char *pcCCPtr;
    unsigned short uisCCLen;
}g_sAppData;

SlSockAddr_t sAddr;
SlSockAddrIn_t sLocalAddr;


static long GetSNTPTime(long ucGmtDiffHr, long ucGmtDiffMins);

static short NTP_YEAR=0;
static short NTP_MONTH=0;
static short NTP_DAY=0;
static short NTP_HOUR=0;
static short NTP_MINUTE=0;
static short NTP_SECOND=0;


//*****************************************************************************
long GetSNTPTime(long ucGmtDiffHr, long ucGmtDiffMins)
{

    char cDataBuf[48];
    long lRetVal = 0;
    int iAddrSize;
    //
    // Send a query ? to the NTP server to get the NTP time
    //
    memset(cDataBuf, 0, sizeof(cDataBuf));
    cDataBuf[0] = '\x1b';

    sAddr.sa_family = AF_INET;
    // the source port
    sAddr.sa_data[0] = 0x00;
    sAddr.sa_data[1] = 0x7B;    // UDP port number for NTP is 123
    sAddr.sa_data[2] = (char)((g_sAppData.ulDestinationIP>>24)&0xff);
    sAddr.sa_data[3] = (char)((g_sAppData.ulDestinationIP>>16)&0xff);
    sAddr.sa_data[4] = (char)((g_sAppData.ulDestinationIP>>8)&0xff);
    sAddr.sa_data[5] = (char)(g_sAppData.ulDestinationIP&0xff);

    lRetVal = sl_SendTo(g_sAppData.iSockID,
                     cDataBuf,
                     sizeof(cDataBuf), 0,
                     &sAddr, sizeof(sAddr));
    if (lRetVal != sizeof(cDataBuf))
    {
        // could not send SNTP request
    	Report("SERVER_GET_TIME_FAILED");
//        ASSERT_ON_ERROR(SERVER_GET_TIME_FAILED);
    }

    //
    // Wait to receive the NTP time from the server
    //
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = 0;
    sLocalAddr.sin_addr.s_addr = 0;
    if(g_sAppData.ulElapsedSec == 0)
    {
        lRetVal = sl_Bind(g_sAppData.iSockID,
                (SlSockAddr_t *)&sLocalAddr,
                sizeof(SlSockAddrIn_t));
    }

    iAddrSize = sizeof(SlSockAddrIn_t);

    lRetVal = sl_RecvFrom(g_sAppData.iSockID,
                       cDataBuf, sizeof(cDataBuf), 0,
                       (SlSockAddr_t *)&sLocalAddr,
                       (SlSocklen_t*)&iAddrSize);
    ASSERT_ON_ERROR(lRetVal);

    //
    // Confirm that the MODE is 4 --> server
    //
    if ((cDataBuf[0] & 0x7) != 4)    // expect only server response
    {
         //ASSERT_ON_ERROR(SERVER_GET_TIME_FAILED);  // MODE is not server, abort
         Report("SERVER_GET_TIME_FAILED");
    }
    else
    {
        unsigned char iIndex;

        //
        // Getting the data from the Transmit Timestamp (seconds) field
        // This is the time at which the reply departed the
        // server for the client
        //
        g_sAppData.ulElapsedSec = cDataBuf[40];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[41];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[42];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[43];

        //
        // seconds are relative to 0h on 1 January 1900
        //
        g_sAppData.ulElapsedSec -= TIME2013;

        //
        // in order to correct the timezone
        //
        g_sAppData.ulElapsedSec += (ucGmtDiffHr * SEC_IN_HOUR);
        g_sAppData.ulElapsedSec += (ucGmtDiffMins * SEC_IN_MIN);
        g_sAppData.pcCCPtr = &g_sAppData.acTimeStore[0];

        //
        // day, number of days since beginning of 2013
        //
        g_sAppData.isGeneralVar = g_sAppData.ulElapsedSec/SEC_IN_DAY;
        memcpy(g_sAppData.pcCCPtr,
               g_acDaysOfWeek2013[g_sAppData.isGeneralVar%7], 3);
        g_sAppData.pcCCPtr += 3;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // month
        //
        g_sAppData.isGeneralVar %= 365;
        for (iIndex = 0; iIndex < 12; iIndex++)
        {
            g_sAppData.isGeneralVar -= g_acNumOfDaysPerMonth[iIndex];
            if (g_sAppData.isGeneralVar < 0)
                    break;
        }
        if(iIndex == 12)
        {
            iIndex = 0;
        }
        memcpy(g_sAppData.pcCCPtr, g_acMonthOfYear[iIndex], 3);

			Report("\tMONTH: %d\r\n", iIndex+1);
			NTP_MONTH=iIndex+1;

        g_sAppData.pcCCPtr += 3;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // date
        // restore the day in current month
        //
        g_sAppData.isGeneralVar += g_acNumOfDaysPerMonth[iIndex];
        g_sAppData.uisCCLen = itoa(g_sAppData.isGeneralVar + 1,
                                   g_sAppData.pcCCPtr);

			Report("\tDAY: %d\r\n",g_sAppData.isGeneralVar + 1,g_sAppData.pcCCPtr);
			NTP_DAY=g_sAppData.isGeneralVar + 1,g_sAppData.pcCCPtr;

        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // time
        //
        g_sAppData.ulGeneralVar = g_sAppData.ulElapsedSec%SEC_IN_DAY;

        // number of seconds per hour
        g_sAppData.ulGeneralVar1 = g_sAppData.ulGeneralVar%SEC_IN_HOUR;

        // number of hours
        g_sAppData.ulGeneralVar /= SEC_IN_HOUR;

			Report("\tHOUR: %d\r\n",g_sAppData.ulGeneralVar);
			NTP_HOUR=g_sAppData.ulGeneralVar;

        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = ':';

        // number of minutes per hour
        g_sAppData.ulGeneralVar = g_sAppData.ulGeneralVar1/SEC_IN_MIN;

			Report("\tMINUTE: %d\r\n",g_sAppData.ulGeneralVar);
			NTP_MINUTE=g_sAppData.ulGeneralVar;

        // number of seconds per minute
        g_sAppData.ulGeneralVar1 %= SEC_IN_MIN;

			Report("\tSECOND: %d\r\n",g_sAppData.ulGeneralVar1);
			NTP_SECOND=g_sAppData.ulGeneralVar1;

        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = ':';
        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar1,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = '\x20';


        //
        // year
        // number of days since beginning of 2013
        //
        g_sAppData.ulGeneralVar = g_sAppData.ulElapsedSec/SEC_IN_DAY;
        g_sAppData.ulGeneralVar /= 365;
        g_sAppData.uisCCLen = itoa(YEAR2013 + g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);

			Report("\tYEAR: %d\r\n", YEAR2013 + g_sAppData.ulGeneralVar, g_sAppData.pcCCPtr);
			NTP_YEAR=YEAR2013 + g_sAppData.ulGeneralVar, g_sAppData.pcCCPtr;

        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = '\0';

        Report("Response from SNTP server %s: %s\r\n",(char *)g_acSNTPserver, g_sAppData.acTimeStore);
    }
    return SUCCESS;
}
//*****************************************************************************
const char * GetDate(void){

    SlDateTime_t dateTime;
    memset(&dateTime, 0, sizeof(dateTime));

	_i8 configLen = sizeof(SlDateTime_t);
	_i8 configOpt = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME;
	//sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION,&configOpt, &configLen,(_u8 *)(&dateTime));
	sl_DevGet((_u8)SL_DEVICE_GENERAL_CONFIGURATION,(_u8 *)&configOpt, (_u8 *)&configLen,(_u8 *)(&dateTime));

    NTP_YEAR=dateTime.sl_tm_year;
    NTP_MONTH=dateTime.sl_tm_mon;
    NTP_DAY=dateTime.sl_tm_day;
    NTP_HOUR=dateTime.sl_tm_hour;
    NTP_MINUTE=dateTime.sl_tm_min;
	NTP_SECOND=dateTime.sl_tm_sec;

	static char dateString[19];

	sprintf(dateString,"%02d-%02d-%04d %02d:%02d:%02d", NTP_MONTH,  NTP_DAY, NTP_YEAR, NTP_HOUR, NTP_MINUTE, NTP_SECOND);
	return dateString;
}

void ReportDate(){
	Report("\t\t\t\t  [%s]\r\n", GetDate());
}

const char * GetTime(void){

    SlDateTime_t dateTime;
    memset(&dateTime, 0, sizeof(dateTime));

	_i8 configLen = sizeof(SlDateTime_t);
	_i8 configOpt = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME;
	//sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION,&configOpt, &configLen,(_u8 *)(&dateTime));
	sl_DevGet((_u8)SL_DEVICE_GENERAL_CONFIGURATION,(_u8 *)&configOpt, (_u8 *)&configLen,(_u8 *)(&dateTime));


    NTP_HOUR=dateTime.sl_tm_hour;
    NTP_MINUTE=dateTime.sl_tm_min;
	NTP_SECOND=dateTime.sl_tm_sec;

	static char dateString[19];

	sprintf(dateString,"%02d:%02d:%02d", NTP_HOUR, NTP_MINUTE, NTP_SECOND);
	return dateString;
}

void ReportTime(){
	Report("\t\t\t\t  [%s]\r\n", GetTime());
}
//===============================================================
//********** GLOBAL VARIABLES -- Application specific **********/
//===============================================================

// initialize configuration data
ConfigData_t g_ConfigData =
{
		DESTINATION_EMAIL_ADDRESS, \
		SOURCE_EMAIL_ADDRESS, \
		EMAIL_PASSWORD, \
		MASTER_UID, \
		INIT_ROLE
};



//===============================================================
/********** GLOBAL VARIABLES Application              **********/
//===============================================================
unsigned char g_reset = 0; // 1- when SW3 is pushed, used as indication for flushing configuration data stored in serial flash
unsigned char g_init = 1; // innitially set to 1 indicating that configuration has to be read from serial flash
unsigned int  g_uiNFCAppState = INIT;

//===============================================================
/********** GLOBAL VARIABLES TRF7970A **********/
//===============================================================
/*
u08_t buf[300];					// TX/RX BUFFER FOR TRF7970A
u08_t g_uid[300] = "none";		// used for coping card ID
char g_tag_content[600]; 		// used for saving a content of TAG buffer
char g_block_content[200];      // used for saving a content of single/multiple block(s)

u08_t g_rssi[10];
u08_t g_tag_found = 0;          // 0->no tag found
								// 1- ISO15693 tag found
								// 2- ISO14443A tag found
								// 8 - MASTER
u08_t Tag_Count;
u08_t i_reg = 0x01;             // INTERRUPT REGISTER
u08_t irq_flag = 0x00;
u08_t rx_error_flag = 0x00;
s08_t rxtx_state = 1;           // USED FOR TRANSMIT RECEIVE BYTE COUNT
u08_t host_control_flag = 0;
u08_t stand_alone_flag = 1;

int g_tag_count;                 // Tag counter
char g_tag_count_str[10];        // string representation of tag counter	*/
//===============================================================
//********** GLOBAL VARIABLES -- SimpleLink *********************/
//===============================================================
char 			g_APSsidName[33] = AP_SSID_NAME;    // AP SSID name
unsigned char   g_ucConnectionSSID[SSID_LEN_MAX+1]; //Connection SSID
unsigned char   g_ucConnectionBSSID[BSSID_LEN_MAX]; //Connection BSSID

unsigned char   g_ulStatus = 0;
unsigned long  	g_ulStaIp = 0; 						// STA IP number
unsigned long  	g_ulGatewayIP = 0; 					//Network Gateway IP address
unsigned long  	g_ulAcquiredIP = 0;
unsigned long  	g_ulPingPacketsRecv = 0;


SlSecParams_t g_SecParams={0,0,0};					// defult secure params, no security

//char g_pcEmailmessage[SMTP_BUF_LEN];   				// Email message buffer
long g_isFileHandle;								// Configuration Record File Handler


// HTTP server tokens variables

unsigned char AEA_POST_token[] = "__SL_P_AEA"; // Admin Email Address
unsigned char AEA_GET_token[]  = "__SL_G_AEA";

unsigned char ACI_POST_token[] = "__SL_P_ACI"; // Admin Card ID
unsigned char ACI_GET_token[]  = "__SL_G_ACI";

unsigned char STA_POST_token[] = "__SL_P_STA"; // AP/Station mode post token
unsigned char STS_GET_token[]  = "__SL_G_STS"; // Station Status (selected/not selected) get token
unsigned char APS_GET_token[]  = "__SL_G_APS"; // AP mode status (selected/not selected) get token
const char SELECTED[] 		= "selected";
const char NOT_SELECTED[] 	= "not_selected";

unsigned char SEP_GET_token[]   = "__SL_G_SEP"; // Gmail Source Email Password
unsigned char SEP_POST_token[]  = "__SL_P_SEP";

unsigned char FUN_GET_token[]  = "__SL_G_FUN"; // First user name
unsigned char SUN_GET_token[]  = "__SL_G_SUN"; // Second user name
unsigned char COM_GET_token[]  = "__SL_G_CMP"; // Company name
unsigned char TEL_GET_token[]  = "__SL_G_TEL"; // Telephone


unsigned char FUN_POST_token[]  = "__SL_P_FUN";
unsigned char SUN_POST_token[]  = "__SL_P_SUN";
unsigned char COM_POST_token[]  = "__SL_P_CMP";
unsigned char TEL_POST_token[]  = "__SL_P_TEL";


unsigned char SEA_POST_token[] = "__SL_P_SEA"; // Source gmail address
unsigned char DEA_POST_token[] = "__SL_P_DEA"; // Destination email address
unsigned char SND_POST_token[] = "__SL_P_SND"; // Reader state (on/off)
unsigned char SEA_GET_token[]  = "__SL_G_SEA";
unsigned char DEA_GET_token[]  = "__SL_G_DEA";

unsigned char CCN_GET_token[]  = "__SL_G_CCN"; // Card count
unsigned char CCN_POST_token[]  = "__SL_P_CCN"; // Card count

unsigned char CCI_GET_token[]  = "__SL_G_CCI"; // Tag ID
unsigned char CCI_POST_token[]  = "__SL_P_CCI"; // Tag ID


//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
static void BoardInit(void);
//void EmailHandleERROR(long error, char * servermessage);
void DisplayBanner(char * AppName);
//void A2CNTIntHandler (void);
void GPIOs2IntHandler(void);
void GPIOs3IntHandler(void);

void ISR_SENSORS(void);

//static void UARTInit(void);
//static void SPIInit(void);
long Network_IF_GetHostIP( char* pcHostName,unsigned long * pDestinationIP );
static int PingTest(unsigned long ulIpAddr);
long ManageConfigData(unsigned char Mode);
int ConfigureMode(int iMode);
//void EmailHandleERROR(long error, char * servermessage);
//long SendEmail(unsigned char iso);
void ElectronicParkingMeter ( void *pvParameters );


void
DisplayBanner(char * AppName)
{

	Report("\n\r");
	Report("\t\t *************************************************\n\r");
	Report("\t\t	    CC3200 %s\n\r", AppName);
	Report("\t\t *************************************************\n\r");
	Report("\n\r");
}

//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- Start
//*****************************************************************************

//*****************************************************************************
//
//! On Successful completion of Wlan Connect, This function triggers Connection
//! status to be set.
//!
//! \param  pSlWlanEvent pointer indicating Event type
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent)
{
	switch(pSlWlanEvent->Event)
	{
	case SL_WLAN_CONNECT_EVENT:
	{
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);

		//
		// Information about the connected AP (like name, MAC etc) will be
		// available in 'slWlanConnectAsyncResponse_t'-Applications
		// can use it if required
		//
		//  slWlanConnectAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.STAandP2PModeWlanConnected;
		//
		//

        // Copy new connection SSID and BSSID to global parameters
         memcpy(g_ucConnectionSSID,pSlWlanEvent->EventData.
                STAandP2PModeWlanConnected.ssid_name,
                pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
         memcpy(g_ucConnectionBSSID,
                pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
                SL_BSSID_LENGTH);
	}
	break;

	case SL_WLAN_DISCONNECT_EVENT:
	{
		slWlanConnectAsyncResponse_t*  pEventData = NULL;

		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

		// If the user has initiated 'Disconnect' request,
		//'reason_code' is SL_USER_INITIATED_DISCONNECTION
		if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
		{
			UART_PRINT("Device disconnected from the AP on application's "
					"request \n\r");
		}
		else
		{
			UART_PRINT("Device disconnected from the AP on an ERROR..!! \n\r");
		}

	}
	break;

	case SL_WLAN_STA_CONNECTED_EVENT:
	{
		// when device is in AP mode and any client connects to device cc3xxx
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);

		//
		// Information about the connected client (like SSID, MAC etc) will be
		// available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
		//

	}
	break;

	case SL_WLAN_STA_DISCONNECTED_EVENT:
	{
		// when client disconnects from device (AP)
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

		//
		// Information about the connected client (like SSID, MAC etc) will
		// be available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
		//
	}
	break;

	default:
	{
		UART_PRINT("[WLAN EVENT] Unexpected event \n\r");
	}
	break;
	}
}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
	switch(pNetAppEvent->Event)
	{
	case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
	case SL_NETAPP_IPV6_IPACQUIRED_EVENT:
	{
		SlIpV4AcquiredAsync_t *pEventData = NULL;

		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
		//Ip Acquired Event Data
		pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
		g_ulAcquiredIP = pEventData->ip;

		//Gateway IP address
		g_ulGatewayIP = pEventData->gateway;


	}
	break;

	case SL_NETAPP_IP_LEASED_EVENT:
	{
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

		g_ulStaIp = (pNetAppEvent)->EventData.ipLeased.ip_address;

		UART_PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d \n\r",
				SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
				SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));
	}
	break;

	case SL_NETAPP_IP_RELEASED_EVENT:
	{
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

		UART_PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d \n\r",
				SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
				SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));

	}
	break;

	default:
	{
		UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
				pNetAppEvent->Event);
	}
	break;
	}
}

//*****************************************************************************
//
//! \brief This function handles General Events
//!
//! \param[in]     pDevEvent - Pointer to General Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
	//
	// Most of the general errors are not FATAL are are to be handled
	// appropriately by the application
	//
	UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
			pDevEvent->EventData.deviceEvent.status,
			pDevEvent->EventData.deviceEvent.sender);
}



//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
	//
	// This application doesn't work w/ socket - Events are not expected
	//
	switch( pSock->Event )
	{
	case SL_SOCKET_TX_FAILED_EVENT:
#if 0
		switch( pSock->EventData.status )
#else
		switch( pSock->socketAsyncEvent.SockTxFailData.status)
#endif
		{
		case SL_ECLOSE:
			break;

		default:
			UART_PRINT("[SOCK ERROR] - TX FAILED : socket %d , reason"
					"(%d) \n\n",
#if 0
					pSock->EventData.sd, pSock->EventData.status);
#else
					pSock->socketAsyncEvent.SockTxFailData.sd, pSock->socketAsyncEvent.SockTxFailData.status);
#endif
		}
		break;

		default:
			UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);
	}
}






//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pSlHttpServerEvent Pointer indicating http server event
//! \param pSlHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse)
{
	_u8 strLenVal = 0;


	switch (pSlHttpServerEvent->Event)
	{
	case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
	{


		pSlHttpServerResponse->ResponseData.token_value.len = 0;

//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, SEP_GET_token, strlen((const char *)SEP_GET_token)) == 0)
//		{
//			strcpy ((char *) pSlHttpServerResponse->ResponseData.token_value.data,  g_ConfigData.Password);
//			strLenVal = strlen((const char*) g_ConfigData.Password);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, SEA_GET_token, strlen((const char *)SEA_GET_token)) == 0)
//		{
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, g_ConfigData.SrcEmailAddress);
//			strLenVal = strlen((const char*)g_ConfigData.SrcEmailAddress);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, DEA_GET_token, strlen((const char *)DEA_GET_token)) == 0)
//		{
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, g_ConfigData.DestEmailAddress);
//			strLenVal = strlen((const char*)g_ConfigData.DestEmailAddress);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, AEA_GET_token, strlen((const char *)AEA_GET_token)) == 0)
//		{
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, g_ConfigData.DestEmailAddress);
//			strLenVal = strlen((const char*)g_ConfigData.DestEmailAddress);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, ACI_GET_token, strlen((const char *)ACI_GET_token)) == 0)
//		{
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, g_ConfigData.CardID);
//			strLenVal = strlen((const char*)g_ConfigData.CardID);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, CCI_GET_token, strlen((const char *)CCI_GET_token)) == 0)
//		{
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, (char*) g_uid);
//			strLenVal = strlen((const char*)g_uid);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
//		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, CCN_GET_token, strlen((const char *)CCN_GET_token)) == 0)
//		{
//			sprintf(g_tag_count_str, "%d", g_tag_count);
//			strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, g_tag_count_str);
//			strLenVal = strlen((const char*)g_tag_count_str);
//			pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
//
//		}
		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, STS_GET_token, strlen((const char *)STS_GET_token)) == 0)
		{
			if (g_ConfigData.Role) {
				strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, SELECTED);
				strLenVal = strlen((const char*)SELECTED);
				pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
			} else {
				strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, NOT_SELECTED);
				strLenVal = strlen((const char*)NOT_SELECTED);
				pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
			}

		}
		if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, APS_GET_token, strlen((const char *)APS_GET_token)) == 0)
		{
			if (g_ConfigData.Role) {
				strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, NOT_SELECTED);
				strLenVal = strlen((const char*)NOT_SELECTED);
				pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
			} else {
				strcpy ((char*)pSlHttpServerResponse->ResponseData.token_value.data, SELECTED);
				strLenVal = strlen((const char*)SELECTED);
				pSlHttpServerResponse->ResponseData.token_value.len += strLenVal;
			}
		}
	}
	break;

	case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:
	{

		unsigned char *ptr = pSlHttpServerEvent->EventData.httpPostData.token_name.data;
//		unsigned char len;


//		if(memcmp(ptr, SEP_POST_token, strlen((const char *)SEP_POST_token)) == 0)
//		{
//			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
//			len = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
//			memcpy ((char *) g_ConfigData.Password, (const char *) ptr, len);
//			g_ConfigData.Password[len]= '\0';
//		}
//		if(memcmp(ptr, SEA_POST_token, strlen((const char *)SEA_POST_token)) == 0)
//		{
//			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
//			len = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
//			strncpy (g_ConfigData.SrcEmailAddress, (const char *) ptr, len);
//			g_ConfigData.SrcEmailAddress[len]= '\0';
//		}
//		if(memcmp(ptr, DEA_POST_token, strlen((const char *)DEA_POST_token)) == 0)
//		{
//			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
//			len = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
//			strncpy (g_ConfigData.DestEmailAddress, (const char *) ptr, len);
//			g_ConfigData.DestEmailAddress[len]= '\0';
//		}
//		if(memcmp(ptr, AEA_POST_token, strlen((const char *)AEA_POST_token)) == 0)
//		{
//			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
//			len = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
//			strncpy (g_ConfigData.DestEmailAddress, (const char *) ptr,len);
//			g_ConfigData.DestEmailAddress[len]= '\0';
//
//		}
//		if(memcmp(ptr, ACI_POST_token, strlen((const char *)ACI_POST_token)) == 0)
//		{
//			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
//			len = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
//			strncpy (g_ConfigData.CardID, (const char *) ptr,len);
//			g_ConfigData.CardID[len]= '\0';
//
//		}
		if(memcmp(ptr, STA_POST_token, strlen((const char *)STA_POST_token)) == 0)
		{
			ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
			strLenVal = strlen("Station");
			if (memcmp (ptr, "Station", strLenVal) == 0) {
				g_ConfigData.Role = 1;
			} else {
				g_ConfigData.Role = 0;

			}
		}


	}
	break;
	default:
		break;
	}
}




//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************


//*****************************************************************************
//
//! This function gets triggered when A2CNT interrupt occures
//!
//! \param none
//!
//! \return None
//!
//*****************************************************************************
/*
void A2CNTIntHandler (void){
	u08_t irq_status[4];
	TimerIntClear(TIMERA2_BASE, TIMER_A); // STOP_COUNTER;

	irq_flag = 0x02;

	Trf7970ReadIrqStatus(irq_status);

	*irq_status = *irq_status & 0xF7;                // set the parity flag to 0

	if(*irq_status == 0x00 || *irq_status == 0x80)
	{
		i_reg = 0x00;                                // timer interrupt
	}
	else
	{
		i_reg = 0x01;
	}

}
*/
//*****************************************************************************
//
//! GPIO Interrupt Handler for S3 button
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void ISR_SENSORS(void)
{
	MAP_GPIOIntClear(GPIOA0_BASE, 0x8);			//PIN_58
	MAP_GPIOIntClear(GPIOA0_BASE, 0x10);		//PIN_59

	//PIN_58
	if(GPIOPinRead(GPIOA0_BASE, 0x8) == 0x8){
	   //UART_PRINT("Sensor 1 ISR\n\r");

	    Sensor1=1;

		MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x8, GPIO_BOTH_EDGES);
		MAP_GPIOIntClear(GPIOA0_BASE, 0x8);
		MAP_IntPendClear(INT_GPIOA0);
	}

	//PIN_59
	if(GPIOPinRead(GPIOA0_BASE, 0x10) == 0x10){
		//UART_PRINT("Sensor 2 ISR\n\r");

		Sensor2=1;

		MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x10, GPIO_BOTH_EDGES);
		MAP_GPIOIntClear(GPIOA0_BASE, 0x10);
		MAP_IntPendClear(INT_GPIOA0);
	}
}

//*****************************************************************************

void GPIOs3IntHandler(void)
{
//
//	// GPIO 13
//	unsigned long ulPinState =  GPIOIntStatus(GPIOA1_BASE,1);
//
//	if(ulPinState & GPIO_PIN_4)
//	{
//		Button_IF_DisableInterrupt(SW3);
//		//do stuff here
//			g_ConfigData.Role = 0;
//			UART_PRINT("Maintenance mode enabled\n\r");
//			g_uiNFCAppState = CONNECT_ap;
//		//go to AP mode
//		Button_IF_EnableInterrupt(SW3);
//	}
//}
//
//
//void ISR_SW3()
//{

	MAP_GPIOIntClear(GPIOA1_BASE, 0x20);

	if( GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20){
		Button_IF_DisableInterrupt(SW3);
		UtilsDelay(5000);

		UART_PRINT("Maintenance mode enabled\n\r");

		UART_PRINT("Starting AP\n\r");

//		long lRetVal = ConfigureMode(ROLE_AP);
//		if (lRetVal == 2) {
//			g_uiNFCAppState= NFC_READER_OFF_ap;
//		}
		g_ConfigData.Role = 0;
		g_uiNFCAppState = CONNECT_ap;

		sprintf(line1, "%-16s", "Maintenance mode");		//format string
		sprintf(line2, "%-16s", "Starting AP...");		//format string
		writeLines(line1, line2);

		Button_IF_EnableInterrupt(SW3);
//		g_reset = 1;
//		//g_ConfigData.Role = 0;
//		g_ConfigData.Role = 0;

		//g_uiNFCAppState = CONNECT_ap;
	}

}
//*****************************************************************************
//
//! GPIO Interrupt Handler for S2 button
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void GPIOs2IntHandler(void)
{
//*****************************************************************************
//NFC Reader INT

	//void ISR_A2()
	//{
	if(GPIOPinRead(GPIOA2_BASE, 0x1) == 0x1){
		//IntMasterDisable();

	    //MAP_GPIOIntClear(GPIOA2_BASE, 0x1);		//PIN_07
	    UART_PRINT("P07 ISR in\n\r");

	    if(TAGEnable){
	    	UARTDataCount=0;
	    	Timer_IF_Stop(g_ulRefBase, TIMER_A);


			while(GPIOPinRead(GPIOA2_BASE, 0x1) == 0x1){


				if(UARTCharsAvail(DEVICE)){
					UARTData = (unsigned char)MAP_UARTCharGetNonBlocking(DEVICE);
					UARTDataBuffer[UARTDataCount] = UARTData;
					UARTDataCount++;
				}

	//	    	if(UARTDataCount > 16)
	//	    		UARTDataCount=0;

			}

			if(UARTDataCount != 16){
				Report("UART1 Buffer error.\r\n");
				Report("UART1 Buffer: %s\r\n", UARTDataBuffer);

				UARTDataCount=0;
				UARTError=1;
				UARTDataFlag=0;

				//MAP_GPIOIntClear(GPIOA2_BASE, 0x1);		//PIN_07
			}else{
				UART_PRINT("UART1 Buffer full.\n\r");
			//Report("Data: %s\n\r", UARTDataBuffer);
				UARTDataFlag=1;
				UARTError=0;
			}
		   // UARTDataCount=0;

			//IntMasterEnable();
			//Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);
	    }else{
	    	UART_PRINT("UART1 Data ignored.\n\r");
	    }

		MAP_GPIOIntTypeSet(GPIOA2_BASE, 0x1, GPIO_RISING_EDGE);
		MAP_GPIOIntClear(GPIOA2_BASE, 0x1);
		MAP_IntPendClear(INT_GPIOA2);
	    UART_PRINT("P07 ISR out\n\r");
	}

	    /*
		if(GPIOPinRead(GPIOA2_BASE, 0x1) == 0x1){
			UART_PRINT("ISR P07 1\n\r");
			MAP_UtilsDelay(400000);
		} else	if(GPIOPinRead(GPIOA2_BASE, 0x1) == 0){
			UART_PRINT("ISR P07 0\n\r");
			MAP_UtilsDelay(400000);
		}
		*/
	//}
//*****************************************************************************
//SW2 Button

//	    unsigned long ulPinState =  GPIOIntStatus(GPIOA2_BASE,1);
//
//	    	if(ulPinState & GPIO_PIN_6)
//	    	{
//	    		Button_IF_DisableInterrupt(SW2);
//	    		g_reset = 1;
//	    		g_ConfigData.Role = 0;
//
//	    		Button_IF_EnableInterrupt(SW2);
//
//	    	}

		MAP_GPIOIntClear(GPIOA2_BASE, 0x40);

		if( GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40){
				Button_IF_DisableInterrupt(SW2);
				UtilsDelay(5000);

				UART_PRINT("Dumping config!\n\r");

				g_reset = 1;
				g_ConfigData.Role = 0;
				g_uiNFCAppState = NFC_READER_OFF_ap;

				sprintf(line1, "%-16s", "Erasing Config");		//format string
				sprintf(line2, "%-16s", "Starting AP...");		//format string
				writeLines(line1, line2);

				Button_IF_EnableInterrupt(SW2);
		}


}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
	/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
	//
	// Set vector table base
	//
#if defined(ccs) || defined(gcc)
	MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
	MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
	//
	// Enable Processor
	//
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
}


//****************************************************************************
//
//! Confgiures the UART module
//!
//! \param none
//!
//! This function
//!    Configurates UART
//!
//! \return none.
//
//****************************************************************************
/*
static void UARTInit(void)
{
	// UART PRCM settings
	MAP_PRCMPeripheralReset(PRCM_UARTA0);

	//UART Initialization
	MAP_UARTConfigSetExpClk(CONSOLE,MAP_PRCMPeripheralClockGet(CONSOLE_PERIPH),
			UART_BAUD_RATE,(UART_CONFIG_WLEN_8 |
					UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	ClearTerm();

}
*/
//****************************************************************************
//
//! Confgiures the SPI module
//!
//! \param none
//!
//! This function
//!    Configurates SPI
//!
//! \return none.
//
//****************************************************************************
/*
static void SPIInit(void)
{
	MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
	MAP_PRCMPeripheralReset(PRCM_GSPI);
	MAP_SPIReset(GSPI_BASE);

	MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
			2000000,SPI_MODE_MASTER,SPI_SUB_MODE_1,
			(SPI_SW_CTRL_CS |
					SPI_3PIN_MODE |
					SPI_TURBO_OFF |
					SPI_CS_ACTIVELOW |
					SPI_WL_8));

	//
	// Enable SPI for communication
	//
	MAP_SPIEnable(GSPI_BASE);

}
*/
//****************************************************************************
//
//! Communicates with DNS server and gets host IP by name
//!
//! \param pcHostName [in] - host name
//! \param pDestinationIP [out] - pinter to host IP
//!
//!
//! \return 0 on success.
//
//****************************************************************************

long Network_IF_GetHostIP( char* pcHostName,unsigned long * pDestinationIP )
{
	long lStatus = 0;

	lStatus = sl_NetAppDnsGetHostByName((signed char *) pcHostName,
			strlen(pcHostName),
			pDestinationIP, SL_AF_INET);
	ASSERT_ON_ERROR(lStatus);

	UART_PRINT("Get Host IP succeeded.\n\rHost: %s IP: %d.%d.%d.%d \n\r",
			pcHostName, SL_IPV4_BYTE(*pDestinationIP,3),
			SL_IPV4_BYTE(*pDestinationIP,2),
			SL_IPV4_BYTE(*pDestinationIP,1),
			SL_IPV4_BYTE(*pDestinationIP,0));
	return lStatus;

}
//*****************************************************************************
//! \brief This function checks the LAN connection by pinging the gateway
//!
//! \param  ulIpAddr [in]
//!
//! \return 0 on success, negative error-code on error
//!
//*****************************************************************************


static int PingTest(unsigned long ulIpAddr)
{
	signed long           lRetVal = -1;
	SlPingStartCommand_t PingParams;
	SlPingReport_t PingReport;
	PingParams.PingIntervalTime = PING_INTERVAL;
	PingParams.PingSize = PING_PKT_SIZE;
	PingParams.PingRequestTimeout = PING_TIMEOUT;
	PingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
	PingParams.Flags = PING_FLAG;
	PingParams.Ip = ulIpAddr; /* Cleint's ip address */

	UART_PRINT("Running Ping Test...\n\r");
	/* Check for LAN connection */
	lRetVal = sl_NetAppPingStart((SlPingStartCommand_t*)&PingParams, SL_AF_INET,
			(SlPingReport_t*)&PingReport, NULL);
	ASSERT_ON_ERROR(lRetVal);

	g_ulPingPacketsRecv = PingReport.PacketsReceived;

	if (g_ulPingPacketsRecv > 0 && g_ulPingPacketsRecv <= NO_OF_ATTEMPTS)
	{
		// LAN connection is successful
		UART_PRINT("Ping Test successful\n\r");
	}
	else
	{
		// Problem with LAN connection
		ASSERT_ON_ERROR(LAN_CONNECTION_FAILED);
	}

	return SUCCESS;
}

//****************************************************************************
//
//! Manages the configuration data in the s-flash
//!
//! \param Mode indicates type of requested operation
//!
//!  Possible Mode values: SF_TEST_DATA_RECORD, SF_CREATE_DATA_RECORD, SF_WRITE_DATA_RECORD, SF_READ_DATA_RECORD, SF_DELETE_DATA_RECORD
//!
//!
//! \return 0 on success.
//
//****************************************************************************
long ManageConfigData(unsigned char Mode)
{

	long lRetVal;
	unsigned long ulToken;

	sl_Start(NULL,NULL,NULL);

	switch (Mode)
	{
	case SF_TEST_DATA_RECORD:
		lRetVal = sl_FsOpen((unsigned char *)CONFIG_FILE_NAME, FS_MODE_OPEN_READ, &ulToken, &g_isFileHandle);
		sl_FsClose(g_isFileHandle,0,0,NULL);

		break;

	case SF_CREATE_DATA_RECORD:
		sl_FsOpen((unsigned char *) CONFIG_FILE_NAME, FS_MODE_OPEN_CREATE(1024,_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE), &ulToken,&g_isFileHandle);
		sl_FsWrite(g_isFileHandle, 0, (unsigned char *) &g_ConfigData, sizeof(g_ConfigData));
		sl_FsClose(g_isFileHandle,0,0,NULL);

		break;


	case SF_WRITE_DATA_RECORD:
		sl_FsOpen((unsigned char *) "AdminData.txt", FS_MODE_OPEN_WRITE, &ulToken,&g_isFileHandle);
		sl_FsWrite(g_isFileHandle, 0, (unsigned char *) &g_ConfigData, sizeof(g_ConfigData));
		sl_FsClose(g_isFileHandle,0,0,NULL);

		break;


	case SF_READ_DATA_RECORD:
		sl_FsOpen((unsigned char *) "AdminData.txt",FS_MODE_OPEN_READ, &ulToken,&g_isFileHandle);
		sl_FsRead(g_isFileHandle,0, (unsigned char *) &g_ConfigData, sizeof(g_ConfigData));
		sl_FsClose(g_isFileHandle,0,0,NULL);

		break;


	case SF_DELETE_DATA_RECORD:
		sl_FsDel((unsigned char *) "AdminData.txt",0);

		break;


	default:
		lRetVal = -1;

		break;

	}

	sl_Stop(200);

	return lRetVal;

}


//****************************************************************************
//
//! Confgiures the mode in which the device will work
//!
//! \param iMode is the current mode of the device
//!
//! This function
//!    1. Change the mode to STA or AP mode and reset the NWP
//!
//! \return none.
//
//****************************************************************************
int ConfigureMode(int iMode)
{
	int status;

	long  lRetVal = -1;

	if(iMode == ROLE_STA)
	{
		sl_Start(NULL,NULL,NULL);

		lRetVal = sl_WlanSetMode(ROLE_STA);
		ASSERT_ON_ERROR(lRetVal);

		/* Restart Network processor */
		sl_Stop(SL_STOP_TIMEOUT);
		osi_Sleep(80);
		status =  sl_Start(NULL,NULL,NULL);


	} else if (iMode == ROLE_AP) {
		//UART_PRINT("iMode == ROLE_AP\n\r");
		sl_Start(NULL,NULL,NULL);

		lRetVal = sl_WlanSetMode(ROLE_AP);
		//UART_PRINT("lRetVal = sl_WlanSetMode(ROLE_AP);\n\r");
		ASSERT_ON_ERROR(lRetVal);
		// setting AP SSID
		lRetVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID, strlen(g_APSsidName), (unsigned char*)g_APSsidName);
		ASSERT_ON_ERROR(lRetVal);

		lRetVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1, (_u8 *)SECURITY_TYPE);
		ASSERT_ON_ERROR(lRetVal);

	    _u8  str[65];
	    _u16  len = strlen(SECURITY_KEY);
	    memset(str, 0, 65);
	    memcpy(str, SECURITY_KEY, len);
	    lRetVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_PASSWORD, len, (_u8 *)str);
	    ASSERT_ON_ERROR(lRetVal);

		/* Restart Network processor */
		lRetVal = sl_Stop(SL_STOP_TIMEOUT);

		// reset status bits
		CLR_STATUS_BIT_ALL(g_ulStatus);

		status=  sl_Start(NULL,NULL,NULL);
	}
	return status;
}

//*****************************************************************************
//
//!	EmailHandleERROR
//!
//! @brief  Performs Error Handling for SMTP State Machine
//!
//! @param  servermessage is the response buffer from the smtp server
//!
//! @return none
//
//*****************************************************************************
/*
void EmailHandleERROR(long error, char * servermessage)
{
	// Errors are handled via flags set in the smtpStateMachine
	switch(error)
	{
	case SL_EMAIL_ERROR_INIT:
		// Server connection could not be established
		Report((char*)"Server connection error.\r\n");
		break;
	case SL_EMAIL_ERROR_HELO:
		// Server did not accept the HELO command from server
		Report((char*)"Server did not accept HELO:\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_AUTH:
		// Server did not accept authorization credentials
		Report((char*)"Authorization unsuccessful, check username/password.\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_FROM:
		// Server did not accept source email.
		Report((char*)"Email of sender not accepted by server.\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_RCPT:
		// Server did not accept destination email
		Report((char*)"Email of recipient not accepted by server.\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_DATA:
		// 'DATA' command to server was unsuccessful
		Report((char*)"smtp 'DATA' command not accepted by server.\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_MESSAGE:
		// Message body could not be sent to server
		Report((char*)"Email Message was not accepted by the server.\r\n");
		Report((char*)servermessage);
		break;
	case SL_EMAIL_ERROR_QUIT:
		// Message could not be finalized
		Report((char*)"Connection could not be properly closed. Message not sent.\r\n");
		Report((char*)servermessage);
		break;
	default:
		break;
	}
	Report("\r\n");
}
*/
//*****************************************************************************
//
//!	SendEmail
//!
//! @brief  Build email subject and body, calls for email set and send functions
//!
//! @param  tag_type - iso tag name (ISO15693 or ISO14443A). Code MASTER is used to indicate that special master tag was detected
//!
//! @return status. status = 0 on success
//
//*****************************************************************************
/*

long SendEmail(unsigned char tag_type)
{
	long lRetCode;
	char EmailSubject[60];
	char TagType[20];
	SlNetAppServerError_t sEmailErrorInfo;

	SlNetAppEmailSubject_t emailSubject;
	SlNetAppDestination_t destEmailAdd;
	SlNetAppSourceEmail_t sourceEmailId;
	SlNetAppSourcePassword_t sourceEmailPwd;

	switch(tag_type)
	{
	case MASTER:
		sprintf(TagType, "Master");
		break;

	case ISO15693:
		sprintf(TagType, "ISO15693");
		break;

	case ISO14443A:
		sprintf(TagType, "ISO14443A");
		break;

	default:
		sprintf(TagType, "Unknown");
		break;

	}

	sprintf(EmailSubject, "%s Tag Detected", TagType);

	UART_PRINT("\n\r%s\n\r", EmailSubject);
	//UART_PRINT("Scanned NFC Tag ID: %s\n\n\r", g_uid);

	//Set Destination Email
	memcpy(destEmailAdd.Email,g_ConfigData.DestEmailAddress,strlen(g_ConfigData.DestEmailAddress)+1);
	// Message
	snprintf(g_pcEmailmessage, sizeof(g_pcEmailmessage), \
			"CARD INFORMATION\n\n%-30s%40s\n%-30s%40s\n%-30s%s\n%-30s%40d\n\nSimple Link Enables Internet of Things\nTexas Instruments\nhttp://www.ti.com/simplelinkwifi", \
			"Scanned NFC TAG type:", TagType, \
			"Scanned NFC Tag ID:", g_uid, \
			"Scanned NFC Tag’s content:\n\n", g_tag_content, \
			"Total cards read since last reset:", g_tag_count);

	// Subject
	memcpy(emailSubject.Value,EmailSubject,strlen(EmailSubject)+1);

	//Set Destination Email
	memcpy(sourceEmailId.Username,g_ConfigData.SrcEmailAddress,strlen(g_ConfigData.SrcEmailAddress)+1);

	// Email Password
	memcpy(sourceEmailPwd.Password,g_ConfigData.Password,strlen(g_ConfigData.Password)+1);
	//    g_email_pass


	sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_DEST_EMAIL,strlen((char *) destEmailAdd.Email)+1,(unsigned char *)&destEmailAdd);
	//Subject Line
	sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_SUBJECT,strlen((char *) emailSubject.Value)+1,(unsigned char *)&emailSubject);

	sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_SOURCE_EMAIL,strlen((char *) sourceEmailId.Username)+1,(unsigned char*)&sourceEmailId);

	sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_PASSWORD,strlen((char *) sourceEmailPwd.Password)+1,(unsigned char*)&sourceEmailPwd);


	SlNetAppEmailOpt_t eMailServerSetting;

	eMailServerSetting.Ip = g_ulGatewayIP;


	eMailServerSetting.Family = AF_INET;
	eMailServerSetting.Port = GMAIL_HOST_PORT;
	eMailServerSetting.SecurityMethod = SL_SO_SEC_METHOD_SSLV3;
	eMailServerSetting.SecurityCypher = SL_SEC_MASK_SSL_RSA_WITH_RC4_128_MD5;

	sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_ADVANCED_OPT,sizeof(SlNetAppEmailOpt_t),(unsigned char*)&eMailServerSetting);

	// open socket

	int indx = 0;
	do {
		lRetCode = sl_NetAppEmailConnect();
		if (lRetCode) {
			UART_PRINT("Failed to open socket, return error code: %d \n\r", lRetCode);
			UART_PRINT("Retrying\n\r");
			indx ++;
		}

		if	(indx > 9)
			return lRetCode;

	} while (lRetCode != 0);

	UART_PRINT("Socket opened succesfully\n\r");


	// run smtp fsm
	lRetCode = sl_NetAppEmailSend(NULL,NULL,g_pcEmailmessage,&sEmailErrorInfo);
	if (lRetCode == 0) {
		UART_PRINT("Message has been sent to Gmail server\n\r");
		UART_PRINT("Source Address       %s\n\r", g_ConfigData.SrcEmailAddress);
		UART_PRINT("Destination Address  %s\n\r", g_ConfigData.DestEmailAddress);
		UART_PRINT("Message size         %d characters\n\r", strlen(g_pcEmailmessage));
	}
	EmailHandleERROR(lRetCode,(char*)sEmailErrorInfo.Value);


	return lRetCode;

	return 0;
}
*/
//*****************************************************************************
//const char *JSON_STRING =
//	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
//	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
//*****************************************************************************

void EPM_parseRequestCallback(ParseClient client, int error, int httpStatus, const char* httpResponseBody) {
    if (error == 0 && httpResponseBody != NULL) {
    	Report("[parseRequestCallback] Received callback:%s\r\n", httpResponseBody);

    	Report("\tParsing JSON\r\n\n");
/*
    	char response[256];


		char line1[16];
		char line2[16];



	    		int i;
	    		int index;
	    		int r;
	    		jsmn_parser p;
	    		jsmntok_t t[128];

	    		jsmn_init(&p);
	    		r = jsmn_parse(&p, httpResponseBody, strlen(httpResponseBody), t, sizeof(t)/sizeof(t[0]));
	    		Report("JSON Tokens: %d\r\n", r-1);
	    		if (r < 0) {
	    			Report("Failed to parse JSON: %d\r\n", r);
	    			//return 1;
	    		}

	    		// Assume the top-level element is an object
	    		if (r < 1 || t[0].type != JSMN_OBJECT) {
	    			Report("Object expected\r\n");
	    			//return 1;
	    		}

	    		// Loop over all keys of the root object
	    		for (index = 1; index < r; index++) {
	    			i=index;
	    			//Report("Loop JSON Token index: %d\r\n", i);
	    			if (jsoneq(httpResponseBody, &t[i], "result") == 0) {
	    				//Report("result in JSON Token index i: %d\r\n", i);
	    				//Report("result in JSON Token index index: %d\r\n", index);

//	    				Report("- result: %.*s\r\n", t[i+1].end-t[i+1].start,
//	    						httpResponseBody + t[i+1].start);
	    				//strncpy(userName, t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);
	    				strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
	    				response[t[i+1].end-t[i+1].start] = '\0';
	    				//sprintf(userName, "%s", t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);		//format string
	    				Report("JSON KEY: result, JSON VALUE: %s\r\n", response);


	    				//here be gates openin'

		    				sprintf(line1, "%-16s", "    Welcome!");		//format string
		    				//line1[7]=' ';
		    				sprintf(line2, "%-16s", response);		//format string

		    				Report("\r\n\tValid Card\r\n");

	    				//i++;
	    				index++;
	    				//Report("result out JSON Token index i: %d\r\n", i);
	    				//Report("result out JSON Token index index: %d\r\n", index);
	    			} else if (jsoneq(httpResponseBody, &t[i], "error") == 0) {
//	    				Report("erro in JSON Token index i: %d\r\n", i);
//	    				Report("erro in JSON Token index index: %d\r\n", index);

//	    				Report("- result: %.*s\r\n", t[i+1].end-t[i+1].start,
//		    						httpResponseBody + t[i+1].start);
		    				//strncpy(userName, t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);
		    				strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
		    				response[t[i+1].end-t[i+1].start] = '\0';
		    				//sprintf(userName, "%s", t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);		//format string
		    				Report("JSON KEY: error, JSON VALUE: %s\r\n", response);


			    				sprintf(line1, "%-16s", "     Error!");		//format string
			    				//line1[7]=' ';
			    				sprintf(line2, "%-16s", response);		//format string

			    				Report("\r\n\tInvalid Card\r\n");

	    				//i++;
	    				index++;
//	    				Report("error out JSON Token index i: %d\r\n", i);
//	    				Report("error out JSON Token index index: %d\r\n", index);
	    			} else if (jsoneq(httpResponseBody, &t[i], "code") == 0) {
//	    				Report("code in JSON Token index i: %d\r\n", i);
//	    				Report("code in JSON Token index index: %d\r\n", index);

//	    				Report("- result: %.*s\r\n", t[i+1].end-t[i+1].start,
//		    						httpResponseBody + t[i+1].start);
		    				//strncpy(userName, t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);
		    				strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
		    				response[t[i+1].end-t[i+1].start] = '\0';
		    				//sprintf(userName, "%s", t[i+1].end-t[i+1].start, httpResponseBody + t[i+1].start);		//format string
		    				Report("JSON KEY: code, JSON VALUE: %s\r\n", response);

//
//			    				sprintf(line1, "%-16s", "Error");		//format string
//			    				line1[7]=' ';
//			    				sprintf(line2, "%-16s", response);		//format string

	    				//i++;
	    				index++;
//	    				Report("code out JSON Token index i : %d\r\n", i);
//	    				Report("code out JSON Token index index: %d\r\n", index);
//	    			} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
//	    				int j;
//	    				Report("- Groups:\n");
//	    				if (t[i+1].type != JSMN_ARRAY) {
//	    					continue;
//	    				}
//	    				for (j = 0; j < t[i+1].size; j++) {
//	    					jsmntok_t *g = &t[i+j+2];
//	    					Report("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
//	    				}
//	    				i += t[i+1].size + 1;
	    			} else {
//	    				Report("unx in JSON Token index i: %d\r\n", i);
//	    				Report("unx in JSON Token index index: %d\r\n", index);
	    				Report("Unexpected JSON KEY -> %.*s\n", t[i].end-t[i].start,
	    						httpResponseBody + t[i].start);
	    				strncpy(response, httpResponseBody + t[i].start, t[i].end-t[i].start);
	    				response[t[i].end-t[i].start] = '\0';
//int res=atoi(response);
	    				Report("\r\n\tUndefined JSON Response\r\n");

//	    				sprintf(line1, "%-16s", "Error");		//format string
//	    				line1[7]=' ';
//	    				sprintf(line2, "%-16s", "");		//format string
	    				//i++;
	    				index++;
//	    				Report("unx out JSON Token index i: %d\r\n", i);
//	    				Report("unx out JSON Token index index: %d\r\n", index);
	    			}
	    		}
	    		//return 0;

*/
    	char response[256];

    		char line1[16];
    		char line2[16];

    		int i;
    		int index;
    		int r;
    		jsmn_parser p;
    		jsmntok_t t[128]; /* We expect no more than 128 tokens */

    		jsmn_init(&p);
    		r = jsmn_parse(&p, httpResponseBody, strlen(httpResponseBody), t, sizeof(t)/sizeof(t[0]));
    		Report("JSON Tokens: %d\r\n", r-1);
    		if (r < 0) {
    			Report("Failed to parse JSON: %d\r\n", r);
    			//return 1;
    		}

    		/* Assume the top-level element is an object */
    		if (r < 1 || t[0].type != JSMN_OBJECT) {
    			Report("Object expected\r\n");
    			//return 1;
    		}

    		/* Loop over all keys of the root object */
    		for (index = 1; index < r; index++) {
    			i=index;
    			if (jsoneq(httpResponseBody, &t[i], "result") == 0) {
    				strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
    				response[t[i+1].end-t[i+1].start] = '\0';
    				Report("JSON KEY: result, JSON VALUE: %s\r\n", response);

    				index++;
    			} else if (jsoneq(httpResponseBody, &t[i], "error") == 0) {
    					strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
    					response[t[i+1].end-t[i+1].start] = '\0';
    					Report("JSON KEY: error, JSON VALUE: %s\r\n", response);

    	    				sprintf(line1, "%-16s", "Error!");
    	    				sprintf(line2, "%-16s", "Try again");

    				index++;
    			} else if (jsoneq(httpResponseBody, &t[i], "message1") == 0) {
    					strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
    					response[t[i+1].end-t[i+1].start] = '\0';
    					Report("JSON KEY: message1, JSON VALUE: %s\r\n", response);

    	    				sprintf(line1, "%-16s", response);

    				index++;
    			} else if (jsoneq(httpResponseBody, &t[i], "message2") == 0) {
    					strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
    					response[t[i+1].end-t[i+1].start] = '\0';
    					Report("JSON KEY: message2, JSON VALUE: %s\r\n", response);

    						sprintf(line2, "%-16s", response);

    				index++;
    			} else if (jsoneq(httpResponseBody, &t[i], "authorization") == 0) {
    					strncpy(response, httpResponseBody + t[i+1].start, t[i+1].end-t[i+1].start);
    					response[t[i+1].end-t[i+1].start] = '\0';
    					int authorization=atoi(response);
    					Report("JSON KEY: authorization, JSON VALUE: %d\r\n", authorization);

    					if(authorization){
    						Report("\r\n\tAuthorized\r\n\r\n");
    						Authorized=1;
    						GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    					}else{
    						Report("\r\n\tNot authorized\r\n\r\n");
    						Authorized=0;
    						GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    				 	}

    					parseSuccess=1;
    					Timer_IF_Start(g_ulBase, TIMER_A, 3000);

    				index++;
    			} else {
    				Report("Unexpected JSON KEY -> %.*s\n", t[i].end-t[i].start, httpResponseBody + t[i].start);
    				strncpy(response, httpResponseBody + t[i].start, t[i].end-t[i].start);
    				response[t[i].end-t[i].start] = '\0';

    				Report("\r\n\tUndefined JSON Response\r\n");
    				index++;
    			}
    		}


	    			writeLines(line1, line2);

	    			Report("\r\n[parseRequestCallback] Callback end\r\n");

  }else{
			Report("\r\n\r\n\tParse Error: %s\r\n\r\n", httpResponseBody);

			parseSuccess=0;

//			sprintf(line1, "%-16s", "System error.");		//format string
//			//line1[7]=' ';
//			sprintf(line2, "%-16s", "Try again?");		//format string
//			writeLines(line1, line2);

//			g_ConfigData.Role = 1;
//			g_uiNFCAppState = CONNECT_sta;

  }
}
//*****************************************************************************
//
//!	ElectronicParkingMeter
//!
//! @brief  Runs NFC reader application and provisioning.
//!
//! @param  none
//!
//! @return none
//
//*****************************************************************************
void ElectronicParkingMeter ( void *pvParameters )
{


	long lRetVal;
	//int iretvalmDNS,
	int iTestResult = 0;
	unsigned int uiConnectTimeoutCnt =0;


	while(1)
	{
		switch(g_uiNFCAppState)
		{
		case INIT:
		{
			//g_tag_count = 0;
			g_reset = 0;
			//TRF_OFF;
			GPIO_Clear(9);
			//GPIO_Clear(10);


			if (ManageConfigData(SF_TEST_DATA_RECORD) < 0) // config file doesn't exist in SF
			{
//				strncpy((char *) g_uid, INIT_CARD_ID, sizeof(INIT_CARD_ID));
				strncpy(g_ConfigData.DestEmailAddress, DESTINATION_EMAIL_ADDRESS, sizeof(DESTINATION_EMAIL_ADDRESS));
				strncpy(g_ConfigData.SrcEmailAddress, SOURCE_EMAIL_ADDRESS, sizeof(SOURCE_EMAIL_ADDRESS));
				strncpy(g_ConfigData.Password, EMAIL_PASSWORD, sizeof(EMAIL_PASSWORD));
				strncpy(g_ConfigData.CardID, MASTER_UID, sizeof(MASTER_UID));
				g_ConfigData.Role = 0;

				sl_Start(NULL,NULL,NULL);
				UART_PRINT("Start Device = For Init \n\r");

				// Remove all profiles
				lRetVal = sl_WlanProfileDel(0xFF);
				UART_PRINT("Delete all profiles \n\r");

				lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);

				UART_PRINT("Remove Fast Connect Policy \n\r");

				//Add Profile
				lRetVal = sl_WlanProfileAdd( (signed char *) STA_SSID_NAME, strlen(STA_SSID_NAME), 0, &g_SecParams, 0, 0 ,0);

				UART_PRINT("Add Profile \n\r");

				sl_Stop(SL_STOP_TIMEOUT);

				g_uiNFCAppState= CREATE_CONFIG;

			}
			else
				g_uiNFCAppState= RESTORE_CONFIG;


			break;
		}
		case CREATE_CONFIG:
		{
			UART_PRINT("Creating config record\n\r");
			ManageConfigData(SF_CREATE_DATA_RECORD);
			g_uiNFCAppState = CONNECT_ap;
			break;
		}
		case SAVE_CONFIG:
		{
			UART_PRINT("Save config record\n\r");
			ManageConfigData(SF_WRITE_DATA_RECORD);
			g_uiNFCAppState = CONNECT_sta;
			break;
		}
		case DEL_CONFIG:
		{
			UART_PRINT("Delete config record\n\r");
			osi_Sleep(500);
			ManageConfigData(SF_DELETE_DATA_RECORD);
			osi_Sleep(500);
			g_uiNFCAppState = INIT;
			break;
		}
		case RESTORE_CONFIG:
		{
			UART_PRINT("Restoring config record\n\r");
			ManageConfigData(SF_READ_DATA_RECORD);
			if(g_ConfigData.Role)
				g_uiNFCAppState= CONNECT_sta;
			else
				g_uiNFCAppState= CONNECT_ap;
			break;
		}
		case CONNECT_ap:
		{
			UART_PRINT("Starting AP\n\r");
			lRetVal = ConfigureMode(ROLE_AP);
			if (lRetVal == 2) {
				g_uiNFCAppState= NFC_READER_OFF_ap;
			}
		}
		case NFC_READER_OFF_ap:
		{
			if (g_reset) {
				sl_Stop(SL_STOP_TIMEOUT);
				g_uiNFCAppState = DEL_CONFIG;
				//				osi_Sleep(200);
			} else if (!IS_IP_LEASED(g_ulStatus)) {
				//wait util user connects to AP
				//GPIO_Set(10);
				osi_Sleep(500);
				//GPIO_Clear(10);
				osi_Sleep(500);
			} else if (!g_ConfigData.Role) {
				// wait until user changes role
				//GPIO_Set(10);
				osi_Sleep(150);
				//GPIO_Clear(10);
				osi_Sleep(150);
			} else {
				g_uiNFCAppState = SAVE_CONFIG;
				//GPIO_Clear(10);
			}
			break;
		}
		case CONNECT_sta:
		{
			UART_PRINT("Starting STA\n\r");
			lRetVal = ConfigureMode(ROLE_STA);



			uiConnectTimeoutCnt =0;
			UART_PRINT("Connecting to AP ");
			do
			{
				GPIO_Set(9);
				osi_Sleep(500);
				GPIO_Clear(9);
				osi_Sleep(500);
				uiConnectTimeoutCnt++;

				if (uiConnectTimeoutCnt>CONNECTION_TIMEOUT_COUNT) {
					g_ConfigData.Role = 0;
					UART_PRINT("Not able to connect to IP\n\r");
					g_uiNFCAppState = CONNECT_ap;
					break;
				}
				UART_PRINT(".");

			}
			while (!IS_CONNECTED(g_ulStatus));

			UART_PRINT(" Connected\n\r");

			UART_PRINT("STA Connected to the AP: %s ,"
					"BSSID: %x:%x:%x:%x:%x:%x\n\r",
					g_ucConnectionSSID,g_ucConnectionBSSID[0],
					g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
					g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
					g_ucConnectionBSSID[5]);


			UART_PRINT("Aqcuiring IP ");
			uiConnectTimeoutCnt =0;

			do
			{
				GPIO_Set(9);
				osi_Sleep(250);
				GPIO_Clear(9);
				osi_Sleep(250);
				uiConnectTimeoutCnt++;

				// Not able to connect, back to AP mode
				if (uiConnectTimeoutCnt>CONNECTION_TIMEOUT_COUNT) {
					g_ConfigData.Role = 0;
					UART_PRINT("Not able to acquire IP\n\r");
					g_uiNFCAppState = CONNECT_ap;
					break;
				}
				UART_PRINT(".");

			}
			while (!IS_IP_ACQUIRED(g_ulStatus));

			if(g_ulAcquiredIP == 0 ||  g_ucConnectionBSSID[0]=='0' && g_ucConnectionBSSID[1]=='0'
									&& g_ucConnectionBSSID[2]=='0' && g_ucConnectionBSSID[3]=='0'
							        && g_ucConnectionBSSID[4]=='0' && g_ucConnectionBSSID[5]=='0'){

				if(Wifi_Error >= 2){

					UART_PRINT("No Wifi, switching to AP mode\n\r");
					g_ConfigData.Role = 0;

					sprintf(line1, "%-16s", "No Wifi");		//format string
					sprintf(line2, "%-16s", "Starting AP...");		//format string
					writeLines(line1, line2);

					g_uiNFCAppState = CONNECT_ap;

				}else{

					UART_PRINT("Invalid IP acquired\n\r");
					g_ConfigData.Role = 1;
					UART_PRINT("Not able to connect to IP\n\r");

					sprintf(line1, "%-16s", "Wifi Error");		//format string
					sprintf(line2, "%-16s", "Retrying...");		//format string
					writeLines(line1, line2);

					g_uiNFCAppState = CONNECT_sta;
				}

				Wifi_Error++;

			}else{

				UART_PRINT(" Acquired\n\r");

				UART_PRINT("IP Acquired: IP=%d.%d.%d.%d , "
						"Gateway=%d.%d.%d.%d\n\r",
						SL_IPV4_BYTE(g_ulAcquiredIP,3),
						SL_IPV4_BYTE(g_ulAcquiredIP,2),
						SL_IPV4_BYTE(g_ulAcquiredIP,1),
						SL_IPV4_BYTE(g_ulAcquiredIP,0),
						SL_IPV4_BYTE(g_ulGatewayIP,3),
						SL_IPV4_BYTE(g_ulGatewayIP,2),
						SL_IPV4_BYTE(g_ulGatewayIP,1),
						SL_IPV4_BYTE(g_ulGatewayIP,0));

//			iretvalmDNS = sl_NetAppMDNSRegisterService( (const signed char *) SERVICE_NAME,\
//					(unsigned char)strlen(SERVICE_NAME),\
//					(const signed char *) "Service registered for 3200",\
//					(unsigned char)strlen("Service registered for 3200"),
//					200,2000,1);
//
//
//			if(iretvalmDNS == 0)
//			{
//				Report("MDNS Registration successful\n\r");
//			}
//			else
//			{
//				Report("MDNS Registration failed\n\r");
//			}
				g_uiNFCAppState = SNTP_GET_TIME_sta;

			}

			break;
		}
		case SNTP_GET_TIME_sta:
		{
			UART_PRINT("Getting time from SNTP server\r\n");
			//****************************************************************************

		    int iSocketDesc;
		    long lRetVal = -1;

		    //
		    // Create UDP socket
		    //
		    iSocketDesc = sl_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		    if(iSocketDesc < 0)
		    {
		        ERR_PRINT(iSocketDesc);
		        //goto end;
		    }
		    g_sAppData.iSockID = iSocketDesc;

		    UART_PRINT("Socket created\n\r");

		    //
		    // Get the NTP server host IP address using the DNS lookup
		    //
		    lRetVal = Network_IF_GetHostIP((char*)g_acSNTPserver, \
		                                    &g_sAppData.ulDestinationIP);

		    if( lRetVal >= 0)
		    {

		        struct SlTimeval_t timeVal;
		        timeVal.tv_sec =  SERVER_RESPONSE_TIMEOUT;    // Seconds
		        timeVal.tv_usec = 0;     // Microseconds. 10000 microseconds resolution
		        lRetVal = sl_SetSockOpt(g_sAppData.iSockID,SL_SOL_SOCKET,SL_SO_RCVTIMEO,\
		                        (unsigned char*)&timeVal, sizeof(timeVal));
		        if(lRetVal < 0)
		        {
//		           ERR_PRINT(lRetVal);
//		           LOOP_FOREVER();
		           NTPServerError=1;
		        }

		        //while(1)
		        //{
		            //
		            // Get the NTP time and display the time
		            //
		            lRetVal = GetSNTPTime(GMT_DIFF_TIME_HRS, GMT_DIFF_TIME_MINS);
		            if(lRetVal < 0)
		            {
		                UART_PRINT("Server Get Time failed\n\r");
		                //handle error, change SNTP domain
		                break;
		            }

		            //
		            // Wait a while before resuming
		            //
		         //   MAP_UtilsDelay(SLEEP_TIME);
		        //}
		    }
		    else
		    {
		        UART_PRINT("DNS lookup failed. \n\r");
		    }

		    //
		    // Close the socket
		    //
		    close(iSocketDesc);
		    UART_PRINT("Socket closed\n\r");





			//****************************************************************************

//		    static short NTP_YEAR=0;
//		    static short NTP_MONTH=0;
//		    static short NTP_DAY=0;
//		    static short NTP_HOUR=0;
//		    static short NTP_MINUTE=0;
//		    static short NTP_SECOND=0;

		    SlDateTime_t dateTime;
		    memset(&dateTime, 0, sizeof(dateTime));
		    dateTime.sl_tm_year=NTP_YEAR;
		    dateTime.sl_tm_mon=NTP_MONTH;
		    dateTime.sl_tm_day=NTP_DAY;
		    dateTime.sl_tm_hour=NTP_HOUR;
		    dateTime.sl_tm_min=NTP_MINUTE;
			dateTime.sl_tm_sec=NTP_SECOND;

		    sl_DevSet(SL_DEVICE_GENERAL_CONFIGURATION, SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME, sizeof(SlDateTime_t), (unsigned char *)&dateTime);

		    Report("Time set: [%s]\r\n", GetTime());

//	         _i8 configLen = sizeof(SlDateTime_t);
//	         _i8 configOpt = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME;
//	         sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION,&configOpt, &configLen,(_u8 *)(&dateTime));
//
//	         Report("Day %d, Month %d, Year %d, Hour %d, Min %d, Sec %d\n",dateTime.sl_tm_day, dateTime.sl_tm_mon, dateTime.sl_tm_year,
//	                 dateTime.sl_tm_hour, dateTime.sl_tm_min, dateTime.sl_tm_sec);

	         //ReportDate();

		    if(!NTPServerError){
		    	g_uiNFCAppState = CONNECT_TO_PARSE_sta;
		    }else{
		    	g_uiNFCAppState = SNTP_GET_TIME_sta;
		    	NTPServerError=0;
		    }

			break;

		}
		case CONNECT_TO_PARSE_sta:
		{

/*
			int		fail_count = 0;

			do // get host ID
			{
				lRetVal = Network_IF_GetHostIP(GMAIL_HOST_NAME, &g_ulGatewayIP);
				if(iTestResult < 0)
					UART_PRINT("Get Host IP failed \n\r");

				GPIO_Set(9);
				osi_Sleep(150);
				GPIO_Clear(9);
				osi_Sleep(150);

				fail_count++;
			}
			while( (lRetVal < 0) && (fail_count < 101 ) ) ;

#if 1
			{
				if (fail_count >= 100) {
					//IRQ_OFF;
					//TRF_OFF;
					GPIO_Clear(9);
					lRetVal = sl_WlanDisconnect();
					osi_Sleep(1000);

					g_ConfigData.Role = 0;
					g_uiNFCAppState = CONNECT_ap;

					UART_PRINT("Failed to connect to Configured AP.. Going back to AP mode!! \n\r");
					break;
				}
			}
#endif
*/

			if(TEST_PING){

				UART_PRINT("Testing network conectivity\n\r");


				GPIO_Set(9);

				do // test connection to host
				{
					iTestResult = PingTest(g_ulGatewayIP);
					//iTestResult = PingTest(134744072);
					//UART_PRINT("Ping to gateway: 8.8.8.8\n\r");

					if(iTestResult)
						UART_PRINT("Ping to client failed \n\r");

				}
				while (iTestResult) ;

				GPIO_Clear(9);
			}

			//state
			UART_PRINT("Conecting to Parse \n\r");
			client = parseInitialize("2pWww3Mbn2k28ctC8O8G5Gtg4cVebSqXaMwNr0EC", "vYYavuwXTPRCiLahrgq1hAGnf8d9UpmRtGJ6XCYq");
			parseSendRequest(client, "POST", "/1/classes/TestObject", "{\"foo\":\"bar\"}", NULL);

			TAGEnable=1;
			Report("UART1 Enabled\n\r");

			UART_PRINT("NFC Reader is active and runs scan \n\r");
		//}

//			sprintf(newLine, "%-16s", " ");		//format string
			sprintf(line1, "%-16s", " ");		//format string
			sprintf(line2, "%-16s", " ");		//format string

		  	Report("\n\r");
		  	Report("\t\t *************************************************\n\r");
		  	Report("\t\t	                Ready!\n\r");
		  	Report("\t\t *************************************************\n\r");
		  	Report("\n\r");

		  	ReportDate();
		  	Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);

				sprintf(line1, "%-16s", "      EPM");
				sprintf(line2, "    %-16s", GetTime());
				writeLines(line1, line2);

			g_uiNFCAppState = NFC_READER_ON_sta;

			break;
		}
		case NFC_READER_ON_sta:
		{

			if(TimmerRefFlag){
				TimmerBaseFlag=0;
				TimmerRefFlag=0;

				sprintf(line1, "%-16s", "      EPM");
				sprintf(line2, "    %-16s", GetTime());
				writeLines(line1, line2);
			}

			if(Sensor1){
				Sensor1=0;

				TimmerBaseFlag=0;
				TimmerRefFlag=0;

				Timer_IF_Stop(g_ulRefBase, TIMER_A);
				Timer_IF_Start(g_ulBase, TIMER_A, 30000);

				Report("Vehicle at Sensor 1\r\n");

				sprintf(line1, "%-16s", "      EPM");		//format string
				sprintf(line2, "%-16s", " Tap RFID Card");		//format string
				writeLines(line1, line2);
			}


			if(TimmerBaseFlag){							//30 seconds for tap screen, go back to normal screen
				TimmerBaseFlag=0;
				TimmerRefFlag=0;

				Timer_IF_Stop(g_ulBase, TIMER_A);
				Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);
			}

			if(UARTDataFlag){
				TAGEnable=0;

				Timer_IF_Stop(g_ulRefBase, TIMER_A);
				Timer_IF_Stop(g_ulBase, TIMER_A);
				TimmerBaseFlag=0;
				TimmerRefFlag=0;

				Report("UART1 Disabled\n\r");

			  	Report("\n\r");
			  	Report("\t\t *************************************************\n\r");
			  	Report("\t\t	            Tag Received!\n\r");
			  	Report("\t\t *************************************************\n\r");
			  	Report("\n\r");

			  	ReportDate();

				//char line1[16];
					sprintf(line1, "%-16s", "   Processing.");		//format string
				//char line2[16];
					sprintf(line2, "%-16s", "Checking account");		//format string
				writeLines(line1, line2);

				//tagData[0] = '\0';	//erase tagData String

				//sprintf(tagData, "%-256s", ' ');		//format string


				int i=0;
				for(i=0; i<UARTDataCount; i++){
					tagData[i] = UARTDataBuffer[i];
				}

				tagData[UARTDataCount] = '\0';	//erase tagData String
				UARTDataBuffer[0] = '\0';	//erase UARTDataBuffer String
				UARTDataFlag=0;
				UARTDataCount=0;
				UARTError=0;

				g_uiNFCAppState = SEND_PARSE_sta;

			} else if(UARTError){
				Timer_IF_Stop(g_ulRefBase, TIMER_A);
				Timer_IF_Stop(g_ulBase, TIMER_A);

				TimmerBaseFlag=0;
				TimmerRefFlag=0;

			  	Report("\n\r");
			  	Report("\t\t *************************************************\n\r");
			  	Report("\t\t	               Card Error!\n\r");
			  	Report("\t\t *************************************************\n\r");
			  	Report("\n\r");

			  	ReportDate();

				//char line1[16];
					sprintf(line1, "%-16s", "Card read error!");		//format string
				//char line2[16];
					sprintf(line2, "%-16s", "   Try Again");		//format string
				writeLines(line1, line2);

				UARTError=0;

				tagData[UARTDataCount] = '\0';	//erase tagData String
				UARTDataBuffer[0] = '\0';	//erase UARTDataBuffer String
				UARTDataFlag=0;
				UARTDataCount=0;

				g_uiNFCAppState = NFC_READER_ON_sta;
				Timer_IF_Start(g_ulBase, TIMER_A, 30000);
			}


			//my_custom_data[0] = '\0';

			//while(1){
/*
				if(UARTDataFlag){

				      UARTDataFlag = 0;
				      UARTDataCount = 0;

					Report("OUTPUT: %s\n\r", UARTDataBuffer);

					//MessageDev(UARTDataBuffer);


					sprintf(newLine, "%-16s", UARTDataBuffer);		//format string
					sprintf(line1, "%s", line2);		//format string
					sprintf(line2, "%s", newLine);		//format string

					writeLines(line1, line2);

					g_uiNFCAppState = SEND_PARSE_sta;

					osi_Sleep(5);

				}
*/
				break;
			//}



			//scan here


		}
		case SEND_PARSE_sta:
		{
			Report("Tag UID: %s\n\r", tagData);


			//Parse here
///*

			sprintf(parseData, "{\"TAG\":\"%s\",\"TIME\":\"%s\",\"GATE\":\"%s\"}", tagData, GetDate(), "FEDCBA0987654321");		//format string

			//Report("client, \"POST\", \"/1/functions/tag\" , %s, NULL\n\r", parseData);
			Report("client, \"POST\", \"/1/functions/ActiveTest\" , %s, NULL\n\r", parseData);

			//parseSendRequest(client, "POST", "/1/functions/tag", parseData, NULL);
			parseSendRequest(client, "POST", "/1/functions/ActiveTest", parseData, EPM_parseRequestCallback);

//			sprintf(newLine, "%-16s", UARTDataBuffer);		//format string
//			sprintf(line1, "%s", "Welcome");		//format string
//			sprintf(line2, "%s", tagData);		//format string
//
//			writeLines(line1, line2);

		      UARTDataFlag = 0;
		      UARTDataCount = 0;



//			sprintf(line1, "%-16s", "      EPM");		//format string
//			sprintf(line2, "%-16s", " Tap RFID Card");		//format string
//			writeLines(line1, line2);

	         ReportDate();

	         if(parseSuccess){
	        	 g_uiNFCAppState = HANDLE_PARSE_sta;
	        	 Sensor2=0;								//ignore flags
	        	 parseRetries=0;
	        	 parseSuccess=0;
			 }else{

				 g_uiNFCAppState = SEND_PARSE_sta;
				 parseRetries++;
				 parseSuccess=0;

					Report("\n\r");
					Report("\t\t *************************************************\n\r");
					Report("\t\t	       Parse Error... Retrying\n\r");
					Report("\t\t *************************************************\n\r");
					Report("\n\r");

				 if(parseRetries>3){
						g_uiNFCAppState = NFC_READER_ON_sta;
						parseRetries=0;
						parseSuccess=0;

						sprintf(line1, "%-16s", "System error.");		//format string
						sprintf(line2, "%-16s", "Try again?");		//format string
						writeLines(line1, line2);

						 TAGEnable=1;
						 Report("UART1 Enabled\n\r");

							Report("\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\t\t	                Ready!\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\n\r");

						Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);
				 }
	         }

//*/
			parseSuccess=0;

		    break;

		}


		case HANDLE_PARSE_sta:
		{
			//Report("HANDLE_PARSE_sta\n\r");
	        //ReportDate();

			if(Authorized){
				if(TimmerBaseFlag){

					Timer_IF_Stop(g_ulBase, TIMER_A);
					TimmerBaseFlag=0;

					sprintf(line1, "%-16s", "      EPM");
					sprintf(line2, "%-16s", " Cross the Gate");
					writeLines(line1, line2);
				}

				if(Sensor2){
					UtilsDelay(10000);
					if(GPIOPinRead(GPIOA0_BASE, 0x10) == 0x10){
						Report("Vehicle at Sensor 2\r\n");
					}else{
						Timer_IF_Stop(g_ulBase, TIMER_A);	//in case vehicle enters too fast
						TimmerBaseFlag=0;
						Authorized=0;

						GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
						GPIO_IF_LedOff(MCU_RED_LED_GPIO);

						Report("Vehicle left Sensor 2\r\n");


				         TAGEnable=1;
				         Report("UART1 Enabled\n\r");

							Report("\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\t\t	                Ready!\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\n\r");

						g_uiNFCAppState = NFC_READER_ON_sta;
						Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);
					}
					Sensor2=0;
				}

			}else{
				if(TimmerBaseFlag){

					Timer_IF_Stop(g_ulBase, TIMER_A);
					TimmerBaseFlag=0;

					GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
					GPIO_IF_LedOff(MCU_RED_LED_GPIO);

			         TAGEnable=1;
				         Report("UART1 Enabled\n\r");

							Report("\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\t\t	                Ready!\n\r");
							Report("\t\t *************************************************\n\r");
							Report("\n\r");

					g_uiNFCAppState = NFC_READER_ON_sta;
					Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);
				}
			}

		    break;

		}

//		case HANDLE_PARSE_sta:
//		{
//			Report("HANDLE_PARSE_sta\n\r");
//	        ReportDate();
//
//	        if(TimmerBaseFlag){
//
//
//	        	Timer_IF_Stop(g_ulBase, TIMER_A);
//	        	TimmerBaseFlag=0;
//
//	        	GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
//	        	GPIO_IF_LedOff(MCU_RED_LED_GPIO);
//
//				sprintf(line1, "%-16s", "      EPM");		//format string
//				sprintf(line2, "%-16s", "Enter Parking");		//format string
//				writeLines(line1, line2);
//
//
//	        	g_uiNFCAppState = NFC_READER_ON_sta;
//	        }
//
//		    break;
//
//		}

		}
	}

}

//*****************************************************************************
void TimerBaseIntHandler(void)
{
    Timer_IF_InterruptClear(g_ulBase);

    TimmerBaseFlag=1;

    g_ulTimerInts ++;
}

//*****************************************************************************
//
//! The interrupt handler for the second timer interrupt.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
void TimerRefIntHandler(void)
{
    Timer_IF_InterruptClear(g_ulRefBase);

    TimmerRefFlag=1;

    g_ulRefTimerInts ++;
}
//*****************************************************************************
/*
void UART1_Handler(){


	GPIO_Set(9);

	//Message("UART1 RX INT\n\r");

	IntMasterDisable();

	  UART_STATUS = 0;

	  UART_STATUS = UARTIntStatus(DEVICE, true);

	  MAP_UARTIntClear(DEVICE, UART_INT_RX);

	  if((UART_STATUS & UART_INT_RX) && MAP_UARTCharsAvail(DEVICE))

	  {

	    UARTData = (unsigned char)MAP_UARTCharGetNonBlocking(DEVICE);

	    UARTDataBuffer[UARTDataCount] =  UARTData;

	    UARTDataCount++;

	    if(UARTDataCount >= 16)

	    {

	      UARTDataFlag = 1;
	      UARTDataCount = 0;

	    }

//	    if(UARTDataCount > 16)
//
//	    {
//
//	      UARTDataCount = 0x00;
//
//	    }

	  }

//	  if(UARTData != '\r' && UARTData != '\n'){
		  char tmp[32];
		  sprintf(tmp, "UART1 RX ISR Data: %c\n\r", UARTData);		//format string
		  Message(tmp);
//	  }

	  IntMasterEnable();
	  MAP_UARTIntEnable(DEVICE, UART_INT_RX);            //Interrupt enabled again

	  GPIO_Clear(9);
}
*/

//*****************************************************************************
//							MAIN FUNCTION
//*****************************************************************************
void main()
{

	//
	// Board Initialization
	//
	BoardInit();
	//
	// configure the GPIO pins for LEDs
	//

	PinMuxConfig();

	//SPI_CS_ON; // Set spi cs to high

	//UARTInit();

//	DisplayBanner(APP_NAME);


	//SPIInit();

	//Initialize Push Botton Switch
	//Button_IF_Init(MDNSEnableInterruptHandler,FactoryResetInterruptHandler);

	// GPIO interrupt setting
	// TRF7970 IRQ
	//GPIOIntInit(GPIOA0_BASE, GPIO_PIN_7, INT_GPIOA0, Trf7970PortB, GPIO_RISING_EDGE, INT_PRIORITY_LVL_1);
	// SW3
	GPIOIntInit(GPIOA1_BASE, GPIO_PIN_5, INT_GPIOA1, GPIOs3IntHandler, GPIO_FALLING_EDGE, INT_PRIORITY_LVL_1);
	// SW2
	GPIOIntInit(GPIOA2_BASE, GPIO_PIN_6, INT_GPIOA2, GPIOs2IntHandler, GPIO_FALLING_EDGE, INT_PRIORITY_LVL_1);

	//A2CounterInit(A2CNTIntHandler);

	// Set Clock Frequency and Modulation
	//Trf7970InitialSettings();

	//****************************

	//Set Interrupt type on PIN07
	MAP_GPIOIntTypeSet(GPIOA2_BASE, 0x1, GPIO_RISING_EDGE);

	//Clear interrupt flags P07
	MAP_GPIOIntClear(GPIOA2_BASE, 0x1);

	//Clear pending interrupt flags P07
	MAP_IntPendClear(INT_GPIOA2);

	//Enable interrupt on GPIOA2_BASE
	MAP_IntEnable(INT_GPIOA2);

	//Enable interrupt on P07
	MAP_GPIOIntEnable(GPIOA2_BASE, 0x1);

	//****************************

	//Set Interrupt type on PIN_58
	MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x8, GPIO_BOTH_EDGES);

	//Clear interrupt flags PIN_58
	MAP_GPIOIntClear(GPIOA0_BASE, 0x8);

	//Clear pending interrupt flags PIN_58
	MAP_IntPendClear(INT_GPIOA0);

	//Enable interrupt on GPIOA0_BASE
	MAP_IntEnable(INT_GPIOA0);

	//Enable interrupt on PIN_58
	MAP_GPIOIntEnable(GPIOA0_BASE, 0x8);

	//****************************

	//Set Interrupt type on PIN_59
	MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x10, GPIO_BOTH_EDGES);

	//Clear interrupt flags PIN_59
	MAP_GPIOIntClear(GPIOA0_BASE, 0x10);

	//Clear pending interrupt flags PIN_59
	MAP_IntPendClear(INT_GPIOA0);

	//Enable interrupt on GPIOA0_BASE
	MAP_IntEnable(INT_GPIOA0);

	//Enable interrupt on PIN_59
	MAP_GPIOIntEnable(GPIOA0_BASE, 0x10);

	//****************************

	MAP_GPIOIntRegister(GPIOA0_BASE, ISR_SENSORS);



//****************************

	initLCD();

//	char newLine[16];
//		sprintf(newLine, "%-16s", " ");		//format string
	char line1[16];
		sprintf(line1, "%-16s", "   Electronic");		//format string
	char line2[16];
		sprintf(line2, "%-16s", " Parking  Meter");		//format string
	writeLines(line1, line2);

	//MAP_UARTFIFODisable(DEVICE); //disable fifo
	MAP_UARTFIFOEnable(DEVICE);
//	MAP_UARTIntRegister(DEVICE, UART1_Handler); //enable interrupts
//	MAP_UARTIntEnable(DEVICE, UART_INT_RX);


    //
    // Initialising the Terminal.
    //
    InitTerm();
    //
    // Clearing the Terminal.
    //
    ClearTerm();

    DisplayBanner(APP_NAME);


	//VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);



    	//Message("UART1 Ready\n\r");

/*
    		sprintf(newLine, "%-16s", " ");		//format string

    		sprintf(line1, "%-16s", " ");		//format string

    		sprintf(line2, "%-16s", " ");		//format string

        while(1){

        	if(UARTDataFlag){

    			UARTDataFlag=0;

    					Report("OUTPUT: %s\n\r", UARTDataBuffer);

    					MessageDev(UARTDataBuffer);


    					sprintf(newLine, "%-16s", UARTDataBuffer);		//format string
    					sprintf(line1, "%s", line2);		//format string
    					sprintf(line2, "%s", newLine);		//format string

    					writeLines(line1, line2);

    		}
        }

*/

	//
	// Start the SimpleLink Host
	//
	VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
//****************************


	//timer

	GPIO_IF_LedConfigure(LED1|LED2);

    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

    //
    // Base address for first timer
    //
    g_ulBase = TIMERA0_BASE;
    //
    // Base address for second timer
    //
    g_ulRefBase = TIMERA1_BASE;
    //
    // Configuring the timers
    //
    Timer_IF_Init(PRCM_TIMERA0, g_ulBase, TIMER_CFG_PERIODIC, TIMER_A, 0);
    Timer_IF_Init(PRCM_TIMERA1, g_ulRefBase, TIMER_CFG_PERIODIC, TIMER_A, 0);

    //
    // Setup the interrupts for the timer timeouts.
    //
    Timer_IF_IntSetup(g_ulBase, TIMER_A, TimerBaseIntHandler);
    Timer_IF_IntSetup(g_ulRefBase, TIMER_A, TimerRefIntHandler);

    //
    // Turn on the timers feeding values in mSec
    //
    //Timer_IF_Start(g_ulBase, TIMER_A, 500);
    //Timer_IF_Start(g_ulRefBase, TIMER_A, 1000);

    //
    // Loop forever while the timers run.
    //
//    while(1)
//    {
//    }


	//
	// Start the WlanStationMode task
	//
///*
	osi_TaskCreate(ElectronicParkingMeter,
			(const signed char*)"Electronic Parking Meter",
			OSI_STACK_SIZE, NULL, 1, NULL );
	UART_PRINT("EPM Process started\n\r");

//	osi_TaskCreate( myPushCallback,
//			(const signed char*)"Push Service",
//			OSI_STACK_SIZE, NULL, 1, NULL );
//	UART_PRINT("Push Process started\n\r");
//*/
	//osi_TaskCreate(PushButtonHandler, (signed char*)"PushButtonHandler",OSI_STACK_SIZE , NULL, 2, &g_PushButtonTask );

	//
	// Start the task scheduler
	//
	osi_start();











//    		int i;
//    		int r;
//    		jsmn_parser p;
//    		jsmntok_t t[128]; /* We expect no more than 128 tokens */
//
//    		jsmn_init(&p);
//    		r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
//    		if (r < 0) {
//    			Report("Failed to parse JSON: %d\n", r);
//    			return 1;
//    		}
//
//    		/* Assume the top-level element is an object */
//    		if (r < 1 || t[0].type != JSMN_OBJECT) {
//    			Report("Object expected\n");
//    			return 1;
//    		}
//
//    		/* Loop over all keys of the root object */
//    		for (i = 1; i < r; i++) {
//    			if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
//    				/* We may use strndup() to fetch string value */
//    				Report("- User: %.*s\n", t[i+1].end-t[i+1].start,
//    						JSON_STRING + t[i+1].start);
//    				i++;
//    			} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
//    				/* We may additionally check if the value is either "true" or "false" */
//    				Report("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
//    						JSON_STRING + t[i+1].start);
//    				i++;
//    			} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
//    				/* We may want to do strtol() here to get numeric value */
//    				Report("- UID: %.*s\n", t[i+1].end-t[i+1].start,
//    						JSON_STRING + t[i+1].start);
//    				i++;
//    			} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
//    				int j;
//    				Report("- Groups:\n");
//    				if (t[i+1].type != JSMN_ARRAY) {
//    					continue; /* We expect groups to be an array of strings */
//    				}
//    				for (j = 0; j < t[i+1].size; j++) {
//    					jsmntok_t *g = &t[i+j+2];
//    					Report("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
//    				}
//    				i += t[i+1].size + 1;
//    			} else {
//    				Report("Unexpected key: %.*s\n", t[i].end-t[i].start,
//    						JSON_STRING + t[i].start);
//    			}
//    		}
//    		return 0;












}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
