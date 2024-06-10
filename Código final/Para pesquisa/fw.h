
#ifndef MAIN_INCLUDE_FW_H_
#define MAIN_INCLUDE_FW_H_

#include "../dw1000/dw1000.h"
#include "../gpio/gpio.h"
#include "../spi/spi.h"
#include "../timer/timer.h"

// Entradas
#define SPI_PIN_MISO	19
#define DW1000_PIN_EON	33
#define DW1000_PIN_IRQ	34

// Saídas
#define SPI_PIN_CS		4
#define SPI_PIN_CLK		18
#define SPI_PIN_MOSI	23
#define DW1000_PIN_RST	27
#define DW1000_PIN_WKP	32

// Geral
#define GPIO_OUTPUT_PIN_SEL		( ( 1ULL << SPI_PIN_CS ) | ( 1ULL << SPI_PIN_CLK ) | ( 1ULL << SPI_PIN_MOSI ) | ( 1ULL << DW1000_PIN_RST ) | ( 1ULL << DW1000_PIN_WKP ) )
#define GPIO_INPUT_PIN_SEL		( ( 1ULL << SPI_PIN_MISO ) | ( 1ULL << DW1000_PIN_EON ) )
#define GPIO_INPUT_INT_PIN_SEL	( 1ULL << DW1000_PIN_IRQ )

#define ESP_INTR_FLAG_DEFAULT	0

/**** DW1000 ****/
// Geral
#define DW1000_INT_TX	0
#define DW1000_INT_RX	1
#define DW1000_TXSTAMP	0
#define DW1000_RXSTAMP	1

// Protocolo
#define DW1000_SPI_HEADER_TAM_MAX	3
#define DW1000_SPI_HEADER_WR_VAL	0x80
#define DW1000_SPI_HEADER_RD_VAL	0x00
#define DW1000_SPI_HEADER_SID_VAL	0x40
#define DW1000_SPI_HEADER_EAD_VAL	0x80
#define DW1000_SUB_REG_RANGE_VAL	128

// Configurações
#define DW1000_PAN_ID_VAL			0xFFFF
#define DW1000_SHORT_ADDR_VAL		0xFFFF

#define DW1000_ANTENNA_CAL			0x4000		// 16384
#define DW1000_CHANNEL_1			1
#define DW1000_CHANNEL_2			2
#define DW1000_CHANNEL_3			3
#define DW1000_CHANNEL_4			4
#define DW1000_CHANNEL_5			5
#define DW1000_CHANNEL_6			6
#define DW1000_CHANNEL_7			7
#define DW1000_PREAM_CODE_16MHZ_1	1
#define DW1000_PREAM_CODE_16MHZ_2  	2
#define DW1000_PREAM_CODE_16MHZ_3  	3
#define DW1000_PREAM_CODE_16MHZ_4  	4
#define DW1000_PREAM_CODE_16MHZ_5  	5
#define DW1000_PREAM_CODE_16MHZ_6  	6
#define DW1000_PREAM_CODE_16MHZ_7  	7
#define DW1000_PREAM_CODE_16MHZ_8  	8
#define DW1000_PREAM_CODE_64MHZ_9  	9
#define DW1000_PREAM_CODE_64MHZ_10 	10
#define DW1000_PREAM_CODE_64MHZ_11 	11
#define DW1000_PREAM_CODE_64MHZ_12 	12
#define DW1000_PREAM_CODE_64MHZ_17 	17
#define DW1000_PREAM_CODE_64MHZ_18 	18
#define DW1000_PREAM_CODE_64MHZ_19 	19
#define DW1000_PREAM_CODE_64MHZ_20 	20
#define DW1000_TRX_RATE_110KBPS		0x00
#define DW1000_TRX_RATE_6800KBPS	0x02
#define DW1000_TX_PULSE_FREQ_16MHZ	0x01
#define DW1000_TX_PULSE_FREQ_64MHZ	0x02
#define DW1000_TX_PREAM_LEN_128		0x05
#define DW1000_TX_PREAM_LEN_1024	0x02
#define DW1000_TX_PREAM_LEN_2048	0x02

typedef struct {
	union {
		uint8_t		u8[ 8 ];
		uint64_t	u64;
	} tround1;

	union {
		uint8_t		u8[ 8 ];
		uint64_t	u64;
	} tround2;

	union {
		uint8_t		u8[ 8 ];
		uint64_t	u64;
	} treply1;

	union {
		uint8_t		u8[ 8 ];
		uint64_t	u64;
	} treply2;

	float	tprop;
	float 	distancia;
} Calculo_t;

typedef struct {
	struct {
		struct {	// registro
			struct {
				union {
					struct {
						uint32_t	rev		:4;
						uint32_t	ver		:4;
						uint32_t	model	:8;
						uint32_t	ridtag	:16;
					};
					uint8_t byte[ REG_DEV_ID_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} dev_id;

			struct {
				union {
					struct {
						  uint64_t extension_id	:40;
						  uint32_t oui 			:24;
						} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_EUI_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} eui;

			struct {
				union {
					struct {
						  uint16_t short_addr;
						  uint16_t pan_id;
						};

					uint8_t byte[ REG_PANADR_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} panadr;

			struct {
				union {
					struct {
						uint32_t	ffen		:1;
						uint32_t	ffbc		:1;
						uint32_t	ffab		:1;
						uint32_t	ffad		:1;
						uint32_t	ffaa		:1;
						uint32_t	ffam		:1;
						uint32_t	ffar		:1;
						uint32_t	ffa4		:1;
						uint32_t	ffa5		:1;
						uint32_t	hirq_pol	:1;
						uint32_t	spi_edge	:1;
						uint32_t	dis_fce		:1;
						uint32_t	dis_drxb	:1;
						uint32_t	dis_phe		:1;
						uint32_t	dis_rsde	:1;
						uint32_t	fcs_init2f	:1;
						uint32_t	phr_mode	:2;
						uint32_t	dis_stxp	:1;
						uint32_t				:3;
						uint32_t	rxm110k		:1;
						uint32_t				:5;
						uint32_t	rxwtoe		:1;
						uint32_t	rxautr		:1;
						uint32_t	autoack		:1;
						uint32_t	aackpend	:1;
					};

					uint8_t byte[ REG_SYS_CFG_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_cfg;

			struct {
				union {
					struct {
						  uint64_t counter	:40;
						} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_SYS_TIME_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_time;

			struct {
				union {
					struct {
						uint32_t	tflen		:7;
						uint32_t	tfle		:3;
						uint32_t	r			:3;
						uint32_t	txbr		:2;
						uint32_t	tr			:1;
						uint32_t	txprf		:2;
						uint32_t	txpsr		:2;
						uint32_t	pe			:2;
						uint32_t	txboffs		:10;
						uint8_t		ifsdelay;
					};

					uint8_t byte[ REG_TX_FCTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} tx_fctrl;

			//tx_buffer -> TBD

			struct {
				union {
					struct {
						  uint64_t delay	:40;
						} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_DX_TIME_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} dx_time;

			struct {
				union {
					struct {
						  uint32_t rxfwto	:16;
						  uint32_t			:16;
						};

					uint8_t byte[ REG_RX_FWTO_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_fwto;

			struct {
				union {
					struct {
						uint32_t	sfcst		:1;
						uint32_t	txstrt		:1;
						uint32_t	txdlys		:1;
						uint32_t	cansfcs		:1;
						uint32_t				:2;
						uint32_t	trxoff		:1;
						uint32_t	wait4resp	:1;
						uint32_t	rxenab		:1;
						uint32_t	rxdlye		:1;
						uint32_t				:14;
						uint32_t	hrbpt		:1;
						uint32_t				:7;
					};

					uint8_t byte[ REG_SYS_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_ctrl;

			struct {
				union {
					struct {
						uint32_t				:1;
						uint32_t	mcplock		:1;
						uint32_t	mesyncr		:1;
						uint32_t	maat		:1;
						uint32_t	mtxfrb		:1;
						uint32_t	mtxprs		:1;
						uint32_t	mtxphs		:1;
						uint32_t	mtxfrs		:1;
						uint32_t	mrxprd		:1;
						uint32_t	mrxsfdd		:1;
						uint32_t	mldedon		:1;
						uint32_t	mrxphd		:1;
						uint32_t	mrxphe		:1;
						uint32_t	mrxdfr		:1;
						uint32_t	mrxfcg		:1;
						uint32_t	mrxfce		:1;
						uint32_t	mrxrfsl		:1;
						uint32_t	mrxrfto		:1;
						uint32_t	mldeerr		:1;
						uint32_t				:1;
						uint32_t	mrxovrr		:1;
						uint32_t	mrxpto		:1;
						uint32_t	mgpioirq	:1;
						uint32_t	mslp2init	:1;
						uint32_t	mrfpllll	:1;
						uint32_t	mcpllll		:1;
						uint32_t	mrxsfdto	:1;
						uint32_t	mhpdwarn	:1;
						uint32_t	mtxberr		:1;
						uint32_t	maffrej		:1;
						uint32_t				:2;
					};

					uint8_t byte[ REG_SYS_MASK_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_mask;

			struct {
				union {
					struct {
						uint64_t	irqs		:1;
						uint64_t	cplock		:1;
						uint64_t	esyncr		:1;
						uint64_t	aat			:1;
						uint64_t	txfrb		:1;
						uint64_t	txprs		:1;
						uint64_t	txphs		:1;
						uint64_t	txfrs		:1;
						uint64_t	rxprd		:1;
						uint64_t	rxsfdd		:1;
						uint64_t	ldedone		:1;
						uint64_t	rxphd		:1;
						uint64_t	rxphe		:1;
						uint64_t	rxdfr		:1;
						uint64_t	rxfcg		:1;
						uint64_t	rxfce		:1;
						uint64_t	rxrfsl		:1;
						uint64_t	rxrfto		:1;
						uint64_t	ldeerr		:1;
						uint64_t				:1;
						uint64_t	rxovrr		:1;
						uint64_t	rxpto		:1;
						uint64_t	gpioirq		:1;
						uint64_t	slp2init	:1;
						uint64_t	rfpll_ll	:1;
						uint64_t	clkpll_ll	:1;
						uint64_t	rxsfdto		:1;
						uint64_t	hpdwarn		:1;
						uint64_t	txberr		:1;
						uint64_t	affrej		:1;
						uint64_t	hsrbp		:1;
						uint64_t	icrbp		:1;
						uint64_t	rxrscs		:1;
						uint64_t	rxprej		:1;
						uint64_t	txpute		:1;
						uint64_t				:5;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_SYS_STATUS_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_status;

			struct {
				union {
					struct {
						uint32_t	rxflen		:7;
						uint32_t	rxfle		:3;
						uint32_t				:1;
						uint32_t	rxnspl		:2;
						uint32_t	rxbr		:2;
						uint32_t	rng			:1;
						uint32_t	rxprfr		:2;
						uint32_t	rxpsr		:2;
						uint32_t	rxpacc		:12;
					};

					uint8_t byte[ REG_RX_FINFO_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_finfo;

			//rx_buffer -> TBD

			struct {
				union {
					struct {
						uint16_t	std_noise;
						uint16_t	fp_ampl2;
						uint16_t	pp_ampl3;
						uint16_t	cir_pwr;
					};

					uint8_t byte[ REG_RX_FQUAL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_fqual;

			struct {
				union {
					struct {
						uint32_t	rxttcki;
					};

					uint8_t byte[ REG_RX_TTCKI_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_ttcki;

			struct {
				union {
					struct {
						uint64_t	rxtofs		:19;
						uint64_t				:5;
						uint64_t	rsmpdel		:8;
						uint64_t	rcphase		:7;
						uint64_t				:1;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_RX_TTCKO_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_ttcko;

			struct {
				union {
					struct {
						uint32_t	rx_stamp_low;
						uint8_t		rx_stamp_high;
						uint16_t	fp_index;
						uint8_t		fp_ampl1_low;
						uint8_t		fp_ampl1_high;
						uint32_t	rx_rawst_low	:24;
						uint16_t	rx_rawst_high;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_RX_TIME_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_time;

			struct {
				union {
					struct {
						uint32_t	tx_stamp_low;
						uint8_t		tx_stamp_high;
						uint32_t	tx_rawst_low	:24;
						uint16_t	tx_rawst_high;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_TX_TIME_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} tx_time;

			struct {
				union {
					struct {
						  uint16_t delay;
						};

					uint8_t byte[ REG_TX_ANTD_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} tx_antd;

			struct {
				union {
					struct {
						  uint32_t	tx_state		:4;
						  uint32_t					:4;
						  uint32_t	rx_state		:5;
						  uint32_t					:3;
						  uint32_t	pmsc_state		:4;
						  uint32_t					:4;
						  uint32_t					:8;
						};

					uint8_t byte[ REG_SYS_STATE_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} sys_state;

			struct {
				union {
					struct {
						  uint32_t	w4r_tim			:20;
						  uint32_t					:4;
						  uint32_t	ack_tim			:8;
						};

					uint8_t byte[ REG_ACK_RESP_T_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} ack_resp_t;

			struct {
				union {
					struct {
						  uint32_t	sniff_ont		:4;
						  uint32_t					:4;
						  uint32_t	sniff_offt		:8;
						  uint32_t					:16;
						};

					uint8_t byte[ REG_RX_SNIFF_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rx_sniff;

			struct {
				union {
					struct {
						  uint32_t	boostnorm_na		:8;
						  uint32_t	boostp500_txpowphr	:8;
						  uint32_t	boostp250_txpowsd	:8;
						  uint32_t	boostp125_na		:8;
						};

					uint8_t byte[ REG_TX_POWER_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} tx_power;

			struct {
				union {
					struct {
						  uint32_t	tx_chan			:4;
						  uint32_t	rx_chan			:4;
						  uint32_t					:9;
						  uint32_t	dwsfd			:1;
						  uint32_t	rxprf			:2;
						  uint32_t	tnssfd			:1;
						  uint32_t	rnssfd			:1;
						  uint32_t	tx_pcode		:5;
						  uint32_t	rx_pcode		:5;
						};

					uint8_t byte[ REG_CHAN_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} chan_ctrl;

			struct {
				union {
					struct {
						  uint8_t	sfd_length		:8;
						  uint8_t	tx_ssfd_magl	:8;
						  uint8_t	tx_ssfd_magh	:8;
						  uint8_t	tx_ssfd_sgnl	:8;
						  uint8_t	tx_ssfd_sgnh	:8;
						  uint8_t	rx_ssfd_magl	:8;
						  uint8_t	rx_ssfd_magh	:8;
						  uint8_t	rx_ssfd_sgnl	:8;
						  uint8_t	rx_ssfd_sgnh	:8;
						  uint8_t	tx_lsfd_mag0	:8;
						  uint8_t	tx_lsfd_mag1	:8;
						  uint8_t	tx_lsfd_mag2	:8;
						  uint8_t	tx_lsfd_mag3	:8;
						  uint8_t	tx_lsfd_mag4	:8;
						  uint8_t	tx_lsfd_mag5	:8;
						  uint8_t	tx_lsfd_mag6	:8;
						  uint8_t	tx_lsfd_mag7	:8;
						  uint8_t	tx_lsfd_sgn0	:8;
						  uint8_t	tx_lsfd_sgn1	:8;
						  uint8_t	tx_lsfd_sgn2	:8;
						  uint8_t	tx_lsfd_sgn3	:8;
						  uint8_t	tx_lsfd_sgn4	:8;
						  uint8_t	tx_lsfd_sgn5	:8;
						  uint8_t	tx_lsfd_sgn6	:8;
						  uint8_t	tx_lsfd_sgn7	:8;
						  uint8_t	rx_lsfd_mag0	:8;
						  uint8_t	rx_lsfd_mag1	:8;
						  uint8_t	rx_lsfd_mag2	:8;
						  uint8_t	rx_lsfd_mag3	:8;
						  uint8_t	rx_lsfd_mag4	:8;
						  uint8_t	rx_lsfd_mag5	:8;
						  uint8_t	rx_lsfd_mag6	:8;
						  uint8_t	rx_lsfd_mag7	:8;
						  uint8_t	rx_lsfd_sgn0	:8;
						  uint8_t	rx_lsfd_sgn1	:8;
						  uint8_t	rx_lsfd_sgn2	:8;
						  uint8_t	rx_lsfd_sgn3	:8;
						  uint8_t	rx_lsfd_sgn4	:8;
						  uint8_t	rx_lsfd_sgn5	:8;
						  uint8_t	rx_lsfd_sgn6	:8;
						  uint8_t	rx_lsfd_sgn7	:8;
						};

					uint8_t byte[ REG_USR_SFD_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} usr_sfd;

			struct {
				union {
					struct {
						  uint16_t	agc_res1;
						  uint16_t	dis_am			:1;
						  uint16_t					:15;
						  uint16_t	agc_tune1;
						  uint32_t	agc_res2_low;
						  uint16_t	agc_res2_high;
						  uint32_t	agc_tune2;
						  uint16_t	agc_res3;
						  uint16_t	agc_tune3;
						  uint64_t	agc_res4_low;
						  uint16_t	agc_res4_high;
						  uint32_t					:6;
						  uint32_t	edg1			:5;
						  uint32_t  edv2			:9;
						  uint32_t					:4;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_AGC_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} agc_ctrl;

			struct {
				union {
					struct {
						  uint32_t	ostsm			:1;
						  uint32_t	osrsm			:1;
						  uint32_t	plldt			:1;
						  uint32_t	wait			:8;
						  uint32_t	ostrm			:1;
						  uint32_t					:20;
						  uint32_t	rx_ts_est;
						  uint32_t	offset_ext		:6;
						  uint32_t					:26;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_EXT_SYNC_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} ext_sync;

			//acc_mem -> TBD

			struct {
				union {
					struct {
						  uint32_t					:6;
						  uint32_t	msgp0			:2;
						  uint32_t	msgp1			:2;
						  uint32_t	msgp2			:2;
						  uint32_t	msgp3			:2;
						  uint32_t	msgp4			:2;
						  uint32_t	msgp5			:2;
						  uint32_t	msgp6			:2;
						  uint32_t	msgp7			:2;
						  uint32_t	msgp8			:2;
						  uint32_t					:8;
						  uint32_t					:32;
						  uint16_t	gdp0			:1;
						  uint16_t	gdp1			:1;
						  uint16_t	gdp2			:1;
						  uint16_t	gdp3			:1;
						  uint16_t	gdm0			:1;
						  uint16_t	gdm1			:1;
						  uint16_t	gdm2			:1;
						  uint16_t	gdm3			:1;
						  uint16_t	gdp4			:1;
						  uint16_t	gdp5			:1;
						  uint16_t	gdp6			:1;
						  uint16_t	gdp7			:1;
						  uint16_t	gdm4			:1;
						  uint16_t	gdm5			:1;
						  uint16_t	gdm6			:1;
						  uint16_t	gdm7			:1;
						  uint16_t	gdp8			:1;
						  uint16_t					:3;
						  uint16_t	gdm8			:1;
						  uint16_t					:11;
						  uint16_t	gop0			:1;
						  uint16_t	gop1			:1;
						  uint16_t	gop2			:1;
						  uint16_t	gop3			:1;
						  uint16_t	gom0			:1;
						  uint16_t	gom1			:1;
						  uint16_t	gom2			:1;
						  uint16_t	gom3			:1;
						  uint16_t	gop4			:1;
						  uint16_t	gop5			:1;
						  uint16_t	gop6			:1;
						  uint16_t	gop7			:1;
						  uint16_t	gom4			:1;
						  uint16_t	gom5			:1;
						  uint16_t	gom6			:1;
						  uint16_t	gom7			:1;
						  uint16_t	gop8			:1;
						  uint16_t					:3;
						  uint16_t	gom8			:1;
						  uint16_t					:11;
						  uint32_t	girqe0			:1;
						  uint32_t	girqe1			:1;
						  uint32_t	girqe2			:1;
						  uint32_t	girqe3			:1;
						  uint32_t	girqe4			:1;
						  uint32_t	girqe5			:1;
						  uint32_t	girqe6			:1;
						  uint32_t	girqe7			:1;
						  uint32_t	girqe8			:1;
						  uint32_t					:23;
						  uint32_t	gisen0			:1;
						  uint32_t	gisen1			:1;
						  uint32_t	gisen2			:1;
						  uint32_t	gisen3			:1;
						  uint32_t	gisen4			:1;
						  uint32_t	gisen5			:1;
						  uint32_t	gisen6			:1;
						  uint32_t	gisen7			:1;
						  uint32_t	gisen8			:1;
						  uint32_t					:23;
						  uint32_t	gimod0			:1;
						  uint32_t	gimod1			:1;
						  uint32_t	gimod2			:1;
						  uint32_t	gimod3			:1;
						  uint32_t	gimod4			:1;
						  uint32_t	gimod5			:1;
						  uint32_t	gimod6			:1;
						  uint32_t	gimod7			:1;
						  uint32_t	gimod8			:1;
						  uint32_t					:23;
						  uint32_t	gibes0			:1;
						  uint32_t	gibes1			:1;
						  uint32_t	gibes2			:1;
						  uint32_t	gibes3			:1;
						  uint32_t	gibes4			:1;
						  uint32_t	gibes5			:1;
						  uint32_t	gibes6			:1;
						  uint32_t	gibes7			:1;
						  uint32_t	gibes8			:1;
						  uint32_t					:23;
						  uint32_t	giclr0			:1;
						  uint32_t	giclr1			:1;
						  uint32_t	giclr2			:1;
						  uint32_t	giclr3			:1;
						  uint32_t	giclr4			:1;
						  uint32_t	giclr5			:1;
						  uint32_t	giclr6			:1;
						  uint32_t	giclr7			:1;
						  uint32_t	giclr8			:1;
						  uint32_t					:23;
						  uint32_t	gidbe0			:1;
						  uint32_t	gidbe1			:1;
						  uint32_t	gidbe2			:1;
						  uint32_t	gidbe3			:1;
						  uint32_t	gidbe4			:1;
						  uint32_t	gidbe5			:1;
						  uint32_t	gidbe6			:1;
						  uint32_t	gidbe7			:1;
						  uint32_t	gidbe8			:1;
						  uint32_t					:23;
						  uint32_t	grawp0			:1;
						  uint32_t	grawp1			:1;
						  uint32_t	grawp2			:1;
						  uint32_t	grawp3			:1;
						  uint32_t	grawp4			:1;
						  uint32_t	grawp5			:1;
						  uint32_t	grawp6			:1;
						  uint32_t	grawp7			:1;
						  uint32_t	grawp8			:1;
						  uint32_t					:23;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_GPIO_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} gpio_ctrl;

			struct {
				union {
					struct {
						  uint16_t	drx_res1;
						  uint16_t	drx_tune0b;
						  uint16_t	drx_tune1a;
						  uint16_t	drx_tune1b;
						  uint32_t	drx_tune2;
						  uint64_t	drx_res2_low;
						  uint64_t	drx_res2_medium;
						  uint32_t	drx_res2_high;
						  uint16_t	drx_sfdtoc;
						  uint16_t	drx_res3;
						  uint16_t	drx_pretoc;
						  uint16_t	drx_tune4h;
						  uint32_t	drx_car_int		:24;
						  uint16_t	rxpacc_nosat;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_DRX_CONF_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} drx_conf;

			struct {
				union {
					struct {
						  uint32_t					:8;
						  uint32_t	txfen			:5;
						  uint32_t	pllfen			:3;
						  uint32_t	ldofen			:5;
						  uint32_t	txrxs			:2;
						  uint32_t					:9;
						  uint32_t	rf_res1_0		:32;
						  uint16_t	rf_res1_1		:16;
						  uint8_t	rf_res1_2		:8;
						  uint8_t	rf_rxctrlh;
						  uint32_t	rf_txctrl;
						  uint32_t					:5;
						  uint32_t	txmtune			:4;
						  uint32_t	txmq			:3;
						  uint32_t					:20;
						  uint64_t	rf_res2_0		:64;
						  uint64_t	rf_res2_1		:64;
						  uint32_t	cplllock		:1;
						  uint32_t	cplllow			:1;
						  uint32_t	cpllhigh		:1;
						  uint32_t	rfplllock		:1;
						  uint32_t					:28;
						  uint32_t	ldo_tune_low;
						  uint8_t	ldo_tune_high;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_RF_CONF_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} rf_conf;

			struct {
				union {
					struct {
						  uint16_t	sar_ctrl	:1;
						  uint16_t				:15;
						  uint8_t	sar_lvbat;
						  uint8_t	sar_ltemp;
						  uint8_t				:8;
						  uint8_t	sar_wvbat;
						  uint8_t	sar_wtemp;
						  uint8_t	pg_start	:1;
						  uint8_t	reserved	:1;
						  uint8_t	t_meas		:4;
						  uint8_t				:2;
						  uint16_t	delay_cnt	:12;
						  uint16_t				:4;
						  uint8_t	tc_pgdelay;
						  uint8_t	tc_pgtest;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_TX_CAL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} tx_cal;

			struct {
				union {
					struct {
						  uint32_t	fs_res1_0	:32;
						  uint16_t	fs_res1_1	:16;
						  uint8_t	fs_res1_2	:8;
						  uint32_t	fs_pllcfg;
						  uint8_t	fs_plltune;
						  uint16_t	fs_res2;
						  uint8_t	xtalt;
						  uint32_t	fs_res3_0	:32;
						  uint16_t	fs_res3_1	:16;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_FS_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} fs_ctrl;

			struct {
				union {
					struct {
						uint16_t	onw_rad		:1;
						uint16_t	onw_rx		:1;
						uint16_t				:1;
						uint16_t	onw_leui	:1;
						uint16_t				:2;
						uint16_t	onw_ldc		:1;
						uint16_t	onw_l64		:1;
						uint16_t	pres_slee	:1;
						uint16_t				:2;
						uint16_t	onw_llde	:1;
						uint16_t	onw_lld		:1;
						uint16_t				:3;
						uint8_t		restore		:1;
						uint8_t		save		:1;
						uint8_t		upl_cfg		:1;
						uint8_t		dca_read	:1;
						uint8_t					:3;
						uint8_t		dca_enab	:1;
						uint8_t		aon_rdat;
						uint8_t		aon_addr;
						uint8_t		aon_res1;
						uint32_t	sleep_en	:1;
						uint32_t	wake_pin	:1;
						uint32_t	wake_spi	:1;
						uint32_t	wake_cnt	:1;
						uint32_t	lpdiv_en	:1;
						uint32_t	lpclkdiva	:11;
						uint32_t	sleep_tim	:16;
						uint16_t	sleep_cen	:1;
						uint16_t	smxx		:1;
						uint16_t	lposc_cal	:1;
						uint16_t				:13;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_AON_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} aon;

			struct {
				union {
					struct {
						uint32_t	otp_wdat;
						uint16_t	otp_addr	:11;
						uint16_t				:5;
						uint16_t	otprden		:1;
						uint16_t	otpread		:1;
						uint16_t				:1;
						uint16_t	otpmrwr		:1;
						uint16_t				:2;
						uint16_t	otpprog		:1;
						uint16_t	otpmr		:4;
						uint16_t				:4;
						uint16_t	ldeload		:1;
						uint16_t	otpprgd		:1;
						uint16_t	otpvpok		:1;
						uint16_t				:14;
						uint32_t	otp_rdat;
						uint32_t	otp_srdat;
						uint8_t		ops_kick	:1;
						uint8_t		ldo_kick	:1;
						uint8_t					:3;
						uint8_t		ops_sel		:2;
						uint8_t					:1;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_OTP_IF_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} otp_if;

			struct {
				union {
					struct {
						uint16_t	lde_thresh;
						uint8_t		ntm			:5;
						uint8_t		pmult		:3;
						uint16_t	lde_ppindx;
						uint16_t	lde_ppampl;
						uint16_t	lde_rxantd;
						uint16_t	lde_cfg2;
						uint16_t	lde_repc;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_LDE_CTRL_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} lde_ctrl;

			struct {
				union {
					struct {
						uint32_t	evc_en		:1;
						uint32_t	evc_clr		:1;
						uint32_t				:30;
						uint16_t	evc_phe		:12;
						uint16_t				:4;
						uint16_t	evc_rse		:12;
						uint16_t				:4;
						uint16_t	evc_fcg		:12;
						uint16_t				:4;
						uint16_t	evc_fce		:12;
						uint16_t				:4;
						uint16_t	evc_ffr		:12;
						uint16_t				:4;
						uint16_t	evc_ovr		:12;
						uint16_t				:4;
						uint16_t	evc_sto		:12;
						uint16_t				:4;
						uint16_t	evc_pto		:12;
						uint16_t				:4;
						uint16_t	evc_fwto	:12;
						uint16_t				:4;
						uint16_t	evc_txfs	:12;
						uint16_t				:4;
						uint16_t	evc_hpw		:12;
						uint16_t				:4;
						uint16_t	evc_tpw		:12;
						uint16_t				:4;
						uint64_t	evc_res1;
						uint16_t				:4;
						uint16_t	diag_tmc	:1;
						uint16_t				:11;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_DIG_DIAG_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} dig_diag;

			struct {
				union {
					struct {
						uint32_t	sysclks		:2;
						uint32_t	rxclks		:2;
						uint32_t	txclks		:2;
						uint32_t	face		:1;
						uint32_t				:3;
						uint32_t	adcce		:1;
						uint32_t				:4;
						uint32_t	amce		:1;
						uint32_t	gpce		:1;
						uint32_t	gprn		:1;
						uint32_t	gpdce		:1;
						uint32_t	gpdrn		:1;
						uint32_t				:3;
						uint32_t	khzclken	:1;
						uint32_t	pll2_seq_en	:1;
						uint32_t				:3;
						uint32_t	softreset	:4;
						uint32_t				:1;
						uint32_t	arx2int		:1;
						uint32_t				:1;
						uint32_t	pktseq		:8;
						uint32_t	atxslp		:1;
						uint32_t	arxslp		:1;
						uint32_t	snoze		:1;
						uint32_t	snozr		:1;
						uint32_t	pllsyn		:1;
						uint32_t				:1;
						uint32_t	lderune		:1;
						uint32_t				:8;
						uint32_t	khzclkdiv	:6;
						uint32_t	pmsc_res1;
						uint8_t		snoz_tim;
						uint64_t	pmsc_res2_0;
						uint64_t	pmsc_res2_1;
						uint32_t	pmsc_res2_2;
						uint16_t	pmsc_res2_3;
						uint16_t	txfineseq;
						uint32_t	blink_tim	:8;
						uint32_t	blink_en	:1;
						uint32_t				:7;
						uint32_t	blink_now	:4;
						uint32_t				:12;
					} __attribute__ ( ( packed ) );

					uint8_t byte[ REG_PMSC_TAM_MAX ];
				} parametro;

				uint8_t endereco;
				uint8_t qtd_bytes;
			} pmsc;

		} registro;

		uint8_t vmeas3V3;
		uint8_t tmeas23C;
		uint8_t tbuf[ 50 ];
		uint8_t rbuf[ 50 ];
	} dw1000;

	struct
	{
		spi_device_handle_t spi2_handler;
		spi_transaction_t spi2_transaction;
	} spi;

	struct
	{
		gpio_config_t io_conf;
	} gpio;

	struct
	{
		esp_timer_handle_t handler;
	} timer;

	union {
		struct {
				uint8_t		dw1000_irq	:1;
				uint8_t		dw1000_tot	:1;
				uint8_t					:6;
			};
		uint8_t	byte;
	} flags;

	struct
	{
		uint64_t	t1;
		uint64_t	t2;
		uint64_t	t3;
		float		distancia;
	} medidas;

	Calculo_t	calculo;

	uint8_t	sm_localizacao;
} Uwb_t;

extern Uwb_t	uwb;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
static const char *TAG = "UWB";

void fw_inicializa( void );

#endif /* MAIN_INCLUDE_FW_H_ */
