
#include "./../fw/fw.h"
#include "./../timer/timer.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void IRAM_ATTR timer_isr( void* arg )
{
	taskENTER_CRITICAL_ISR( &spinlock );
		uwb.flags.dw1000_tot = 1;
	taskEXIT_CRITICAL_ISR(  &spinlock );
}

void timer_inicializa( void )
{
	const esp_timer_create_args_t periodic_timer_args = {
			.callback = &timer_isr,
			/* name is optional, but may help identify the timer when debugging */
			.name = "periodic"
	};

	ESP_ERROR_CHECK( esp_timer_create( &periodic_timer_args, &uwb.timer.handler ) );
}

void timer_liga( void )
{
	ESP_ERROR_CHECK( esp_timer_start_periodic( uwb.timer.handler, 250000 ) );
}

void timer_desliga( void )
{
	ESP_ERROR_CHECK( esp_timer_stop( uwb.timer.handler ) );
}
