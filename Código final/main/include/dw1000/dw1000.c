
#include <math.h>
#include "../dw1000/dw1000.h"
#include "../fw/fw.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void dw1000_reset( uint16_t delay );
void dw1000_otp_leitura( uint16_t end, uint8_t dado[] );
void dw1000_carrega_microcodigo( void );

const unsigned char dw1000_enderecamento[ REG_QTD ] [ 2 ] = {  	// excluídos -> REG_TX_BUFFER - REG_RX_BUFFER - REG_ACC_MEM )
	{ REG_DEV_ID_END,		REG_DEV_ID_TAM_MAX		}, //0
	{ REG_EUI_END,			REG_EUI_TAM_MAX			}, //1
	{ REG_PANADR_END,		REG_PANADR_TAM_MAX		}, //2
	{ REG_SYS_CFG_END,		REG_SYS_CFG_TAM_MAX		}, //3
	{ REG_SYS_TIME_END,		REG_SYS_TIME_TAM_MAX	}, //4
	{ REG_TX_FCTRL_END,		REG_TX_FCTRL_TAM_MAX	}, //5
	{ REG_DX_TIME_END,		REG_DX_TIME_TAM_MAX		}, //6
	{ REG_RX_FWTO_END,		REG_RX_FWTO_TAM_MAX		}, //7
	{ REG_SYS_CTRL_END,		REG_SYS_CTRL_TAM_MAX	}, //8
	{ REG_SYS_MASK_END,		REG_SYS_MASK_TAM_MAX	}, //9
	{ REG_SYS_STATUS_END,	REG_SYS_STATUS_TAM_MAX	}, //A
	{ REG_RX_FINFO_END,		REG_RX_FINFO_TAM_MAX	}, //B
	{ REG_RX_FQUAL_END,		REG_RX_FQUAL_TAM_MAX	}, //C
	{ REG_RX_TTCKI_END,		REG_RX_TTCKI_TAM_MAX	}, //D
	{ REG_RX_TTCKO_END,		REG_RX_TTCKO_TAM_MAX	}, //E
	{ REG_RX_TIME_END,		REG_RX_TIME_TAM_MAX		}, //F
	{ REG_TX_TIME_END,		REG_TX_TIME_TAM_MAX		}, //10
	{ REG_TX_ANTD_END,		REG_TX_ANTD_TAM_MAX		}, //11
	{ REG_SYS_STATE_END,	REG_SYS_STATE_TAM_MAX	}, //12
	{ REG_ACK_RESP_T_END,	REG_ACK_RESP_T_TAM_MAX	}, //13
	{ REG_RX_SNIFF_END,		REG_RX_SNIFF_TAM_MAX	}, //14
	{ REG_TX_POWER_END,		REG_TX_POWER_TAM_MAX	}, //15
	{ REG_CHAN_CTRL_END,	REG_CHAN_CTRL_TAM_MAX	}, //16
	{ REG_USR_SFD_END,		REG_USR_SFD_TAM_MAX		}, //17
	{ REG_AGC_CTRL_END,		REG_AGC_CTRL_TAM_MAX	}, //18
	{ REG_EXT_SYNC_END,		REG_EXT_SYNC_TAM_MAX	}, //19
	{ REG_GPIO_CTRL_END,	REG_GPIO_CTRL_TAM_MAX	}, //1A
	{ REG_DRX_CONF_END,		REG_DRX_CONF_TAM_MAX	}, //1B
	{ REG_RF_CONF_END,		REG_RF_CONF_TAM_MAX		}, //1C
	{ REG_TX_CAL_END,		REG_TX_CAL_TAM_MAX		}, //1D
	{ REG_FS_CTRL_END,		REG_FS_CTRL_TAM_MAX		}, //1E
	{ REG_AON_END,			REG_AON_TAM_MAX			}, //1F
	{ REG_OTP_IF_END,		REG_OTP_IF_TAM_MAX		}, //20
	{ REG_LDE_CTRL_END,		REG_LDE_CTRL_TAM_MAX	}, //21
	{ REG_DIG_DIAG_END,		REG_DIG_DIAG_TAM_MAX	}, //22
	{ REG_PMSC_END,			REG_PMSC_TAM_MAX		}  //23
};

const char registro[ 36 ][ 19 ] = {
	"REG_DEV_ID_END",
	"REG_EUI_END",
	"REG_PANADR_END",
	"REG_SYS_CFG_END",
	"REG_SYS_TIME_END",
	"REG_TX_FCTRL_END",
	"REG_DX_TIME_END",
	"REG_RX_FWTO_END",
	"REG_SYS_CTRL_END",
	"REG_SYS_MASK_END",
	"REG_SYS_STATUS_END",
	"REG_RX_FINFO_END",
	"REG_RX_FQUAL_END",
	"REG_RX_TTCKI_END",
	"REG_RX_TTCKO_END",
	"REG_RX_TIME_END",
	"REG_TX_TIME_END",
	"REG_TX_ANTD_END",
	"REG_SYS_STATE_END",
	"REG_ACK_RESP_T_END",
	"REG_RX_SNIFF_END",
	"REG_TX_POWER_END",
	"REG_CHAN_CTRL_END",
	"REG_USR_SFD_END",
	"REG_AGC_CTRL_END",
	"REG_EXT_SYNC_END",
	"REG_GPIO_CTRL_END",
	"REG_DRX_CONF_END",
	"REG_RF_CONF_END",
	"REG_TX_CAL_END",
	"REG_FS_CTRL_END",
	"REG_AON_END",
	"REG_OTP_IF_END",
	"REG_LDE_CTRL_END",
	"REG_DIG_DIAG_END",
	"REG_PMSC_END"
};

void dw1000_inicializa( void )
{
	gpio_set_level( DW1000_PIN_WKP, 0 );
	dw1000_reset( 500 );

	#ifdef DEBUG_DW1000_CHIP_RESET_VAL
		uint16_t i;
		uint16_t j;
		for( i = 0; i < REG_QTD; i++ )
		{
			spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, dw1000_enderecamento[ i ][ 0 ], SUB_REG_NENHUM, uwb.dw1000.rbuf , dw1000_enderecamento[ i ][ 1 ] );
			ESP_LOGW(TAG, "i = %d reg = %s tamanho = %d", i, registro[ i ], dw1000_enderecamento[ i ][ 1 ] ); //dw1000_enderecamento[ i ][ 0 ], dw1000_enderecamento[ i ][ 1 ] );
			for( j = 0; j < dw1000_enderecamento[ i ][ 1 ]; j++ )
			{
				ESP_LOGW(TAG, "end = %x valor = %x", j, uwb.dw1000.rbuf[ j ] );
			}
		}
	#endif

	// SYS_CTRL
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.trxoff = 1;			// Transceiver off, idle mode
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	vTaskDelay( 1 / portTICK_PERIOD_MS );

	// Carrega micro código
	dw1000_carrega_microcodigo();
	vTaskDelay( 5 / portTICK_PERIOD_MS );

//	// PMSC
//	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_PMSC_CTRL0_END, uwb.dw1000.registro.pmsc.parametro.byte, SUB_REG_PMSC_CTRL0_TAM_MAX );
//	uwb.dw1000.registro.pmsc.parametro.sysclks			= 0;		// Auto – The system clock will run off the 19.2 MHz XTI clock until the PLL is calibrated and locked, then it will switch over the 125 MHz PLL clock
//	uwb.dw1000.registro.pmsc.parametro.gpce 			= 0;		// GPIO clock disable
//	uwb.dw1000.registro.pmsc.parametro.gprn 			= 0;		// GPIO reset (NOT), active low
//	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_PMSC_CTRL0_END, uwb.dw1000.registro.pmsc.parametro.byte, SUB_REG_PMSC_CTRL0_TAM_MAX );


//	uwb.dw1000.tbuf[ 0 ] = 0x0000;		// PMSC fine grain TX sequencing control (TX fine grain power sequencing must be disabled if an external power amplifier is being used with the DW1000)
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_PMSC_TXFSEQ_END, uwb.dw1000.tbuf, SUB_REG_PMSC_TXFSEQ_TAM_MAX );

	// OTP_ROM
	dw1000_otp_leitura( 0x04, uwb.dw1000.rbuf );
	if( uwb.dw1000.rbuf[ 0 ] != 0 )
	{
		uwb.dw1000.tbuf[ 0 ] = uwb.dw1000.rbuf[ 0 ];
		uwb.dw1000.tbuf[ 1 ] = uwb.dw1000.rbuf[ 1 ];
		uwb.dw1000.tbuf[ 2 ] = uwb.dw1000.rbuf[ 2 ];
		uwb.dw1000.tbuf[ 3 ] = uwb.dw1000.rbuf[ 3 ];

		dw1000_otp_leitura( 0x05, uwb.dw1000.rbuf );
		uwb.dw1000.tbuf[ 4 ] = uwb.dw1000.rbuf[ 0 ];

		spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RF_CONF_END, SUB_REG_LDOTUNE_END, uwb.dw1000.tbuf, SUB_REG_LDOTUNE_TAM_MAX );
	}
	dw1000_otp_leitura( REG_OTP_VMEAS_3V3_END, uwb.dw1000.rbuf );
	uwb.dw1000.vmeas3V3 = uwb.dw1000.rbuf[ 0 ];
	dw1000_otp_leitura( REG_OTP_TMEAS_23C_END, uwb.dw1000.rbuf );
	uwb.dw1000.tmeas23C = uwb.dw1000.rbuf[ 0 ];

	// SYS_CFG
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );
	uwb.dw1000.registro.sys_cfg.parametro.hirq_pol	= 1;		// Host interrupt polarity active high
	uwb.dw1000.registro.sys_cfg.parametro.dis_drxb 	= 1;		// Disable Double RX Buffer
	uwb.dw1000.registro.sys_cfg.parametro.dis_stxp 	= 1;		// Disable Smart TX Power control
	//uwb.dw1000.registro.sys_cfg.parametro.rxm110k	= 1;		// Receiver Mode 110 kbps data rate
	uwb.dw1000.registro.sys_cfg.parametro.rxautr	= 1;		// Receiver Auto-Re-enable
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );

	// SYS_STATUS
	uwb.dw1000.tbuf[ 0 ] = 0xFF;
	uwb.dw1000.tbuf[ 1 ] = 0xFF;
	uwb.dw1000.tbuf[ 2 ] = 0xFF;
	uwb.dw1000.tbuf[ 3 ] = 0xFF;
	uwb.dw1000.tbuf[ 4 ] = 0xFF;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_STATUS_END, SUB_REG_NENHUM, uwb.dw1000.tbuf, REG_SYS_STATUS_TAM_MAX );

	// SYS_MASK
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_MASK_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_mask.parametro.byte, REG_SYS_MASK_TAM_MAX );
	uwb.dw1000.registro.sys_mask.parametro.maat		= 1;		// Mask automatic acknowledge trigger event
	uwb.dw1000.registro.sys_mask.parametro.mtxfrb	= 1;		// Mask transmit frame begins event
	uwb.dw1000.registro.sys_mask.parametro.mtxprs	= 1;		// Mask transmit preamble sent event
	uwb.dw1000.registro.sys_mask.parametro.mtxphs	= 1;		// Mask transmit PHY Header Sent event
	uwb.dw1000.registro.sys_mask.parametro.mtxfrs	= 1;		// Mask transmit frame sent event
	uwb.dw1000.registro.sys_mask.parametro.mldedon	= 1;		// Mask LDE processing done event
	uwb.dw1000.registro.sys_mask.parametro.mrxphe	= 1;		// Mask receiver PHY header error event
	uwb.dw1000.registro.sys_mask.parametro.mrxdfr	= 1;		// Mask receiver data frame ready event
	uwb.dw1000.registro.sys_mask.parametro.mrxfcg	= 1;		// Mask receiver FCS good event
	uwb.dw1000.registro.sys_mask.parametro.mrxfce	= 1;		// Mask receiver FCS error event
	uwb.dw1000.registro.sys_mask.parametro.mrxrfsl	= 1;		// Mask receiver Reed Solomon Frame Sync Loss event
	uwb.dw1000.registro.sys_mask.parametro.mldeerr	= 1;		// Mask leading edge detection processing error event
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_MASK_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_mask.parametro.byte, REG_SYS_MASK_TAM_MAX );

	//	// PANADR
	//	uwb.dw1000.registro.panadr.parametro.byte[ 0 ] = ( uint8_t )DW1000_SHORT_ADDR_VAL;
	//	uwb.dw1000.registro.panadr.parametro.byte[ 1 ] = ( uint8_t )( DW1000_SHORT_ADDR_VAL >> 8 );
	//	uwb.dw1000.registro.panadr.parametro.byte[ 2 ] = ( uint8_t )DW1000_PAN_ID_VAL;
	//	uwb.dw1000.registro.panadr.parametro.byte[ 3 ] = ( uint8_t )( DW1000_PAN_ID_VAL >> 8 );
	//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PANADR_END, SUB_REG_NENHUM, uwb.dw1000.registro.panadr.parametro.byte, uwb.dw1000.registro.panadr.qtd_bytes );

	// TX_FCTRL
//	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_FCTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_fctrl.parametro.byte, REG_TX_FCTRL_TAM_MAX );
//	uwb.dw1000.registro.tx_fctrl.parametro.txbr		= DW1000_TRX_RATE_110KBPS;			// Transmit Bit Rate
//	uwb.dw1000.registro.tx_fctrl.parametro.txprf	= DW1000_TX_PULSE_FREQ_16MHZ;		// Transmit Pulse Repetition Frequency
//	uwb.dw1000.registro.tx_fctrl.parametro.txpsr	= DW1000_TX_PREAM_LEN_2048;			// Transmit Preamble Symbol Repetitions
//	uwb.dw1000.registro.tx_fctrl.parametro.pe		= DW1000_TX_PREAM_LEN_2048;			// Preamble Extension
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_FCTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_fctrl.parametro.byte, REG_TX_FCTRL_TAM_MAX );

	// CHAN_CTRL
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_CHAN_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.chan_ctrl.parametro.byte, REG_CHAN_CTRL_TAM_MAX );
//	uwb.dw1000.registro.chan_ctrl.parametro.tx_chan		= DW1000_CHANNEL_5;				// Selects the transmit channel
//	uwb.dw1000.registro.chan_ctrl.parametro.rx_chan		= DW1000_CHANNEL_5;				// Selects the receive channel
//	uwb.dw1000.registro.chan_ctrl.parametro.dwsfd		= 1;							// Enables a non-standard Decawave proprietary SFD sequence
//	uwb.dw1000.registro.chan_ctrl.parametro.rxprf		= DW1000_TX_PULSE_FREQ_16MHZ;	// Selects the PRF used in the receiver
//	uwb.dw1000.registro.chan_ctrl.parametro.tnssfd		= 0;							// Disables the use of a user specified (non-standard) SFD in the transmitter
//	uwb.dw1000.registro.chan_ctrl.parametro.rnssfd		= 0;							// Disables the use of a user specified (non-standard) SFD in the receiver
	uwb.dw1000.registro.chan_ctrl.parametro.tx_pcode	= DW1000_PREAM_CODE_16MHZ_4;	// Selects the preamble code used in the transmitter
	uwb.dw1000.registro.chan_ctrl.parametro.rx_pcode	= DW1000_PREAM_CODE_16MHZ_4;	// Selects the preamble code used in the receiver
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_CHAN_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.chan_ctrl.parametro.byte, REG_CHAN_CTRL_TAM_MAX );

	// USR_SFD
//	uwb.dw1000.tbuf[ 0 ] = 0x10;//0x40;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_USR_SFD_END, SUB_REG_NENHUM, uwb.dw1000.tbuf, 1 );

	// AGC_CTRL
	uwb.dw1000.tbuf[ 0 ] = 0x70;		// AGC Tuning register 1 (16 MHz PRF)
	uwb.dw1000.tbuf[ 1 ] = 0x88;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_AGC_CTRL_END, SUB_REG_AGC_TUNE1_END, uwb.dw1000.tbuf, SUB_REG_AGC_TUNE1_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0x07;		// AGC Tuning register 2
	uwb.dw1000.tbuf[ 1 ] = 0xA9;
	uwb.dw1000.tbuf[ 2 ] = 0x02;
	uwb.dw1000.tbuf[ 3 ] = 0x25;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_AGC_CTRL_END, SUB_REG_AGC_TUNE2_END, uwb.dw1000.tbuf, SUB_REG_AGC_TUNE2_TAM_MAX );

//	uwb.dw1000.tbuf[ 0 ] = 0x35;		// AGC Tuning register 3
//	uwb.dw1000.tbuf[ 1 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_AGC_CTRL_END, SUB_REG_AGC_TUNE3_END, uwb.dw1000.tbuf, SUB_REG_AGC_TUNE3_TAM_MAX );

	// DRX_CONF
//	uwb.dw1000.tbuf[ 0 ] = 0x16;		// Digital Tuning Register 0B (110 kbps, Non-Standard SFD)
//	uwb.dw1000.tbuf[ 1 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DRX_CONF_END, SUB_REG_DRX_TUNE0b_END, uwb.dw1000.tbuf, SUB_REG_DRX_TUNE0b_TAM_MAX );

//	uwb.dw1000.tbuf[ 0 ] = 0x87;		// Digital Tuning Register 1A (64 MHz PRF)
//	uwb.dw1000.tbuf[ 1 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DRX_CONF_END, SUB_REG_DRX_TUNE1a_END, uwb.dw1000.tbuf, SUB_REG_DRX_TUNE1a_TAM_MAX );

//	uwb.dw1000.tbuf[ 0 ] = 0x64;		// 2048 symbols, for 110 kbps operation (no use case)
//	uwb.dw1000.tbuf[ 1 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DRX_CONF_END, SUB_REG_DRX_TUNE1b_END, uwb.dw1000.tbuf, SUB_REG_DRX_TUNE1b_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0x2D;//0x1D;		// Digital Tuning Register 2 (PAC size 64, 16 MHz PRF)
	uwb.dw1000.tbuf[ 1 ] = 0x00;//0x01;
	uwb.dw1000.tbuf[ 2 ] = 0x1A;
	uwb.dw1000.tbuf[ 3 ] = 0x31;//0x37;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DRX_CONF_END, SUB_REG_DRX_TUNE2_END, uwb.dw1000.tbuf, SUB_REG_DRX_TUNE2_TAM_MAX );

//	uwb.dw1000.tbuf[ 0 ] = 0x28;		// Digital Tuning Register 4H (Expected Receive Preamble Length in Symbols 128 or greater)
//	uwb.dw1000.tbuf[ 1 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DRX_CONF_END, SUB_REG_DRX_TUNE4H_END, uwb.dw1000.tbuf, SUB_REG_DRX_TUNE4H_TAM_MAX );

	// LDE_CTRL
//	uwb.dw1000.tbuf[ 0 ] = 0x0D;		// Noise Threshold Multiplier (More accuracy in close-up LOS conditions)
//	uwb.dw1000.tbuf[ 0 ] |= 0x60;		// Peak Multiplier (By default this value is 3, giving a factor of 1.5)
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_LDE_CTRL_END, SUB_REG_LDE_CFG1_END, uwb.dw1000.tbuf, SUB_REG_LDE_CFG1_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0x07;		// LDE Configuration Register 2 (16 MHz PRF)
	uwb.dw1000.tbuf[ 1 ] = 0x16;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_LDE_CTRL_END, SUB_REG_LDE_CFG2_END, uwb.dw1000.tbuf, SUB_REG_LDE_CFG2_TAM_MAX );

//	uwb.dw1000.tbuf[ 0 ] = 0x51;		// LDE Replica Coefficient configuration (RX_PCODE4 0x428E div 8)
//	uwb.dw1000.tbuf[ 1 ] = 0x08;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_LDE_CTRL_END, SUB_REG_LDE_REPC_END, uwb.dw1000.tbuf, SUB_REG_LDE_REPC_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0x8E;		// LDE Replica Coefficient configuration (RX_PCODE4 0x428E)
	uwb.dw1000.tbuf[ 1 ] = 0x42;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_LDE_CTRL_END, SUB_REG_LDE_REPC_END, uwb.dw1000.tbuf, SUB_REG_LDE_REPC_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = ( uint8_t )DW1000_ANTENNA_CAL;		// LDE Receive Antenna Delay configuration
	uwb.dw1000.tbuf[ 1 ] = ( DW1000_ANTENNA_CAL >> 8 );
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_LDE_CTRL_END, SUB_REG_LDE_RXANTD_END, uwb.dw1000.tbuf, SUB_REG_LDE_RXANTD_TAM_MAX );

	// TX_POWER
	uwb.dw1000.registro.tx_power.parametro.boostnorm_na			= 0x0E;		// Transmit Power Control (TX Channel 5, 16 MHz and DIS_STXP = 1)
	uwb.dw1000.registro.tx_power.parametro.boostp250_txpowsd	= 0x1F;
	uwb.dw1000.registro.tx_power.parametro.boostp500_txpowphr	= 0x1F;
	uwb.dw1000.registro.tx_power.parametro.boostp125_na 		= 0x22;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_POWER_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_power.parametro.byte, REG_TX_POWER_TAM_MAX );

	// RF_CONF
//	uwb.dw1000.tbuf[ 0 ] = 0xD8;		// Analog RX Control Register (RX Channel 5)
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RF_CONF_END, SUB_REG_RF_RXCTRLH_END, uwb.dw1000.tbuf, SUB_REG_RF_RXCTRLH_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0xE3;		// Analog TX Control Register (TX Channel 5)
	uwb.dw1000.tbuf[ 1 ] = 0x3F;
	uwb.dw1000.tbuf[ 2 ] = 0x13;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RF_CONF_END, SUB_REG_RF_TXCTRL_END, uwb.dw1000.tbuf, 3 );

	// TX_CAL
	uwb.dw1000.tbuf[ 0 ] = 0xB5;		// Transmitter Calibration – Pulse Generator Delay
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_CAL_END, SUB_REG_TC_PGDELAY_END, uwb.dw1000.tbuf, SUB_REG_TC_PGDELAY_TAM_MAX );

	// FS_CTRL
//	uwb.dw1000.tbuf[ 0 ] = 0x1D;		// Frequency synthesiser – PLL Coonfiguration (Operating Channel 5)
//	uwb.dw1000.tbuf[ 1 ] = 0x04;
//	uwb.dw1000.tbuf[ 2 ] = 0x00;
//	uwb.dw1000.tbuf[ 3 ] = 0x08;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_FS_CTRL_END, SUB_REG_FS_PLLCFG_END, uwb.dw1000.tbuf, SUB_REG_FS_PLLCFG_TAM_MAX );

	uwb.dw1000.tbuf[ 0 ] = 0xBE;		// Frequency synthesiser – PLL Tuning (Operating Channel 5)
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_FS_CTRL_END, SUB_REG_FS_PLLTUNE_END, uwb.dw1000.tbuf, SUB_REG_FS_PLLTUNE_TAM_MAX );

//	dw1000_otp_leitura( 0x01E, uwb.dw1000.rbuf );		// IC Calibration – Crystal Oscillator Trim
//	uwb.dw1000.tbuf[ 0 ] = ( uwb.dw1000.rbuf[ 0 ] == 0 )? 0x10 | 0x60: ( uwb.dw1000.rbuf[ 0 ] & 0x1F ) | 0x60;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_FS_CTRL_END, SUB_REG_FS_XTALT_END, uwb.dw1000.tbuf, SUB_REG_FS_XTALT_TAM_MAX );

	// TX_ANTD
	uwb.dw1000.registro.tx_antd.parametro.delay = DW1000_ANTENNA_CAL;		// 16-bit Delay from Transmit to Antenna
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_ANTD_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_antd.parametro.byte, REG_TX_ANTD_TAM_MAX );

//	// GPIO_CTRL
//	uwb.dw1000.tbuf[ 0 ] = 0x00;
//	uwb.dw1000.tbuf[ 1 ] = 0x40;		// The pin operates as the EXTPA output
//	uwb.dw1000.tbuf[ 2 ] = 0x05;		// The pin operates as the EXTTXE and EXTRXE output
//	uwb.dw1000.tbuf[ 3 ] = 0x00;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_GPIO_CTRL_END, SUB_REG_GPIO_MODE_END, uwb.dw1000.tbuf, SUB_REG_GPIO_MODE_TAM_MAX );

	// SYS_STATUS
	uwb.dw1000.tbuf[ 0 ] = 0xFF;
	uwb.dw1000.tbuf[ 1 ] = 0xFF;
	uwb.dw1000.tbuf[ 2 ] = 0xFF;
	uwb.dw1000.tbuf[ 3 ] = 0xFF;
	uwb.dw1000.tbuf[ 4 ] = 0xFF;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_STATUS_END, SUB_REG_NENHUM, uwb.dw1000.tbuf, REG_SYS_STATUS_TAM_MAX );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_DEV_ID_END, SUB_REG_NENHUM, uwb.dw1000.registro.dev_id.parametro.byte, REG_DEV_ID_TAM_MAX );
}

void dw1000_reset( uint16_t delay )
{
	gpio_set_direction( DW1000_PIN_RST, GPIO_MODE_OUTPUT );
	gpio_set_level( DW1000_PIN_RST, 0 );
	vTaskDelay( delay / portTICK_PERIOD_MS );
	gpio_set_direction( DW1000_PIN_RST, GPIO_MODE_INPUT );
	vTaskDelay( 10 / portTICK_PERIOD_MS );
}

void dw1000_otp_leitura( uint16_t end, uint8_t dado[] )
{
	uint8_t endereco[ SUB_REG_OTP_ADDR_TAM_MAX ];
	uint8_t buffer[ SUB_REG_OTP_RDAT_TAM_MAX ];

	endereco[ 0 ] = ( uint8_t )end;
	endereco[ 1 ] = ( uint8_t )( end >> 8 );
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_ADDR_END, endereco, SUB_REG_OTP_ADDR_TAM_MAX );

	buffer[ 0 ] = 0x03;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_CTRL_END, buffer, 1 );

	buffer[ 0 ] = 0x01;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_CTRL_END, buffer, 1 );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_RDAT_END, dado, SUB_REG_OTP_RDAT_TAM_MAX );

	buffer[ 0 ] = 0x00;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_CTRL_END, buffer, 1 );
}

void dw1000_carrega_microcodigo( void )
{
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_NENHUM, uwb.dw1000.registro.pmsc.parametro.byte, REG_PMSC_TAM_MAX );
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_NENHUM, uwb.dw1000.registro.otp_if.parametro.byte, REG_OTP_IF_TAM_MAX );

	uwb.dw1000.registro.pmsc.parametro.byte[ 0 ]	= 0x01;		// Force system clock to be the 19.2 MHz XTI clock
	uwb.dw1000.registro.pmsc.parametro.byte[ 1 ] 	= 0x03;		// Load microcode
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_PMSC_CTRL0_END, uwb.dw1000.registro.pmsc.parametro.byte, SUB_REG_PMSC_CTRL0_TAM_MAX );

	uwb.dw1000.registro.otp_if.parametro.byte[ 0 ]	= 0x00;
	uwb.dw1000.registro.otp_if.parametro.byte[ 1 ]	= 0x80;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_OTP_IF_END, SUB_REG_OTP_CTRL_END, uwb.dw1000.registro.otp_if.parametro.byte, SUB_REG_OTP_CTRL_TAM_MAX );

	vTaskDelay( 1 / portTICK_PERIOD_MS );

	uwb.dw1000.registro.pmsc.parametro.byte[ 0 ]	= 0x00;		// 19.2 MHz XTI clock until the PLL is calibrated and locked, then it will switch over the 125 MHz PLL clock
	uwb.dw1000.registro.pmsc.parametro.byte[ 1 ]	= 0x02;		// Default state
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_PMSC_END, SUB_REG_PMSC_CTRL0_END, uwb.dw1000.registro.pmsc.parametro.byte, SUB_REG_PMSC_CTRL0_TAM_MAX );
}

void dw1000_int_trata( void )
{
	// Obtém flags de int
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_STATUS_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, REG_SYS_STATUS_TAM_MAX );
	if(  ( uwb.dw1000.rbuf[ 0 ] & 0x01 ) == 1 )
	{
		uwb.dw1000.registro.sys_status.parametro.byte[ 0 ] = uwb.dw1000.rbuf[ 0 ];
		uwb.dw1000.registro.sys_status.parametro.byte[ 1 ] = uwb.dw1000.rbuf[ 1 ];
		uwb.dw1000.registro.sys_status.parametro.byte[ 2 ] = uwb.dw1000.rbuf[ 2 ];
		uwb.dw1000.registro.sys_status.parametro.byte[ 3 ] = uwb.dw1000.rbuf[ 3 ];
		uwb.dw1000.registro.sys_status.parametro.byte[ 4 ] = uwb.dw1000.rbuf[ 4 ];

		// Escrita para apagar flags acionados
		spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_STATUS_END, SUB_REG_NENHUM, uwb.dw1000.rbuf, REG_SYS_STATUS_TAM_MAX );
	}
}

void dw1000_transmite_dados( uint8_t *dados, uint8_t qtd_bytes )
{
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.trxoff 	= 1;									// Transceiver off, idle mode
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );

	vTaskDelay( 1 / portTICK_PERIOD_MS );

	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_BUFFER_END, SUB_REG_NENHUM, dados, ( qtd_bytes + 2 ) );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_FCTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_fctrl.parametro.byte, REG_TX_FCTRL_TAM_MAX );
	uwb.dw1000.registro.tx_fctrl.parametro.tflen 	= qtd_bytes + 2;
	uwb.dw1000.registro.tx_fctrl.parametro.tfle		= 0;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_FCTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_fctrl.parametro.byte, REG_TX_FCTRL_TAM_MAX );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.sfcst 	= 0;
	uwb.dw1000.registro.sys_ctrl.parametro.txstrt	= 1;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
}

void dw1000_habilita_rx( void )
{
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.trxoff 	= 1;									// Transceiver off, idle mode
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );

	vTaskDelay( 1 / portTICK_PERIOD_MS );

//	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );
//	uwb.dw1000.registro.sys_cfg.parametro.rxautr	= 1;
//	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.sfcst 	= 0;
	uwb.dw1000.registro.sys_ctrl.parametro.rxenab	= 1;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
}

void dw1000_desabilita_rx( void )
{
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );
	uwb.dw1000.registro.sys_ctrl.parametro.trxoff 	= 1;									// Transceiver off, idle mode
	uwb.dw1000.registro.sys_ctrl.parametro.sfcst 	= 0;
	uwb.dw1000.registro.sys_ctrl.parametro.rxenab	= 0;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CTRL_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_ctrl.parametro.byte, REG_SYS_CTRL_TAM_MAX );

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );
	uwb.dw1000.registro.sys_cfg.parametro.rxautr	= 0;
	spi_escrita( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_SYS_CFG_END, SUB_REG_NENHUM, uwb.dw1000.registro.sys_cfg.parametro.byte, REG_SYS_CFG_TAM_MAX );

	vTaskDelay( 1 / portTICK_PERIOD_MS );
}


uint64_t dw1000_leitura_timestamp( uint8_t tipo )
{
	uint64_t dado = 0;

	if( tipo == DW1000_TXSTAMP )
	{
		spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_TX_TIME_END, SUB_REG_NENHUM, uwb.dw1000.registro.tx_time.parametro.byte, REG_TX_TIME_TAM_MAX );
		dado  = uwb.dw1000.registro.tx_time.parametro.tx_stamp_low;
		dado |= ( ( uint64_t )uwb.dw1000.registro.tx_time.parametro.tx_stamp_high << 32 );
	}
	else if ( tipo == DW1000_RXSTAMP )
	{
		spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_TIME_END, SUB_REG_NENHUM, uwb.dw1000.registro.rx_time.parametro.byte, REG_RX_TIME_TAM_MAX );
		dado  = uwb.dw1000.registro.rx_time.parametro.rx_stamp_low;
		dado |= ( ( uint64_t )uwb.dw1000.registro.rx_time.parametro.rx_stamp_high << 32 );
	}

	return dado;
}

float dw1000_potencia_rx( void )
{
	float A, k, potencia;
	uint32_t C, N;

	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_FQUAL_END, SUB_REG_NENHUM, uwb.dw1000.registro.rx_fqual.parametro.byte, REG_RX_FQUAL_TAM_MAX );
	spi_leitura( uwb.spi.spi2_handler, uwb.spi.spi2_transaction, REG_RX_FINFO_END, SUB_REG_NENHUM, uwb.dw1000.registro.rx_finfo.parametro.byte, REG_RX_FINFO_TAM_MAX );

	A = 113.77;
	C = uwb.dw1000.registro.rx_fqual.parametro.cir_pwr;
	N = uwb.dw1000.registro.rx_finfo.parametro.rxpacc;
	k = 2.3334;

	potencia = 10.0 * log10( ( ( float )C * ( float )131072.0) / ( (float )N * ( float )N ) ) - A;
	if ( potencia  > -88 )
	{
		potencia += ( potencia + 88 ) * k;
	}

	return potencia;
}
