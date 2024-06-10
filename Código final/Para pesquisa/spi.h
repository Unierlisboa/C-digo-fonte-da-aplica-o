
#ifndef MAIN_SPI_SPI_H_
#define MAIN_SPI_SPI_H_

#include <driver/spi_master.h>

spi_device_handle_t spi_inicializa( void );
void spi_escrita( spi_device_handle_t spi_handler, spi_transaction_t spi_transaction, uint8_t reg, uint16_t sub_reg, uint8_t *spi_dado, uint8_t spi_dado_qtd_bytes );
void spi_leitura( spi_device_handle_t spi_handler, spi_transaction_t spi_transaction, uint8_t reg, uint16_t sub_reg, uint8_t spi_dado[], uint8_t spi_dado_qtd_bytes );

#endif /* MAIN_SPI_SPI_H_ */
