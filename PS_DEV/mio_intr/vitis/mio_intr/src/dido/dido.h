/**********************************************************************
 * @file dido.h
 * @brief
 * @author wangh (wanghuan3037@fiberhome.com)
 * @version 1.0
 * @date 2026-03-21
 *
 * @copyright Copyright (c) 2026s FiberHome. All rights reserved.
 **********************************************************************/
#ifndef DIDO_H
#define DIDO_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

#define PS_LED1 0 // ps_led1 固定为mio0
#define PS_LED2 9 // ps_led2 固定为mio9

#define PS_KEY1 10 // ps_key1 固定为mio10
#define PS_KEY2 11 // ps_key2 固定为mio11

#define PL_KEY2 54 // pl_key2 固定为emio54
#define PL_LED1 55 // pl_led1 固定为emio55



    int gpio_init();

    void set_ps_led(uint32_t led_num, uint32_t state);
    void set_pl_led(uint32_t led_num, uint32_t state);
    uint32_t read_pl_key(uint32_t key_num);

    int axi_gpio_init();
    void axi_gpio_interrupt_handler(void *CallbackRef);
    
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // DIDO_H
