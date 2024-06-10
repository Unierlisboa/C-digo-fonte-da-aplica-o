
#include <string.h>
#include "../dw1000/dw1000.h"
#include "../fw/fw.h"
#include "../gpio/gpio.h"
#include "../spi/spi.h"
#include "../timer/timer.h"
#include <esp_task_wdt.h>

void fw_twdt_inicializa( void );

void fw_inicializa( void )
{
	memset( ( Uwb_t *)&uwb, 0x00, sizeof( Uwb_t ) );

	fw_twdt_inicializa();
	gpio_inicializa();
	uwb.spi.spi2_handler = spi_inicializa();
	dw1000_inicializa();
	timer_inicializa();
	gpio_habilita_isr();
}

void fw_twdt_inicializa( void )
{
   esp_task_wdt_config_t twdt_config={0};

   twdt_config.timeout_ms 		= 3000;
   twdt_config.idle_core_mask 	= ( 1 << portNUM_PROCESSORS)  - 1;
   twdt_config.trigger_panic 	= true;

   esp_task_wdt_reconfigure( &twdt_config );
}
