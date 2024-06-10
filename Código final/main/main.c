
#include "../sw/sw.h"

void sw_processo_localizacao( void );
void sw_task_processo_localizacao( void *arg );

void app_main(void)
{
	fw_inicializa();

	#if ( SW_UWB == UWB_TAG )
		uwb.sm_localizacao = SM_LOCALIZACAO_TAG_AGUARDA;
	#elif ( SW_UWB == UWB_ANCORA )
		uwb.sm_localizacao = SM_LOCALIZACAO_ANC_RX;
		dw1000_habilita_rx();
	#endif

	xTaskCreatePinnedToCore( sw_task_processo_localizacao, "sw_task_processo_inicializacao", 2048, xTaskGetCurrentTaskHandle(), tskIDLE_PRIORITY, NULL, 0 );

	ESP_LOGI( TAG, "DW1000: ID %X - REV %X - RIDTAG %X - VER %X", uwb.dw1000.registro.dev_id.parametro.model, uwb.dw1000.registro.dev_id.parametro.rev, uwb.dw1000.registro.dev_id.parametro.ridtag, uwb.dw1000.registro.dev_id.parametro.ver );
}

void sw_task_processo_localizacao( void *arg )
{
    esp_task_wdt_add( NULL );

    while( TRUE )
    {
    	sw_processo_localizacao();
    	esp_task_wdt_reset();
    }

	esp_task_wdt_delete( NULL );
	xTaskNotifyGive( ( TaskHandle_t )arg );
    vTaskDelete( NULL );
}

void sw_processo_localizacao( void )
{
	uint8_t i;

	if( uwb.flags.dw1000_irq )
	{
		taskENTER_CRITICAL( &spinlock );
			uwb.flags.dw1000_irq = 0;
		taskEXIT_CRITICAL( &spinlock );

		dw1000_int_trata();

		for( i = 0; i < 5; i++ )
			ESP_LOGW(TAG, "INT = %X", uwb.dw1000.registro.sys_status.parametro.byte[i] );
	}

	if( uwb.flags.dw1000_tot )
	{
		timer_desliga();

		taskENTER_CRITICAL( &spinlock );
			uwb.flags.dw1000_tot = 0;
		taskEXIT_CRITICAL( &spinlock );

		uwb.dw1000.registro.sys_status.parametro.txfrs = 0;
		uwb.dw1000.registro.sys_status.parametro.rxdfr = 0;
		uwb.dw1000.registro.sys_status.parametro.rxfcg = 0;
		#if ( SW_UWB == UWB_TAG )
			dw1000_desabilita_rx();
			uwb.sm_localizacao = SM_LOCALIZACAO_TAG_AGUARDA;
		#elif ( SW_UWB == UWB_ANCORA )
			dw1000_habilita_rx();
			uwb.sm_localizacao = SM_LOCALIZACAO_ANC_RX;
		#endif

		ESP_LOGW( TAG, "TIMEOUT" );
	}


	switch ( uwb.sm_localizacao )
	{
		#if ( SW_UWB == UWB_TAG )
			case SM_LOCALIZACAO_TAG_AGUARDA:
				ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_AGUARDA" );
				vTaskDelay( 1000 / portTICK_PERIOD_MS );
				uwb.sm_localizacao = SM_LOCALIZACAO_TAG_TX1;
				break;

			case SM_LOCALIZACAO_TAG_TX1:
				ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_TX1" );
				uwb.dw1000.registro.sys_status.parametro.txfrs = 0;
				uwb.dw1000.tbuf[ 0 ] = 0xAA;
				uwb.dw1000.tbuf[ 1 ] = 0x55;
				uwb.dw1000.tbuf[ 2 ] = 0xAA;
				uwb.dw1000.tbuf[ 3 ] = 'T';
				uwb.dw1000.tbuf[ 4 ] = 'T';
				uwb.dw1000.tbuf[ 5 ] = '1';
				dw1000_transmite_dados( uwb.dw1000.tbuf, 6 );

				timer_liga();
				uwb.sm_localizacao = SM_LOCALIZACAO_TAG_CONFIRMA_TX1;
				break;

			case SM_LOCALIZACAO_TAG_CONFIRMA_TX1:
				if( uwb.dw1000.registro.sys_status.parametro.txfrs )
				{
					uwb.dw1000.registro.sys_status.parametro.txfrs = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_CONFIRMA_TX1" );
					uwb.medidas.t1 = dw1000_leitura_timestamp( DW1000_TXSTAMP );
					dw1000_habilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_ANC_ACK1;
				}
				break;

			case SM_LOCALIZACAO_ANC_ACK1:
				if( uwb.dw1000.registro.sys_status.parametro.rxdfr && uwb.dw1000.registro.sys_status.parametro.rxfcg )
				{
					uwb.dw1000.registro.sys_status.parametro.rxdfr = 0;
					uwb.dw1000.registro.sys_status.parametro.rxfcg = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_ACK1" );
					spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_BUFFER_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, 8 );
//					for( i = 0; i < 8 ; i++ )
//					{
//						ESP_LOGW(TAG, "end = %d valor = %x", i, uwb.dw1000.rbuf[ i ] );
//					}
					uwb.medidas.t2 = dw1000_leitura_timestamp( DW1000_RXSTAMP );
					dw1000_desabilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_TAG_TX2;
				}
				break;

			case SM_LOCALIZACAO_TAG_TX2:
				ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_TX2" );
				esp_timer_restart( uwb.timer.handler, 250000 );

				uwb.dw1000.registro.sys_status.parametro.txfrs = 0;
				uwb.dw1000.tbuf[ 0 ] = 0xAA;
				uwb.dw1000.tbuf[ 1 ] = 0x55;
				uwb.dw1000.tbuf[ 2 ] = 0xAA;
				uwb.dw1000.tbuf[ 3 ] = 'T';
				uwb.dw1000.tbuf[ 4 ] = 'T';
				uwb.dw1000.tbuf[ 5 ] = '2';
				dw1000_transmite_dados( uwb.dw1000.tbuf, 6 );

				uwb.sm_localizacao = SM_LOCALIZACAO_TAG_CONFIRMA_TX2;
				break;

			case SM_LOCALIZACAO_TAG_CONFIRMA_TX2:
				if( uwb.dw1000.registro.sys_status.parametro.txfrs )
				{
					uwb.dw1000.registro.sys_status.parametro.txfrs = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_CONFIRMA_TX2" );
					uwb.medidas.t3 = dw1000_leitura_timestamp( DW1000_TXSTAMP );
					dw1000_habilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_ANC_RESP;
				}
				break;

			case SM_LOCALIZACAO_ANC_RESP:
				if( uwb.dw1000.registro.sys_status.parametro.rxdfr && uwb.dw1000.registro.sys_status.parametro.rxfcg )
				{
					uwb.dw1000.registro.sys_status.parametro.rxdfr = 0;
					uwb.dw1000.registro.sys_status.parametro.rxfcg = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_RESP" );
					spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_BUFFER_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, 15 );
//					for( i = 0; i < 15 ; i++ )
//					{
//						ESP_LOGW(TAG, "end = %d valor = %x", i, uwb.dw1000.rbuf[ i ] );
//					}

					dw1000_desabilita_rx();
					timer_desliga();
					// faz calculo aqui
					uint8_t i;
					memset( ( Calculo_t *)&uwb.calculo, 0x00, sizeof( Calculo_t ) );
					for( i = 0; i < 5; i++ )
					{
						uwb.calculo.treply1.u8[ i ] = uwb.dw1000.rbuf[ i + 3 ];
						uwb.calculo.tround2.u8[ i ] = uwb.dw1000.rbuf[ i + 8 ];
					}

					//t2 > t1
					if( uwb.medidas.t2 > uwb.medidas.t1 )
					{
						uwb.calculo.tround1.u64  = uwb.medidas.t2;
						uwb.calculo.tround1.u64 -= uwb.medidas.t1;
					}
					else
					{
						uwb.calculo.tround1.u64  = ( uint64_t )0x10000000000;
						uwb.calculo.tround1.u64 -= uwb.medidas.t1;
						uwb.calculo.tround1.u64 += uwb.medidas.t2;
					}

					//t3 > t2
					if( uwb.medidas.t3 > uwb.medidas.t2 )
					{
						uwb.calculo.treply2.u64  = uwb.medidas.t3;
						uwb.calculo.treply2.u64 -= uwb.medidas.t2;
					}
					else
					{
						uwb.calculo.treply2.u64  = ( uint64_t )0x10000000000;
						uwb.calculo.treply2.u64 -= uwb.medidas.t2;
						uwb.calculo.treply2.u64 += uwb.medidas.t3;
					}

					// Nota: Em razão da parcela de multiplicação da fórmula o "timeout" deve ser limitado a 250 ms para manter o range do resultado em 64 bits
					uwb.calculo.tprop 		 = uwb.calculo.tround1.u64 * uwb.calculo.tround2.u64;
					uwb.calculo.tprop 		-= ( uwb.calculo.treply1.u64 * uwb.calculo.treply2.u64 );
					uwb.calculo.tprop 		/= ( uwb.calculo.tround1.u64 + uwb.calculo.tround2.u64 + uwb.calculo.treply1.u64 + uwb.calculo.treply2.u64 );
					uwb.calculo.distancia 	 = uwb.calculo.tprop;
					uwb.calculo.distancia 	/= 213.139451293;
					uwb.calculo.distancia 	-= 0.789055;//77.580872;

					ESP_LOGI(TAG, "Distancia = %f m", uwb.calculo.distancia );

					uwb.calculo.distancia = dw1000_potencia_rx();

					ESP_LOGI(TAG, "Potencia  = %f dBm", uwb.calculo.distancia );

					uwb.sm_localizacao = SM_LOCALIZACAO_TAG_AGUARDA;
				}
				break;
		#elif ( SW_UWB == UWB_ANCORA )
			case SM_LOCALIZACAO_ANC_RX:
				if( uwb.dw1000.registro.sys_status.parametro.rxdfr && uwb.dw1000.registro.sys_status.parametro.rxfcg )
				{
					uwb.dw1000.registro.sys_status.parametro.rxdfr = 0;
					uwb.dw1000.registro.sys_status.parametro.rxfcg = 0;

					ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_RX" );
					spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_BUFFER_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, 8 );
					for( i = 0; i < 8 ; i++ )
					{
						ESP_LOGW(TAG, "end = %d valor = %x", i, uwb.dw1000.rbuf[ i ] );
					}
					uwb.medidas.t1 = dw1000_leitura_timestamp( DW1000_RXSTAMP );
					dw1000_desabilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_ANC_TX;
				}
				break;

			case SM_LOCALIZACAO_ANC_TX:
				ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_TX" );

				uwb.dw1000.registro.sys_status.parametro.txfrs = 0;
				uwb.dw1000.tbuf[ 0 ] = 0xAA;
				uwb.dw1000.tbuf[ 1 ] = 0x55;
				uwb.dw1000.tbuf[ 2 ] = 0xAA;
				uwb.dw1000.tbuf[ 3 ] = 'A';
				uwb.dw1000.tbuf[ 4 ] = 'T';
				uwb.dw1000.tbuf[ 5 ] = '1';
				dw1000_transmite_dados( uwb.dw1000.tbuf, 6 );

				timer_liga();
				uwb.sm_localizacao = SM_LOCALIZACAO_ANC_CONFIRMA_TX;
				break;

			case SM_LOCALIZACAO_ANC_CONFIRMA_TX:
				if( uwb.dw1000.registro.sys_status.parametro.txfrs )
				{
					uwb.dw1000.registro.sys_status.parametro.txfrs = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_CONFIRMA_TX" );
					uwb.medidas.t2 = dw1000_leitura_timestamp( DW1000_TXSTAMP );
					dw1000_habilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_TAG_ACK;
				}
				break;

			case SM_LOCALIZACAO_TAG_ACK:
				if( uwb.dw1000.registro.sys_status.parametro.rxdfr && uwb.dw1000.registro.sys_status.parametro.rxfcg )
				{
					uwb.dw1000.registro.sys_status.parametro.rxdfr = 0;
					uwb.dw1000.registro.sys_status.parametro.rxfcg = 0;

					esp_timer_restart( uwb.timer.handler, 250000 );
					ESP_LOGW(TAG, "SM_LOCALIZACAO_TAG_ACK" );
					spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_BUFFER_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, 8 );
					for( i = 0; i < 8 ; i++ )
					{
						ESP_LOGW(TAG, "end = %d valor = %x", i, uwb.dw1000.rbuf[ i ] );
					}
					uwb.medidas.t3 = dw1000_leitura_timestamp( DW1000_RXSTAMP );
					dw1000_desabilita_rx();
					uwb.sm_localizacao = SM_LOCALIZACAO_ANC_RESP;
				}
				break;

			case SM_LOCALIZACAO_ANC_RESP:
				ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_RESP" );
				esp_timer_restart( uwb.timer.handler, 250000 );

				uwb.dw1000.registro.sys_status.parametro.txfrs = 0;
				uwb.dw1000.tbuf[ 0 ] = 0xAA;
				uwb.dw1000.tbuf[ 1 ] = 0x55;
				uwb.dw1000.tbuf[ 2 ] = 0xAA;

				uint8_t i;
				union {
					uint8_t		u8[ 8 ];
					uint64_t 	u64;
				} valor;

				//t2 > t1
				if( uwb.medidas.t2 > uwb.medidas.t1 )
				{
					valor.u64  = uwb.medidas.t2;
					valor.u64 -= uwb.medidas.t1;
				}
				else
				{
					valor.u64  = ( uint64_t )0x10000000000;
					valor.u64 -= uwb.medidas.t1;
					valor.u64 += uwb.medidas.t2;
				}

				for( i = 0; i < 5; i++ )
				{
					uwb.dw1000.tbuf[ i + 3 ] = valor.u8[ i ];
				}

				//t3 > t2
				if( uwb.medidas.t3 > uwb.medidas.t2 )
				{
					valor.u64  = uwb.medidas.t3;
					valor.u64 -= uwb.medidas.t2;
				}
				else
				{
					valor.u64  = ( uint64_t )0x10000000000;
					valor.u64 -= uwb.medidas.t2;
					valor.u64 += uwb.medidas.t3;
				}

				for( i = 0; i < 5; i++ )
				{
					uwb.dw1000.tbuf[ i + 8 ] = valor.u8[ i ];
				}

				dw1000_transmite_dados( uwb.dw1000.tbuf, 13 );

				uwb.sm_localizacao = SM_LOCALIZACAO_ANC_CONFIRMA_RESP;
				break;

			case SM_LOCALIZACAO_ANC_CONFIRMA_RESP:
					if( uwb.dw1000.registro.sys_status.parametro.txfrs )
					{
						uwb.dw1000.registro.sys_status.parametro.txfrs = 0;

						timer_desliga();
						ESP_LOGW(TAG, "SM_LOCALIZACAO_ANC_CONFIRMA_RESP" );

						dw1000_habilita_rx();
						uwb.sm_localizacao = SM_LOCALIZACAO_ANC_RX;
					}
					break;
		#endif

		default:
			ESP_LOGW(TAG, "FALHA NA MAQUINA DE ESTADOS" );
			break;

	}
}

