#include "imx6u.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"
#include "bsp_uart.h"
#include "stdio.h"
static unsigned char led_state = OFF;
unsigned int a,b;
int main(void)
{
    int_init();
    imx6u_clk_init();
    clk_enable();
    delay_init();
    uart_init();
    led_init();
    beep_init();
    key_init();
    exti_init();
    epit1_init(0,66000000/100);/*初始化定时器1,1分频66MHz,定时时间为10ms,用于按键消抖*/
    while(1)
    {
        /*
        led_state = !led_state;
        led_switch(LED0,led_state);
        delay_ms(100);
        
        puts("请输入一个字符:");
        a = getc();
        putc(a); //发送输入的字符
        puts("\r\n");

        puts("您输入的字符为:");
        putc(a);
        puts("\r\n");
        */
       printf("请输入两个整数，并使用空格隔开：");
       scanf("%d %d",&a,&b);
       printf("\r\n%d + %d = %d\r\n",a,b,a+b);

       printf("%d的十六进制是%#x\r\n",a+b,a+b);
    }
    return 0;
}
