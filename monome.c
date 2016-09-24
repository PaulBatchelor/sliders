#include <lo/lo.h>
#include <stdio.h>
#include <soundpipe.h>
#include "sliders.h"

int sliders_led(sliders_d *slide, int x, int y, int s)
{
    if(lo_send(slide->t, "/monome/led", "iii", x, y, s) == -1) {
        printf("OSC error: %d: %s\n", lo_address_errno(slide->t),
                lo_address_errstr(slide->t));
        return SLIDER_NOT_OK;
    }
    return SLIDER_OK;
}

int sliders_led_row(sliders_d *slide, int row, int val)
{
    if(lo_send(slide->t, "/monome/led_row", "ii", row, val) == -1) {
        printf("OSC error: %d: %s\n", lo_address_errno(slide->t),
                lo_address_errstr(slide->t));
        return SLIDER_NOT_OK;
    }
    return SLIDER_OK;
}

int sliders_led_col(sliders_d *slide, int col, int val)
{
    if(lo_send(slide->t, "/monome/led_col", "ii", col, val) == -1) {
        printf("OSC error: %d: %s\n", lo_address_errno(slide->t),
                lo_address_errstr(slide->t));
        return SLIDER_NOT_OK;
    }
    return SLIDER_OK;
}

int sliders_press_handler(sliders_d *slide, int x, int y, int s)
{
    if(y == 7) {
        sliders_select(slide, x);
    }
    return SLIDER_OK;
}

int sliders_set_val(sliders_d *slide) 
{
    SPFLOAT *tbl = slide->vals->tbl;
    SPFLOAT val = tbl[slide->selected];
    int ival = (int)(val * 7);
    int out = 0;
    while(ival--) out |= (1 << ival);
    out |= 1 << 7;
    sliders_led_col(slide, slide->selected, out);
    return SLIDER_OK;
}

int sliders_clear(sliders_d *slide)
{
    if(lo_send(slide->t, "/monome/clear", "") == -1) {
        printf("OSC error: %d: %s\n", lo_address_errno(slide->t),
                lo_address_errstr(slide->t));
        return SLIDER_NOT_OK;
    }
    return SLIDER_OK;
}
