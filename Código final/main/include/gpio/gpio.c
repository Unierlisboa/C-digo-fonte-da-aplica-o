
#include <string.h>
#include "./../fw/fw.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void IRAM_ATTR gpio_isr( void *args )
{
	taskENTER_CRITICAL_ISR( &spinlock );
		uwb.flags.dw1000_irq = 1;
	taskEXIT_CRITICAL_ISR(  &spinlock );
}

void gpio_inicializa( void )
{
	gpio_config_t io_conf;
	memset( ( gpio_config_t *)&io_conf, 0x00, sizeof( io_conf ) );

	io_conf.intr_type 		= GPIO_INTR_DISABLE;
	io_conf.pin_bit_mask 	= GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en 	= 0;
	io_conf.pull_up_en 	= 0;
	io_conf.mode 			= GPIO_MODE_OUTPUT;
	gpio_config( &io_conf );

	io_conf.intr_type 		= GPIO_INTR_DISABLE;
	io_conf.pin_bit_mask 	= GPIO_INPUT_PIN_SEL;
	io_conf.pull_down_en 	= 0;
	io_conf.pull_up_en 	= 0;
	io_conf.mode 			= GPIO_MODE_INPUT;
	gpio_config( &io_conf );

	io_conf.intr_type 		= GPIO_INTR_POSEDGE;
	io_conf.pin_bit_mask 	= GPIO_INPUT_INT_PIN_SEL;
	io_conf.pull_down_en 	= GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en 		= GPIO_PULLUP_DISABLE;
	io_conf.mode 			= GPIO_MODE_INPUT;
	gpio_config( &io_conf );
}

void gpio_habilita_isr( void )
{
	gpio_install_isr_service( ESP_INTR_FLAG_DEFAULT );
	gpio_isr_handler_add( DW1000_PIN_IRQ, gpio_isr, ( void * )DW1000_PIN_IRQ );
}
