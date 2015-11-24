//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************

#ifndef DEMO_CONFIG_H
#define DEMO_CONFIG_H

//#include "config_defs.h"

//
// Modify the following settings as necessary to run the demo
//

#define IP_ALLOC_METHOD USE_DHCP

// Default SSID Settings


#define INIT_ROLE               0 // AP role

#define STA_SSID_NAME             	"demo"
#define AP_SSID_NAME				"EPM"
#define SECURITY_TYPE           	SL_SEC_TYPE_WPA_WPA2
#define SECURITY_KEY            	"1234567890"

#define GMAIL_HOST_NAME         	"smtp.gmail.com"
#define GMAIL_HOST_PORT         	465

#define SOURCE_EMAIL_ADDRESS        "cc3100.nfc.apps@gmail.com"   //Set Sender/Source Email Address
#define DESTINATION_EMAIL_ADDRESS 	"sergeyo@ti.com"

#define EMAIL_PASSWORD              "simplelink"              //Set Sender/Source Email Password
#define RCPT_RFC                	"<sergeyo@ti.com>"        //Set Recipient Email Password

#define MASTER_UID 					"04507f62f92680"
#define INIT_CARD_ID 				"None"
#define APP_NAME					"Electronic Parking Meter"
#define CONFIG_FILE_NAME        	"AdminData.txt"


///////////////////////////////////////////////////////////////////////////////

#endif
