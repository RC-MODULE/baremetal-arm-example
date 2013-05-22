#include "uemd_early.h"

#define PL341_MEMC_STATUS             0x000
#define PL341_MEMC_CMD                0x004
#define PL341_DIRECT_CMD              0x008
#define PL341_MEMORY_CFG              0x00C 
#define PL341_REFRESH_PRD             0x010
#define PL341_CAS_LATENCY             0x014
#define PL341_WRITE_LATENCY           0x018
#define PL341_T_MRD                   0x01C
#define PL341_T_RAS                   0x020
#define PL341_T_RC                    0x024
#define PL341_T_RCD                   0x028
#define PL341_T_RFC                   0x02C
#define PL341_T_RP                    0x030
#define PL341_T_RRD                   0x034
#define PL341_T_WR                    0x038
#define PL341_T_WTR                   0x03C
#define PL341_T_XP                    0x040
#define PL341_T_XSR                   0x044
#define PL341_T_ESR                   0x048
#define PL341_MEMORY_CFG2             0x04C
#define PL341_MEMORY_CFG3             0x050
#define PL341_T_FAW                   0x054
#define PL341_TRD_EN                  0x05C


#define PL341_CHIP_0_CFG              0X200
#define PL341_CHIP_1_CFG              0X204
#define PL341_CHIP_2_CFG              0X208
#define PL341_CHIP_3_CFG              0X20C


#define MEM(addr)  (*((volatile unsigned long int*)(addr)))

#define DMC0_APB_BASE      0x20032000
#define DMC1_APB_BASE      0x20031000


static void ddr_init(unsigned DMCX_APB_Based)
{
	int i;

	//set DMC in PAUSE mode
	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000003;
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x2) {}

	//set DMC in CONFIGURE mode
	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000004;
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x0) {}

	MEM(0x180200)=MEM(DMCX_APB_Based+PL341_MEMC_STATUS);
	//---------------------------------------------------------------	
	//------------- set timing parameters of DDR --------------------
	//---------------------------------------------------------------
	// tCK=1/324 MHz = 0.003086 * 10^-6 = 3.086 ns= 3 ns
	// tACLK = 1/162= 0.00617 * 10^-6 = 6.17 ns = 6 ns

	//cas_latency 0x00000006 Set CAS latency to 3
	MEM(DMCX_APB_Based+PL341_CAS_LATENCY)=0x00000008; //CAS=4

	//t_mrd 0x00000002 Set tMRD to 2
	MEM(DMCX_APB_Based+PL341_T_MRD)=0x00000002;

	//t_ras 0x00000008 Set tRAS to 8
	MEM(DMCX_APB_Based+PL341_T_RAS)=0x0000000F; // tRAS = 45 ns / tCK = 15 = 0xF

	//t_rc 0x0000000B Set tRC to 11
	MEM(DMCX_APB_Based+PL341_T_RC)=0x00000014;  // tRC  = 60 ns / tCK = 20 = 0x14

	//t_rcd 0x00000103 Set tRCD to 3 and schedule_rcd to 1
	MEM(DMCX_APB_Based+PL341_T_RCD)=0x00000305; // tRCD = 15 ns / tCK = 5  = 0x5

	//t_rfc 0x00001315 Set tRFC to 21 and schedule_rfc to 19
	MEM(DMCX_APB_Based+PL341_T_RFC)=0x0000132B; // tRFC=127,5 ns/tACLK - 3 = 19 = 0x13; tRFC = 127,5 ns/tACLK= 42,5 = 0x2B;

	//t_rp 0x00000103 Set tRP to 3 and schedule_rp to 1
	MEM(DMCX_APB_Based+PL341_T_RP)=0x00000005;  // tRP  = 15 ns / tCK = 5 = 0x5; tRP  = 15 ns / tACLK -3 = 2,5 - 3 =0

	//t_rrd 0x00000002 Set tRRD to 2
	MEM(DMCX_APB_Based+PL341_T_RRD)=0x00000004; // tRRD = 10 ns / tCK = 3,33=4

	//t_wr 0x00000003 Set tWR to 3
	MEM(DMCX_APB_Based+PL341_T_WR)=0x00000005;  // tWR  = 15 ns / tCK = 5

	//t_wtr 0x00000002 Set tWTR to 2
	MEM(DMCX_APB_Based+PL341_T_WTR)=0x00000003; // tWTR = 7,5 ns/ tCK = 2,5=3

	//t_xp 0x00000002 Set tXP to 2
	MEM(DMCX_APB_Based+PL341_T_XP)=0x00000002;  // tXP = 2

	//t_xsr 0x000000C8 Set tXSR to 200
	MEM(DMCX_APB_Based+PL341_T_XSR)=0x000000c8; //

	//t_esr 0x00000003 Set tESR to 3
	MEM(DMCX_APB_Based+PL341_T_ESR)=0x000000c8; //

	//t_rddata_en 0x00000003 Set trddata_en to 3 if the DDR2 DMC is configured with a DFI pad interface
	MEM(DMCX_APB_Based+PL341_TRD_EN)=0x00000002;

	//memory_cfg 0x0001A411 Sets the following memory configuration:
	MEM(DMCX_APB_Based+PL341_MEMORY_CFG)=0x00010012; //1 chip 14 row 10 column BL=4

	//refresh_prd 0x00000618 Set an auto-refresh period of 3.9?s, that is, every 1560 (or 0x618) mclk periods when
	//mclk frequency is 400MHz
	MEM(DMCX_APB_Based+PL341_REFRESH_PRD)=0x00000a28; //

	//memory_cfg2 0x00000001 Sets the following memory configuration:
	// - 3 bits for the bank address
	// - 16-bit data bus
	MEM(DMCX_APB_Based+PL341_MEMORY_CFG2)=0x00000030;

	//chip_cfg0 0x000000FF Sets address for chip 0 to be 0x00XXXXXX, Row Bank Column (RBC) configuration
	//BRC organization, 
	MEM(DMCX_APB_Based+PL341_CHIP_0_CFG)=0x00000000;

	//chip_cfg1 0x000022FF Sets address for chip 1 to be 0x22XXXXXX, RBC configuration
	MEM(DMCX_APB_Based+PL341_CHIP_1_CFG)=0x000000FF;

	//chip_cfg2 0x000055FF Sets address for chip 2 to be 0x55XXXXXX, RBC configuration
	MEM(DMCX_APB_Based+PL341_CHIP_2_CFG)=0x000000FF;

	//chip_cfg3 0x00007FFF Sets address for chip 3 to be 0x7FXXXXXX, RBC configuration
	MEM(DMCX_APB_Based+PL341_CHIP_3_CFG)=0x000000FF;

	//Wait 200?s to enable the memory devices to stabilize
	for(i=0; i<30000;i++){
	}

	//Sequence of writes to initialize the memory devices, see Table 2-3 on page 2-35
	//direct_cmd 0x000C0000 NOP command to chip 0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x000C0000;

	//Wait 400ns
	for(i=0; i<100;i++){
	}
	//direct_cmd 0x00000000 PRECHARGEALL command to chip 0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00000000;

	//direct_cmd 0x000A0000 MODEREG command to extended mode register 2, with low address bits = 0x0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x000A0000;

	//direct_cmd 0x000B0000 MODEREG command to extended mode register 3, with low address bits = 0x0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x000B0000;

	//direct_cmd 0x00090000 MODEREG command to extended mode register 1, with low address bits = 0x0, to enable the
	//DLL.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00090000;

	//direct_cmd MODEREG command to mode register. This resets the DLL, WR=5,
	//burst length = 4, sequential bursts, CAS latency = 4.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00080942;

	//direct_cmd 0x00000000 PRECHARGEALL command to chip 0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00000000;

	//direct_cmd 0x00040000 AUTO REFRESH command to chip 0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00040000;

	//direct_cmd 0x00040000 AUTO REFRESH command to chip 0.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00040000;

	//direct_cmd  MODEREG command to mode register, This resets the DLL, WR=5,
	//burst length = 4, sequential bursts, CAS latency = 4.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00080842;

	//Wait for 200 mclk periods
	for(i=0; i<80;i++){
	}

	//direct_cmd 0x00090380 MODEREG command to extended mode register 1, with low address bits = 0x380, to set OCD
	//calibration default.
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00090380;

	//direct_cmd 0x00090000 MODEREG command to extended mode register 1, with low address bits = 0x0, to exit OCD.
	//while (1) {
	//MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00091555;
	//MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x000a2aaa;
	//}
	MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=0x00090040;

	//memc_cmd 0x00000000 Change DDR2 DMC to the Ready state
	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000000;

	//memc_status - Poll the register until the memc_status field returns b01, signifying that the controller is
	//ready to accept AXI accesses to the memory devices
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x1) {}
}


void uemd_em_init(void)
{
	MEM(0x2003c050)=0x9; //DDR PHY ODT ON
	ddr_init(DMC0_APB_BASE);

	MEM(0x2003c064)=0x9; //DDR PHY ODT ON
	ddr_init(DMC1_APB_BASE);
}
