
#ifndef MAIN_INCLUDE_SW_SW_H_
#define MAIN_INCLUDE_SW_SW_H_

#include <stdio.h>
#include <string.h>
#include "../fw/fw.h"
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TRUE			1
#define UWB_TAG			0
#define UWB_ANCORA		1
#define SW_UWB			UWB_ANCORA

#if ( SW_UWB == UWB_TAG )
	enum{	SM_LOCALIZACAO_TAG_AGUARDA= 0,
			SM_LOCALIZACAO_TAG_TX1,
			SM_LOCALIZACAO_TAG_CONFIRMA_TX1,
			SM_LOCALIZACAO_ANC_ACK1,
			SM_LOCALIZACAO_TAG_TX2,
			SM_LOCALIZACAO_TAG_CONFIRMA_TX2,
			SM_LOCALIZACAO_ANC_RESP };
#elif ( SW_UWB == UWB_ANCORA )
	enum{	SM_LOCALIZACAO_ANC_RX = 0,
			SM_LOCALIZACAO_ANC_TX,
			SM_LOCALIZACAO_ANC_CONFIRMA_TX,
			SM_LOCALIZACAO_TAG_ACK,
			SM_LOCALIZACAO_ANC_RESP,
			SM_LOCALIZACAO_ANC_CONFIRMA_RESP };
#endif

Uwb_t	uwb;

#endif /* MAIN_INCLUDE_SW_SW_H_ */
