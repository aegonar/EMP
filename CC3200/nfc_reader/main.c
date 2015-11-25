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
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "simplelink.h"
#include "osi.h"
#include "interrupt.h"
#include "pin.h"
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
#include "common.h"
#include "demo_config.h"
//*****************************************************************************
#include "LCD.h"
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
#include "jsmn.h"
//*****************************************************************************
int TEST_PING=0;
int Wifi_Error=0;
//*****************************************************************************
#define UartGetCharDev()        MAP_UARTCharGet(DEVICE)
#define UartPutCharDev(c)       MAP_UARTCharPut(DEVICE,c)

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

long g_isFileHandle;								// Configuration Record File Handler

// HTTP server tokens variables

unsigned char STA_POST_token[] = "__SL_P_STA"; // AP/Station mode post token
unsigned char STS_GET_token[]  = "__SL_G_STS"; // Station Status (selected/not selected) get token
unsigned char APS_GET_token[]  = "__SL_G_APS"; // AP mode status (selected/not selected) get token
const char SELECTED[] 		= "selected";
const char NOT_SELECTED[] 	= "not_selected";

unsigned char FUN_POST_token[]  = "__SL_P_FUN";
unsigned char SUN_POST_token[]  = "__SL_P_SUN";
unsigned char COM_POST_token[]  = "__SL_P_CMP";
unsigned char TEL_POST_token[]  = "__SL_P_TEL";

unsigned char SND_POST_token[] = "__SL_P_SND"; // Reader state (on/off)
unsigned char SEA_GET_token[]  = "__SL_G_SEA";
unsigned char DEA_GET_token[]  = "__SL_G_DEA";


//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
static void BoardInit(void);

void DisplayBanner(char * AppName);
void GPIOs2IntHandler(void);
void GPIOs3IntHandler(void);

void ISR_SENSORS(void);

long Network_IF_GetHostIP( char* pcHostName,unsigned long * pDestinationIP );
static int PingTest(unsigned long ulIpAddr);
long ManageConfigData(unsigned char Mode);
int ConfigureMode(int iMode);

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

	    Sensor1=1;

		MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x8, GPIO_BOTH_EDGES);
		MAP_GPIOIntClear(GPIOA0_BASE, 0x8);
		MAP_IntPendClear(INT_GPIOA0);
	}

	//PIN_59
	if(GPIOPinRead(GPIOA0_BASE, 0x10) == 0x10){

		Sensor2=1;

		MAP_GPIOIntTypeSet(GPIOA0_BASE, 0x10, GPIO_BOTH_EDGES);
		MAP_GPIOIntClear(GPIOA0_BASE, 0x10);
		MAP_IntPendClear(INT_GPIOA0);
	}
}

//*****************************************************************************

void GPIOs3IntHandler(void)
{

	MAP_GPIOIntClear(GPIOA1_BASE, 0x20);

	if( GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20){
		Button_IF_DisableInterrupt(SW3);
		UtilsDelay(5000);

		Timer_IF_Stop(g_ulRefBase, TIMER_A);

		UART_PRINT("Maintenance mode enabled\n\r");

		UART_PRINT("Starting AP\n\r");

		g_ConfigData.Role = 0;
		g_uiNFCAppState = CONNECT_ap;

		sprintf(line1, "%-16s", "Maintenance mode");		//format string
		sprintf(line2, "%-16s", "Starting AP...");		//format string
		writeLines(line1, line2);

		Button_IF_EnableInterrupt(SW3);

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

	if(GPIOPinRead(GPIOA2_BASE, 0x1) == 0x1){

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

			}

			if(UARTDataCount != 16){
				Report("UART1 Buffer error.\r\n");
				Report("UART1 Buffer: %s\r\n", UARTDataBuffer);

				UARTDataCount=0;
				UARTError=1;
				UARTDataFlag=0;

			}else{
				UART_PRINT("UART1 Buffer full.\n\r");

				UARTDataFlag=1;
				UARTError=0;
			}

	    }else{
	    	UART_PRINT("UART1 Data ignored.\n\r");
	    }

		MAP_GPIOIntTypeSet(GPIOA2_BASE, 0x1, GPIO_RISING_EDGE);
		MAP_GPIOIntClear(GPIOA2_BASE, 0x1);
		MAP_IntPendClear(INT_GPIOA2);
	    UART_PRINT("P07 ISR out\n\r");
	}


//*****************************************************************************
//SW2 Button

		MAP_GPIOIntClear(GPIOA2_BASE, 0x40);

		if( GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40){
				Button_IF_DisableInterrupt(SW2);
				UtilsDelay(5000);

				Timer_IF_Stop(g_ulRefBase, TIMER_A);

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
//*JSON_STRING 

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

			g_reset = 0;

			GPIO_Clear(9);



			if (ManageConfigData(SF_TEST_DATA_RECORD) < 0) // config file doesn't exist in SF
			{

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

				osi_Sleep(500);

				osi_Sleep(500);
			} else if (!g_ConfigData.Role) {
				// wait until user changes role

				osi_Sleep(150);

				osi_Sleep(150);
			} else {
				g_uiNFCAppState = SAVE_CONFIG;

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

		           NTPServerError=1;
		        }

		            lRetVal = GetSNTPTime(GMT_DIFF_TIME_HRS, GMT_DIFF_TIME_MINS);
		            if(lRetVal < 0)
		            {
		                UART_PRINT("Server Get Time failed\n\r");
		                //handle error, change SNTP domain
		                break;
		            }

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


				sprintf(line1, "%-16s", "   Processing.");		//format string

				sprintf(line2, "%-16s", "Checking account");		//format string
				writeLines(line1, line2);

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


				sprintf(line1, "%-16s", "Card read error!");		//format string

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

				break;

		}
		case SEND_PARSE_sta:
		{
			Report("Tag UID: %s\n\r", tagData);



			sprintf(parseData, "{\"TAG\":\"%s\",\"TIME\":\"%s\",\"GATE\":\"%s\"}", tagData, GetDate(), "FEDCBA0987654321");		//format string

			Report("client, \"POST\", \"/1/functions/ActiveTest\" , %s, NULL\n\r", parseData);


			parseSendRequest(client, "POST", "/1/functions/ActiveTest", parseData, EPM_parseRequestCallback);


		      UARTDataFlag = 0;
		      UARTDataCount = 0;

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

						g_uiNFCAppState = NFC_READER_ON_sta;
						Timer_IF_Start(g_ulBase, TIMER_A, 30000);
				 }
	         }


			parseSuccess=0;

		    break;

		}


		case HANDLE_PARSE_sta:
		{

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

	// SW3
	GPIOIntInit(GPIOA1_BASE, GPIO_PIN_5, INT_GPIOA1, GPIOs3IntHandler, GPIO_FALLING_EDGE, INT_PRIORITY_LVL_1);
	// SW2
	GPIOIntInit(GPIOA2_BASE, GPIO_PIN_6, INT_GPIOA2, GPIOs2IntHandler, GPIO_FALLING_EDGE, INT_PRIORITY_LVL_1);


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


	char line1[16];
		sprintf(line1, "%-16s", "   Electronic");		//format string
	char line2[16];
		sprintf(line2, "%-16s", " Parking  Meter");		//format string
	writeLines(line1, line2);


	MAP_UARTFIFOEnable(DEVICE);


    //
    // Initialising the Terminal.
    //
    InitTerm();
    //
    // Clearing the Terminal.
    //
    ClearTerm();

    DisplayBanner(APP_NAME);

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



	osi_TaskCreate(ElectronicParkingMeter,
			(const signed char*)"Electronic Parking Meter",
			OSI_STACK_SIZE, NULL, 1, NULL );
	UART_PRINT("EPM Process started\n\r");

	//
	// Start the task scheduler
	//
	osi_start();


}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
