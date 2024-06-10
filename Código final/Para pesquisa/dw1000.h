
#ifndef MAIN_UWB_UWB_H_
#define MAIN_UWB_UWB_H_

#include "./../gpio/gpio.h"
#include "./../spi/spi.h"

// Registradores
#define REG_QTD				36		// 39 - 3 ( REG_TX_BUFFER - REG_RX_BUFFER - REG_ACC_MEM )
#define REG_DEV_ID_END		0x00	// Device Identifier – includes device type and revision info
#define REG_EUI_END 		0x01 	// Extended Unique Identifier
#define REG_PANADR_END 		0x03 	// PAN Identifier and Short Address
#define REG_SYS_CFG_END		0x04 	// System Configuration bitmap
#define REG_SYS_TIME_END	0x06 	// System Time Counter (40-bit)
#define REG_TX_FCTRL_END	0x08 	// Transmit Frame Control
#define REG_TX_BUFFER_END	0x09 	// Transmit Data Buffer
#define REG_DX_TIME_END 	0x0A 	// Delayed Send or Receive Time (40-bit)
#define REG_RX_FWTO_END 	0x0C 	// Receive Frame Wait Timeout Period
#define REG_SYS_CTRL_END 	0x0D 	// System Control Register
#define REG_SYS_MASK_END 	0x0E 	// System Event Mask Register
#define REG_SYS_STATUS_END 	0x0F 	// System Event Status Register
#define REG_RX_FINFO_END 	0x10 	// RX Frame Information
#define REG_RX_BUFFER_END 	0x11 	// Receive Data
#define REG_RX_FQUAL_END 	0x12 	// Rx Frame Quality information
#define REG_RX_TTCKI_END 	0x13 	// Receiver Time Tracking Interval
#define REG_RX_TTCKO_END	0x14 	// Receiver Time Tracking Offset
#define	REG_RX_TIME_END		0x15	// Receive Message Time of Arrival
#define	REG_TX_TIME_END		0x17	// Transmit Message Time of Sending
#define	REG_TX_ANTD_END		0x18 	// 16-bit Delay from Transmit to Antenna
#define REG_SYS_STATE_END 	0x19 	// System State information
#define REG_ACK_RESP_T_END	0x1A 	// Acknowledgement Time and Response Time
#define REG_RX_SNIFF_END 	0x1D 	// Pulsed Preamble Reception Configuration
#define REG_TX_POWER_END 	0x1E 	// TX Power Control
#define REG_CHAN_CTRL_END 	0x1F 	// Channel Control
#define REG_USR_SFD_END 	0x21 	// User-specified short/long TX/RX SFD sequences
#define REG_AGC_CTRL_END 	0x23 	// Automatic Gain Control configuration
#define REG_EXT_SYNC_END 	0x24 	// External synchronisation control.
#define REG_ACC_MEM_END 	0x25 	// Read access to accumulator data
#define REG_GPIO_CTRL_END 	0x26 	// Peripheral register bus 1 access – GPIO control
#define REG_DRX_CONF_END 	0x27 	// Digital Receiver configuration
#define REG_RF_CONF_END 	0x28 	// Analog RF Configuration
#define REG_TX_CAL_END		0x2A 	// Transmitter calibration block
#define REG_FS_CTRL_END 	0x2B 	// Frequency synthesiser control block
#define REG_AON_END			0x2C 	// Always-On register set
#define REG_OTP_IF_END		0x2D 	// One Time Programmable Memory Interface
#define REG_LDE_CTRL_END	0x2E 	// Leading edge detection control block
#define REG_DIG_DIAG_END	0x2F 	// Digital Diagnostics Interface
#define REG_PMSC_END		0x36 	// Power Management System Control Block

#define REG_DEV_ID_TAM_MAX		4
#define REG_EUI_TAM_MAX			8
#define REG_PANADR_TAM_MAX		4
#define REG_SYS_CFG_TAM_MAX		4
#define REG_SYS_TIME_TAM_MAX	5
#define REG_TX_FCTRL_TAM_MAX	5
#define REG_TX_BUFFER_TAM_MAX	1024
#define REG_DX_TIME_TAM_MAX		5
#define REG_RX_FWTO_TAM_MAX		4		// 2 bytes "zerados" + 2 bytes de dados
#define REG_SYS_CTRL_TAM_MAX	4
#define REG_SYS_MASK_TAM_MAX 	4
#define REG_SYS_STATUS_TAM_MAX	5
#define REG_RX_FINFO_TAM_MAX	4
#define REG_RX_BUFFER_TAM_MAX	1024
#define REG_RX_FQUAL_TAM_MAX	8
#define REG_RX_TTCKI_TAM_MAX	4
#define REG_RX_TTCKO_TAM_MAX	5
#define REG_RX_TIME_TAM_MAX		14
#define REG_TX_TIME_TAM_MAX		10
#define REG_TX_ANTD_TAM_MAX		2
#define REG_SYS_STATE_TAM_MAX	4
#define REG_ACK_RESP_T_TAM_MAX	4
#define REG_RX_SNIFF_TAM_MAX	4
#define REG_TX_POWER_TAM_MAX	4
#define REG_CHAN_CTRL_TAM_MAX	4
#define REG_USR_SFD_TAM_MAX		41
#define REG_AGC_CTRL_TAM_MAX	33
#define REG_EXT_SYNC_TAM_MAX	12
#define REG_ACC_MEM_TAM_MAX		4064
#define REG_GPIO_CTRL_TAM_MAX	44
#define REG_DRX_CONF_TAM_MAX	45
#define REG_RF_CONF_TAM_MAX		45
#define REG_TX_CAL_TAM_MAX		12
#define REG_FS_CTRL_TAM_MAX		21
#define REG_AON_TAM_MAX			12
#define REG_OTP_IF_TAM_MAX		19
#define REG_LDE_CTRL_TAM_MAX	13
#define REG_DIG_DIAG_TAM_MAX	38
#define REG_PMSC_TAM_MAX		41
#define REG_TAM_MAX	(																																\
	  REG_DEV_ID_TAM_MAX + REG_EUI_TAM_MAX + REG_PANADR_TAM_MAX + REG_SYS_CFG_TAM_MAX + REG_SYS_TIME_TAM_MAX + REG_TX_FCTRL_TAM_MAX  				\
	+ REG_DX_TIME_TAM_MAX + REG_RX_FWTO_TAM_MAX + REG_SYS_CTRL_TAM_MAX + REG_SYS_MASK_TAM_MAX + REG_SYS_STATUS_TAM_MAX + REG_RX_FINFO_TAM_MAX		\
	+ REG_RX_FQUAL_TAM_MAX	+ REG_RX_TTCKI_TAM_MAX + REG_RX_TTCKO_TAM_MAX + REG_RX_TIME_TAM_MAX + REG_TX_TIME_TAM_MAX + REG_TX_ANTD_TAM_MAX			\
	+ REG_SYS_STATE_TAM_MAX + REG_ACK_RESP_T_TAM_MAX + REG_RX_SNIFF_TAM_MAX + REG_TX_POWER_TAM_MAX + REG_CHAN_CTRL_TAM_MAX + REG_USR_SFD_TAM_MAX	\
	+ REG_AGC_CTRL_TAM_MAX + REG_EXT_SYNC_TAM_MAX + REG_GPIO_CTRL_TAM_MAX + REG_DRX_CONF_TAM_MAX + REG_RF_CONF_TAM_MAX + REG_TX_CAL_TAM_MAX			\
	+ REG_FS_CTRL_TAM_MAX + REG_AON_TAM_MAX + REG_OTP_IF_TAM_MAX + REG_LDE_CTRL_TAM_MAX + REG_DIG_DIAG_TAM_MAX + REG_PMSC_TAM_MAX )					\

// Subregistradores
#define SUB_REG_NENHUM				0xFF	// Valor para não endereçar subregistrador
#define SUB_REG_AGC_CTRL1_END		0x02	// AGC Control #1
#define SUB_REG_AGC_TUNE1_END		0x04	// AGC Tuning register 1
#define SUB_REG_AGC_TUNE2_END 		0x0C	// AGC Tuning register 2
#define SUB_REG_AGC_TUNE3_END		0x12	// AGC Tuning register 3
#define SUB_REG_AGC_STAT1_END		0x1E	// AGC Status
#define SUB_REG_EC_CTRL_END			0x00	// External clock synchronisation counter configuration
#define SUB_REG_EC_RXTC_END			0x04	// External clock counter captured on RMARKER
#define SUB_REG_EC_GOLP_END			0x08	// External clock offset to first path 1 GHz counter
#define SUB_REG_GPIO_MODE_END		0x00	// GPIO Mode Control Register
#define SUB_REG_GPIO_DIR_END		0x08	// GPIO Direction Control Register
#define SUB_REG_GPIO_DOUT_END		0x0C	// GPIO Data Output register
#define SUB_REG_GPIO_IRQE_END		0x10	// GPIO Interrupt Enable
#define SUB_REG_GPIO_ISEN_END		0x14	// GPIO Interrupt Sense Selection
#define SUB_REG_GPIO_IMODE_END		0x18	// GPIO Interrupt Mode (Level / Edge)
#define SUB_REG_GPIO_IBES_END		0x1C	// GPIO Interrupt “Both Edge” Select
#define SUB_REG_GPIO_ICLR_END		0x20	// GPIO Interrupt Latch Clear
#define SUB_REG_GPIO_IDBE_END		0x24	// GPIO Interrupt De-bounce Enable
#define SUB_REG_GPIO_RAW_END		0x28	// GPIO raw state
#define SUB_REG_DRX_TUNE0b_END		0x02	// Digital Tuning Register 0b
#define SUB_REG_DRX_TUNE1a_END		0x04	// Digital Tuning Register 1a
#define SUB_REG_DRX_TUNE1b_END		0x06	// Digital Tuning Register 1b
#define SUB_REG_DRX_TUNE2_END		0x08	// Digital Tuning Register 2
#define SUB_REG_DRX_SFDTOC_END		0x20	// SFD timeout
#define SUB_REG_DRX_PRETOC_END		0x24	// Preamble detection timeout
#define SUB_REG_DRX_TUNE4H_END		0x26	// Digital Tuning Register 4H
#define SUB_REG_DRX_CAR_INT_END		0x28	// Carrier Recovery Integrator Register
#define SUB_REG_RXPACC_NOSAT_END	0X2C	// Unsaturated accumulated preamble symbols
#define SUB_REG_RF_CONF_END			0x00	// RF Configuration Register
#define SUB_REG_RF_RXCTRLH_END		0x0B	// Analog RX Control Register
#define SUB_REG_RF_TXCTRL_END		0x0C	// Analog TX Control Register
#define SUB_REG_RF_STATUS_END		0x2C	// RF Status Register
#define SUB_REG_LDOTUNE_END			0x30	// LDO voltage tuning
#define SUB_REG_TC_SARC_END			0x00	// Transmitter Calibration – SAR control
#define SUB_REG_TC_SARL_END			0x03	// Transmitter Calibration – Latest SAR readings
#define SUB_REG_TC_SARW_END			0x06	// Transmitter Calibration – SAR readings at last Wake-Up
#define SUB_REG_TC_PG_CTRL_END		0x08	// Transmitter Calibration – Pulse Generator Control
#define SUB_REG_TC_PG_STATUS_END	0x09	// Transmitter Calibration – Pulse Generator Status
#define SUB_REG_TC_PGDELAY_END		0x0B	// Transmitter Calibration – Pulse Generator Delay
#define SUB_REG_TC_PGTEST_END		0x0C	// Transmitter Calibration – Pulse Generator Test
#define SUB_REG_FS_PLLCFG_END		0x07	// Frequency synthesiser – PLL configuration
#define SUB_REG_FS_PLLTUNE_END		0x0B	// Frequency synthesiser – PLL Tuning
#define SUB_REG_FS_XTALT_END		0x0E	// Frequency synthesiser – Crystal trim
#define SUB_REG_AON_WCFG_END		0x00	// AON Wakeup Configuration Register
#define SUB_REG_AON_CTRL_END		0x02	// AON Control Register
#define SUB_REG_AON_RDAT_END		0x03	// AON Direct Access Read Data Result
#define SUB_REG_AON_ADDR_END		0x04	// AON Direct Access Address
#define SUB_REG_AON_CFG0_END		0x06	// AON Configuration Register 0
#define SUB_REG_AON_CFG1_END		0x0A	// AON Configuration Register 1
#define SUB_REG_OTP_WDAT_END		0x00	// OTP Write Data
#define SUB_REG_OTP_ADDR_END		0x04	// OTP Address
#define SUB_REG_OTP_CTRL_END		0x06	// OTP Control
#define SUB_REG_OTP_STAT_END		0x08	// OTP Status
#define SUB_REG_OTP_RDAT_END		0x0A	// OTP Read Data
#define SUB_REG_OTP_SRDAT_END		0x0E	// OTP SR Read Data
#define SUB_REG_OTP_SF_END			0x12	// OTP Special Function
#define SUB_REG_LDE_THRESH_END		0x0000	// LDE Threshold report
#define SUB_REG_LDE_CFG1_END		0x0806	// LDE Configuration Register 1
#define SUB_REG_LDE_PPINDX_END		0x1000	// LDE Peak Path Index
#define SUB_REG_LDE_PPAMPL_END		0x1002	// LDE Peak Path Amplitude
#define SUB_REG_LDE_RXANTD_END		0x1804	// LDE Receive Antenna Delay configuration
#define SUB_REG_LDE_CFG2_END		0x1806	// LDE Configuration Register 2
#define SUB_REG_LDE_REPC_END		0x2804	// LDE Replica Coefficient configuration
#define SUB_REG_EVC_CTRL_END		0x00	// Event Counter Control
#define SUB_REG_EVC_PHE_END			0x04	// PHR Error Counter
#define SUB_REG_EVC_RSE_END			0x06	// RSD Error Counter
#define SUB_REG_EVC_FCG_END			0x08	// Frame Check Sequence Good Counter
#define SUB_REG_EVC_FCE_END			0x0A	// Frame Check Sequence Error Counter
#define SUB_REG_EVC_FFR_END			0x0C	// Frame Filter Rejection Counter
#define SUB_REG_EVC_OVR_END			0x0E	// RX Overrun Error Counter
#define SUB_REG_EVC_STO_END			0x10	// SFD Timeout Counter
#define SUB_REG_EVC_PTO_END			0x12	// Preamble Timeout Counter
#define SUB_REG_VC_FWTO_END			0x14	// RX Frame Wait Timeout Counter
#define SUB_REG_EVC_TXFS_END		0x16	// TX Frame Sent Counter
#define SUB_REG_EVC_HPW_END			0x18	// Half Period Warning Counter
#define SUB_REG_EVC_TPW_END			0x1A	// Transmitter Power-Up Warning Counter
#define SUB_REG_DIAG_TMC_END		0x24	// Test Mode Control Register
#define SUB_REG_PMSC_CTRL0_END		0x00	// PMSC Control Register 0
#define SUB_REG_PMSC_CTRL1_END		0x04	// PMSC Control Register 1
#define SUB_REG_PMSC_SNOZT_END		0x0C	// PMSC Snooze Time Register
#define SUB_REG_PMSC_TXFSEQ_END		0x26	// PMSC fine grain TX sequencing control
#define SUB_REG_PMSC_LEDC_END		0x28	// PMSC LED Control Register

#define SUB_REG_AGC_CTRL1_TAM_MAX		2
#define SUB_REG_AGC_TUNE1_TAM_MAX		2
#define SUB_REG_AGC_TUNE2_TAM_MAX		4
#define SUB_REG_AGC_TUNE3_TAM_MAX		2
#define SUB_REG_AGC_STAT1_TAM_MAX		3
#define SUB_REG_EC_CTRL_TAM_MAX			4
#define SUB_REG_EC_RXTC_TAM_MAX			4
#define SUB_REG_EC_GOLP_TAM_MAX			4
#define SUB_REG_GPIO_MODE_TAM_MAX		4
#define SUB_REG_GPIO_DIR_TAM_MAX		4
#define SUB_REG_GPIO_DOUT_TAM_MAX		4
#define SUB_REG_GPIO_IRQE_TAM_MAX		4
#define SUB_REG_GPIO_ISEN_TAM_MAX		4
#define SUB_REG_GPIO_IMODE_TAM_MAX		4
#define SUB_REG_GPIO_IBES_TAM_MAX		4
#define SUB_REG_GPIO_ICLR_TAM_MAX		4
#define SUB_REG_GPIO_IDBE_TAM_MAX		4
#define SUB_REG_GPIO_RAW_TAM_MAX		4
#define SUB_REG_DRX_TUNE0b_TAM_MAX		2
#define SUB_REG_DRX_TUNE1a_TAM_MAX		2
#define SUB_REG_DRX_TUNE1b_TAM_MAX		2
#define SUB_REG_DRX_TUNE2_TAM_MAX		4
#define SUB_REG_DRX_SFDTOC_TAM_MAX		2
#define SUB_REG_DRX_PRETOC_TAM_MAX		2
#define SUB_REG_DRX_TUNE4H_TAM_MAX		2
#define SUB_REG_DRX_CAR_INT_TAM_MAX		3
#define SUB_REG_RXPACC_NOSAT_TAM_MAX	2
#define SUB_REG_RF_CONF_TAM_MAX			4
#define SUB_REG_RF_RXCTRLH_TAM_MAX		1
#define SUB_REG_RF_TXCTRL_TAM_MAX		4
#define SUB_REG_RF_STATUS_TAM_MAX		1
#define SUB_REG_LDOTUNE_TAM_MAX			5
#define SUB_REG_TC_SARC_TAM_MAX			2
#define SUB_REG_TC_SARL_TAM_MAX			3
#define SUB_REG_TC_SARW_TAM_MAX			2
#define SUB_REG_TC_PG_CTRL_TAM_MAX		1
#define SUB_REG_TC_PG_STATUS_TAM_MAX	2
#define SUB_REG_TC_PGDELAY_TAM_MAX		1
#define SUB_REG_TC_PGTEST_TAM_MAX		1
#define SUB_REG_FS_PLLCFG_TAM_MAX		4
#define SUB_REG_FS_PLLTUNE_TAM_MAX		1
#define SUB_REG_FS_XTALT_TAM_MAX		1
#define SUB_REG_AON_WCFG_TAM_MAX		2
#define SUB_REG_AON_CTRL_TAM_MAX		1
#define SUB_REG_AON_RDAT_TAM_MAX		1
#define SUB_REG_AON_ADDR_TAM_MAX		1
#define SUB_REG_AON_CFG0_TAM_MAX		4
#define SUB_REG_AON_CFG1_TAM_MAX		2
#define SUB_REG_OTP_WDAT_TAM_MAX		4
#define SUB_REG_OTP_ADDR_TAM_MAX		2
#define SUB_REG_OTP_CTRL_TAM_MAX		2
#define SUB_REG_OTP_STAT_TAM_MAX		2
#define SUB_REG_OTP_RDAT_TAM_MAX		4
#define SUB_REG_OTP_SRDAT_TAM_MAX		4
#define SUB_REG_OTP_SF_TAM_MAX			1
#define SUB_REG_LDE_THRESH_TAM_MAX		2
#define SUB_REG_LDE_CFG1_TAM_MAX		1
#define SUB_REG_LDE_PPINDX_TAM_MAX		2
#define SUB_REG_LDE_PPAMPL_TAM_MAX		2
#define SUB_REG_LDE_RXANTD_TAM_MAX		2
#define SUB_REG_LDE_CFG2_TAM_MAX		2
#define SUB_REG_LDE_REPC_TAM_MAX		2
#define SUB_REG_EVC_CTRL_TAM_MAX		4
#define SUB_REG_EVC_PHE_TAM_MAX			2
#define SUB_REG_EVC_RSE_TAM_MAX			2
#define SUB_REG_EVC_FCG_TAM_MAX			2
#define SUB_REG_EVC_FCE_TAM_MAX			2
#define SUB_REG_EVC_FFR_TAM_MAX			2
#define SUB_REG_EVC_OVR_TAM_MAX			2
#define SUB_REG_EVC_STO_TAM_MAX			2
#define SUB_REG_EVC_PTO_TAM_MAX			2
#define SUB_REG_VC_FWTO_TAM_MAX			2
#define SUB_REG_EVC_TXFS_TAM_MAX		2
#define SUB_REG_EVC_HPW_TAM_MAX			2
#define SUB_REG_EVC_TPW_TAM_MAX			2
#define SUB_REG_DIAG_TMC_TAM_MAX		2
#define SUB_REG_PMSC_CTRL0_TAM_MAX		4
#define SUB_REG_PMSC_CTRL1_TAM_MAX		4
#define SUB_REG_PMSC_SNOZT_TAM_MAX		1
#define SUB_REG_PMSC_TXFSEQ_TAM_MAX		2
#define SUB_REG_PMSC_LEDC_TAM_MAX		4

// Registradores  OTP
#define REG_OTP_VMEAS_3V3_END			0x008
#define REG_OTP_TMEAS_23C_END			0x009

void dw1000_inicializa( void );
void dw1000_reset( uint16_t delay );
void dw1000_int_trata( void );
void dw1000_transmite_dados( uint8_t *dados, uint8_t qtd_bytes );
void dw1000_habilita_rx( void );
void dw1000_desabilita_rx( void );
uint64_t dw1000_leitura_timestamp( uint8_t );
float dw1000_potencia_rx( void );

#endif  // _UWB_H_

