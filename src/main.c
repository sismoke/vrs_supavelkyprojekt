/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 6.0.0   2016-10-18

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */
#include <stddef.h>
#include <stdio.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include "ringbuffer.h"
#include "i2c.h"
#include "ads1100.h"
#include "sht21.h"
#include "datastore.h"


/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

uint16_t adc_conv_val = 0;
volatile uint16_t printmode  = 0;

int main(void)
{
	SysTick_Config (SystemCoreClock / 1000);
	uart_init();
	gpio_init();
	int_init();
	initI2C1();

	char buff[10];
	uint16_t i;
  while (1)
  {
		datastore_proc();
		if (!gettxfull())
			{
				if (printmode)
					{
						sprintf(buff, "%f4", ((float)adc_conv_val)/(1241.21));
						buff[4] = 'V';
						buff[5] = ' ';
						buff[6] = ' ';
						buff[7] = '\n';
						buff[8] = '\r';
						buff[9] = 0;
					}
				else
					sprintf(buff, "%d\n\r", adc_conv_val);
				for (i = 0; i < 10; i++)
					{
						if (buff[i] == 0)
							break;
						puttxbuff(buff[i]);
					}
				datastore_storedata();
			}
		if (getrxfull())
			{
				if (getrxbuff() == 'm')
					printmode = !printmode;
			}
  }
  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
