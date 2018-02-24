#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

//#define INT_WTIMER0A            110         // 32/64-Bit Timer 0A

//*****************************************************************************
//
// Timer registers (WTIMER0)
//
//*****************************************************************************
#define WTIMER0_CFG_R           (*((volatile uint32_t *)0x40036000))
#define WTIMER0_TAMR_R          (*((volatile uint32_t *)0x40036004))
#define WTIMER0_TBMR_R          (*((volatile uint32_t *)0x40036008))
#define WTIMER0_CTL_R           (*((volatile uint32_t *)0x4003600C))
#define WTIMER0_SYNC_R          (*((volatile uint32_t *)0x40036010))
#define WTIMER0_IMR_R           (*((volatile uint32_t *)0x40036018))
#define WTIMER0_RIS_R           (*((volatile uint32_t *)0x4003601C))
#define WTIMER0_MIS_R           (*((volatile uint32_t *)0x40036020))
#define WTIMER0_ICR_R           (*((volatile uint32_t *)0x40036024))
#define WTIMER0_TAILR_R         (*((volatile uint32_t *)0x40036028))
#define WTIMER0_TBILR_R         (*((volatile uint32_t *)0x4003602C))
#define WTIMER0_TAMATCHR_R      (*((volatile uint32_t *)0x40036030))
#define WTIMER0_TBMATCHR_R      (*((volatile uint32_t *)0x40036034))
#define WTIMER0_TAPR_R          (*((volatile uint32_t *)0x40036038))
#define WTIMER0_TBPR_R          (*((volatile uint32_t *)0x4003603C))
#define WTIMER0_TAPMR_R         (*((volatile uint32_t *)0x40036040))
#define WTIMER0_TBPMR_R         (*((volatile uint32_t *)0x40036044))
#define WTIMER0_TAR_R           (*((volatile uint32_t *)0x40036048))
#define WTIMER0_TBR_R           (*((volatile uint32_t *)0x4003604C))
#define WTIMER0_TAV_R           (*((volatile uint32_t *)0x40036050))
#define WTIMER0_TBV_R           (*((volatile uint32_t *)0x40036054))
#define WTIMER0_RTCPD_R         (*((volatile uint32_t *)0x40036058))
#define WTIMER0_TAPS_R          (*((volatile uint32_t *)0x4003605C))
#define WTIMER0_TBPS_R          (*((volatile uint32_t *)0x40036060))
#define WTIMER0_TAPV_R          (*((volatile uint32_t *)0x40036064))
#define WTIMER0_TBPV_R          (*((volatile uint32_t *)0x40036068))
#define WTIMER0_PP_R            (*((volatile uint32_t *)0x40036FC0))

#define TIMER_ICR_TBTOCINT      0x00000100  // GPTM Timer B Time-Out Interrupt
                                            // Clear
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM Timer A Time-Out Raw
                                            // Interrupt

#define SYSCTL_RCGCWTIMER_R     (*((volatile uint32_t *)0x400FE65C))

#define NVIC_PRI5_R             (*((volatile uint32_t *)0xE000E414))
#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))
#define NVIC_EN0_INT21          0x00200000  // Interrupt 21 enable

void WideTimer0A_Init(void(*task)(void), uint64_t period);

//*****************************************************************************
//
// Timer registers (TIMER1)
//
//*****************************************************************************
#define TIMER1_CFG_R            (*((volatile uint32_t *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile uint32_t *)0x40031004))
#define TIMER1_TBMR_R           (*((volatile uint32_t *)0x40031008))
#define TIMER1_CTL_R            (*((volatile uint32_t *)0x4003100C))
#define TIMER1_SYNC_R           (*((volatile uint32_t *)0x40031010))
#define TIMER1_IMR_R            (*((volatile uint32_t *)0x40031018))
#define TIMER1_RIS_R            (*((volatile uint32_t *)0x4003101C))
#define TIMER1_MIS_R            (*((volatile uint32_t *)0x40031020))
#define TIMER1_ICR_R            (*((volatile uint32_t *)0x40031024))
#define TIMER1_TAILR_R          (*((volatile uint32_t *)0x40031028))
#define TIMER1_TBILR_R          (*((volatile uint32_t *)0x4003102C))
#define TIMER1_TAMATCHR_R       (*((volatile uint32_t *)0x40031030))
#define TIMER1_TBMATCHR_R       (*((volatile uint32_t *)0x40031034))
#define TIMER1_TAPR_R           (*((volatile uint32_t *)0x40031038))
#define TIMER1_TBPR_R           (*((volatile uint32_t *)0x4003103C))
#define TIMER1_TAPMR_R          (*((volatile uint32_t *)0x40031040))
#define TIMER1_TBPMR_R          (*((volatile uint32_t *)0x40031044))
#define TIMER1_TAR_R            (*((volatile uint32_t *)0x40031048))
#define TIMER1_TBR_R            (*((volatile uint32_t *)0x4003104C))
#define TIMER1_TAV_R            (*((volatile uint32_t *)0x40031050))
#define TIMER1_TBV_R            (*((volatile uint32_t *)0x40031054))
#define TIMER1_RTCPD_R          (*((volatile uint32_t *)0x40031058))
#define TIMER1_TAPS_R           (*((volatile uint32_t *)0x4003105C))
#define TIMER1_TBPS_R           (*((volatile uint32_t *)0x40031060))
#define TIMER1_TAPV_R           (*((volatile uint32_t *)0x40031064))
#define TIMER1_TBPV_R           (*((volatile uint32_t *)0x40031068))
#define TIMER1_PP_R             (*((volatile uint32_t *)0x40031FC0))
	
#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))
	
//find the NVIC ENABLE and NVIC priority registers...

void Timer1A_Init(void(*task)(void), uint32_t period);

#endif //__TIMER_H__
