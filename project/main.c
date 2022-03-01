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
#include "bsp_lcd.h"
static unsigned char led_state = OFF;
unsigned int color;
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
    lcd_init();
    lcd_draw_point(0,0,LCD_RED);/*左上方*/
    lcd_draw_point(lcd_tft_dev.width-1,0,LCD_RED);/*右上方*/
    lcd_draw_point(0,lcd_tft_dev.height-1,LCD_RED);/*左下方*/
    lcd_draw_point(lcd_tft_dev.width-1,lcd_tft_dev.height-1,LCD_RED);/*右下方*/
    color = lcd_read_point(0,0);
    printf("左上方 = %#x\r\n",color);

    color = lcd_read_point(lcd_tft_dev.width-1,0);
    printf("右上方 = %#x\r\n",color);

    color = lcd_read_point(0,lcd_tft_dev.height-1);
    printf("左下方 = %#x\r\n",color);

    color = lcd_read_point(lcd_tft_dev.width-1,lcd_tft_dev.height-1);
    printf("右下方 = %#x\r\n",color);
    
    while(1)
    {
        led_state = !led_state;
        led_switch(LED0,led_state);
        delay_ms(1000);
    }
    return 0;
}
