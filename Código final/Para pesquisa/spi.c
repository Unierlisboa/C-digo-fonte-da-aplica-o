
#include "./../fw/fw.h"
#include "./../spi/spi.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SPI_HEADER_TAM_MAX		3
#define SPI_HEADER_WR_VAL		0x80
#define SPI_HEADER_RD_VAL		0x00
#define SPI_HEADER_SID_VAL		0x40
#define SPI_HEADER_EAD_VAL		0x80

#define SPI_SUB_REG_NONE		0xFF
#define SPI_SUB_REG_RANGE_VAL	128

spi_device_handle_t spi2_handler;
spi_transaction_t spi2_transaction;

spi_device_handle_t spi_inicializa( void )
{
	esp_err_t ret;
	spi_device_handle_t spi2;

	spi_bus_config_t buscfg={
		.miso_io_num = SPI_PIN_MISO,
		.mosi_io_num = SPI_PIN_MOSI,
		.sclk_io_num = SPI_PIN_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 10,
	};
	ret = spi_bus_initialize( SPI2_HOST, &buscfg, SPI_DMA_DISABLED );
	ESP_ERROR_CHECK( ret );

	spi_device_interface_config_t devcfg={
        .clock_speed_hz = 1000000,  // 1 MHz        .mode = 0,                  //SPI mode 0
        .spics_io_num = SPI_PIN_CS,
        .queue_size = 2,
        .flags = SPI_DEVICE_HALFDUPLEX,
        .pre_cb = NULL,
        .post_cb = NULL,
	};

	ESP_ERROR_CHECK( spi_bus_add_device( SPI2_HOST, &devcfg, &spi2 ) );

	return spi2;
}

void spi_leitura( spi_device_handle_t spi_handler, spi_transaction_t spi_transaction, uint8_t reg, uint16_t sub_reg, uint8_t spi_dado[], uint8_t spi_dado_qtd_bytes )
{
	uint8_t	spi_header[ SPI_HEADER_TAM_MAX ];
	uint8_t	spi_header_qtd_bytes;

	spi_header[ 0 ] = reg;
	if( sub_reg == SPI_SUB_REG_NONE )
	{
		spi_header[ 0 ] |= SPI_HEADER_RD_VAL;
		spi_header_qtd_bytes = 1;
	}
	else
	{
		spi_header[ 0 ] |= ( SPI_HEADER_RD_VAL | SPI_HEADER_SID_VAL );
		spi_header[ 1 ] = ( uint8_t )sub_reg;
		if ( sub_reg < SPI_SUB_REG_RANGE_VAL )
		{
			spi_header_qtd_bytes = 2;
		}
		else
		{
			spi_header[ 1 ] |= SPI_HEADER_EAD_VAL;
			spi_header[ 2 ] = ( uint8_t )( sub_reg >> 7 );
			spi_header_qtd_bytes = SPI_HEADER_TAM_MAX;
		}
	}

	spi_device_acquire_bus( spi_handler, portMAX_DELAY );

	spi_transaction.flags = SPI_TRANS_CS_KEEP_ACTIVE;
	spi_transaction.length = spi_header_qtd_bytes*8;
	spi_transaction.rxlength = 0;
	spi_transaction.tx_buffer = spi_header;
	spi_transaction.rx_buffer = NULL;
	spi_device_transmit( spi_handler, &spi_transaction );

	//spi_transaction.flags = SPI_TRANS_USE_RXDATA;
	spi_transaction.flags = SPI_TRANS_MODE_OCT;
	spi_transaction.length = 0;
	spi_transaction.rxlength = spi_dado_qtd_bytes*8;
	spi_transaction.tx_buffer = NULL;
	spi_transaction.rx_buffer = spi_dado;
	spi_device_transmit( spi_handler, &spi_transaction );

	spi_device_release_bus( spi_handler );

	vTaskDelay( 1 / portTICK_PERIOD_MS );
}

void spi_escrita( spi_device_handle_t spi_handler, spi_transaction_t spi_transaction, uint8_t reg, uint16_t sub_reg, uint8_t *spi_dado, uint8_t spi_dado_qtd_bytes )
{
	uint8_t	spi_header[ SPI_HEADER_TAM_MAX ];
	uint8_t	spi_header_qtd_bytes;

	spi_header[ 0 ] = reg;
	if( sub_reg == SPI_SUB_REG_NONE )
	{
		spi_header[ 0 ] |= SPI_HEADER_WR_VAL;
		spi_header_qtd_bytes = 1;
	}
	else
	{
		spi_header[ 0 ] |= ( SPI_HEADER_WR_VAL | SPI_HEADER_SID_VAL );
		spi_header[ 1 ] = ( uint8_t )sub_reg;
		if ( sub_reg < SPI_SUB_REG_RANGE_VAL )
		{
			spi_header_qtd_bytes = 2;
		}
		else
		{
			spi_header[ 1 ] |= SPI_HEADER_EAD_VAL;
			spi_header[ 2 ] = ( uint8_t )( sub_reg >> 7 );
			spi_header_qtd_bytes = SPI_HEADER_TAM_MAX;
		}
	}

	spi_device_acquire_bus( spi_handler, portMAX_DELAY );

	spi_transaction.flags = SPI_TRANS_CS_KEEP_ACTIVE;
	spi_transaction.length = spi_header_qtd_bytes*8;
	spi_transaction.rxlength = 0;
	spi_transaction.tx_buffer = spi_header;
	spi_transaction.rx_buffer = NULL;
	spi_device_transmit( spi_handler, &spi_transaction );

	spi_transaction.flags = 0;
	spi_transaction.length = spi_dado_qtd_bytes*8;
	spi_transaction.rxlength = 0;
	spi_transaction.tx_buffer = spi_dado;
	spi_transaction.rx_buffer = NULL;
	spi_device_transmit( spi_handler, &spi_transaction );

	spi_device_release_bus( spi_handler );

	vTaskDelay( 1 / portTICK_PERIOD_MS );
}
