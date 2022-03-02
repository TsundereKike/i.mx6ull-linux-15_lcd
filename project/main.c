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
#include "bsp_lcdapi.h"
static unsigned char led_state = OFF;
unsigned int color;
unsigned char color_num = 0;
unsigned int LCD_color_trans[10] = {
             LCD_BLUE,LCD_BROWN,LCD_CYAN,LCD_DARKGREEN,LCD_DARKMAGENTA,LCD_DARKYELLOW,
             LCD_GRAY,LCD_LIGHTGREEN,LCD_LIGHTRED,LCD_WHITE};
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
    
    lcd_fill(400, 300, 600, 400, LCD_BLUE);

    lcd_show_string(10,10,360,32,32,"this tmh's lcd test");
    lcd_show_string(10,40,260,24,24,"this tmh's lcd test");
    lcd_show_string(10,110,260,16,16,"this tmh's lcd test");
    lcd_show_string(10,180,260,12,12,"2022/3/2");
    while(1)
    {
        color_num++;
        lcd_clear(LCD_color_trans[color_num]);
        lcd_shownum(400,240,31415926,8,32);
        lcd_show_string(10,10,360,32,32,"this tmh's lcd test");
        lcd_show_string(10,40,260,24,24,"this tmh's lcd test");
        lcd_show_string(10,110,260,16,16,"this tmh's lcd test");
        lcd_show_string(10,180,260,12,12,"2022/3/2");
        if(color_num>=10)
            color_num = 0;
        led_state = !led_state;
        led_switch(LED0,led_state);
        delay_ms(1000);
    }
    return 0;
}
