/************************************************************************************** 
*   日期    ：2013年11月6日 (版权所有，翻版必究) 
*   目标    ：ARM Cortex-M0 LPC1114 
*   编译环境：KEIL 4.60
*   外部晶振：10Mhz(主频50M)
*   功能    ：学会使用自动波特率测试功能
*   作者    ：Ration
*   官方网址：http://www.rationmcu.com
*   淘宝店  ：http://ration.taobao.com
**************************************************************************************/
#include "lpc11xx.h"
#include "uart.h"

void AutoBaud(void)
{
	uint32_t baud;
	uint8_t dlm,  dll;

	LPC_UART->ACR  = 0x03;							// 允许自动波特率
	while ((LPC_UART->ACR & 0x01) != 0x0);			// 等待自动波特率测量完成
	LPC_UART->ACR 		 |= 0x01 << 8;				// 清除自动波特率中断标志
	LPC_UART->LCR = 0x83;
	dlm   = LPC_UART->DLM;
	dll   = LPC_UART->DLL;
	LPC_UART->LCR = 0x03;
	LPC_UART->FCR = 0x07;    //允许FIFO，清空RxFIFO 和 TxFIFO
	baud  = ((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV) / (16 * (256 * dlm + dll)));
	UART_send_byte(baud>>24);
	UART_send_byte(baud>>16);
	UART_send_byte(baud>>8);
	UART_send_byte(baud);
}

/**********************************************/
/*               主函数                       */
/**********************************************/
int main()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // 使能IOCON时钟 
	LPC_IOCON->PIO1_6 &= ~0x07;    
	LPC_IOCON->PIO1_6 |= 0x01; //把P1.6脚设置为RXD
	LPC_IOCON->PIO1_7 &= ~0x07;	
	LPC_IOCON->PIO1_7 |= 0x01; //把P1.7脚设置为TXD
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // 禁能IOCON时钟

	LPC_SYSCON->UARTCLKDIV = 0x1;	//时钟分频值为1	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//允许UART时钟
	
	while(1)
	{
		AutoBaud();
	}
}







