#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "sleep.h"
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "dido/dido.h"
#include "xscugic.h"
#include "xgpiops.h"
#include "xgpio.h"

    extern XScuGic InterruptController;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAIN_H
