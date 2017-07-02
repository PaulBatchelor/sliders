#include <stdio.h>
#include <lo/lo.h>
#include <signal.h>
#include <soundpipe.h>

#ifdef BUILD_SPORTH
#include <sporth.h>
#include <stdlib.h>
#endif

#ifdef PD
#include <m_pd.h>
#endif

#include "sliders.h"

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

static void osc_error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

static int monome_listener(const char *path, const char *types, lo_arg ** argv,
                    int argc, void *data, void *user_data);

int sliders_init(sliders_d *slide)
{
    slide->selected = 0;
    slide->inc_sm = 0.007;
    slide->inc_lg = 0.001;

#ifdef STANDALONE 
    sp_create(&slide->sp);
#endif
    sp_ftbl_create(slide->sp, &slide->vals, 8);
    slide->begin = 1;
    return SLIDER_OK;
}

int sliders_begin(sliders_d *slide)
{
    slide->t = lo_address_new(NULL, "8080");
    slide->st = lo_server_thread_new("8000", osc_error);
    //if(slide->st == NULL) return SLIDER_NOT_OK;
    lo_server_thread_add_method(slide->st, "/monome/press", "iii", 
            monome_listener, slide);
    lo_server_thread_start(slide->st);
    sliders_clear(slide);
    sliders_led(slide, 0, 7, 1);
    griffin_start(slide);
}

int sliders_clean(sliders_d *slide)
{
#ifdef STANDALONE
    sp_ftbl_destroy(&slide->vals);
    sp_destroy(&slide->sp);
#endif
    griffin_stop(slide);
    lo_server_thread_free(slide->st);
    return SLIDER_OK;
}

static int monome_listener(const char *path, const char *types, lo_arg ** argv,
                    int argc, void *data, void *user_data)
{
    sliders_d *slide = user_data;
    sliders_press_handler(slide, argv[0]->i, argv[1]->i, argv[2]->i);
    fflush(stdout);

    return 1;
}

int sliders_select(sliders_d *slide, int col)
{
    slide->selected = col; 
    sliders_led_row(slide, 7, 1 << col);
    return SLIDER_OK;
}

int sliders_act(sliders_d *slide)
{
    SPFLOAT *tbl;
    if(slide->begin) {
        slide->begin = 0;
        sliders_begin(slide);
    }
    if(slide->gd.trigme == -1) {
        slide->gd.trigme = 0;
        tbl = slide->vals->tbl;
        tbl[slide->selected] -= sliders_incr(slide);
        tbl[slide->selected] = min(tbl[slide->selected], 1);
        tbl[slide->selected] = max(tbl[slide->selected], 0);
        sliders_set_val(slide);
    } else if(slide->gd.trigme == 1) {
        slide->gd.trigme = 0;
        SPFLOAT *tbl = slide->vals->tbl;
        tbl[slide->selected] += sliders_incr(slide);
        tbl[slide->selected] = min(tbl[slide->selected], 1);
        tbl[slide->selected] = max(tbl[slide->selected], 0);
        sliders_set_val(slide);
    }

    return SLIDER_OK;
}

void sliders_set_incr(sliders_d *slide, SPFLOAT f)
{
    slide->inc_sm = f;
}

SPFLOAT sliders_incr(sliders_d *slide)
{
    return slide->inc_sm;
}


#ifdef BUILD_MAIN

static volatile int g_keep_running = 1;

static void handler(int dum) 
{
    g_keep_running = 0;
}


int main(int argc, char *argv[])
{
    sliders_d slide;
    sliders_init(&slide);
    signal(SIGINT, handler); 
    while(g_keep_running) {
        sliders_act(&slide);
        usleep(1000);
    }
    printf("cleaning up\n");
    sliders_clean(&slide);
    return 0;
}
#endif

#ifdef BUILD_SPORTH
static int sliders(plumber_data *pd, sporth_stack *stack, void **ud)
{
    sliders_d *slide;
    const char *str;
    switch(pd->mode) {
        case PLUMBER_CREATE: {
            if(sporth_check_args(stack, "s") != PLUMBER_OK) {
                fprintf(stderr, "sliders: incorrect args\n");
                return PLUMBER_NOTOK;
            }
            slide = malloc(sizeof(sliders_d));
            *ud = slide;
            slide->sp = pd->sp;
            str = sporth_stack_pop_string(stack);
            sliders_init(slide);
            plumber_ftmap_add(pd, str, slide->vals);
            }
            break;
        case PLUMBER_INIT: {
            str = sporth_stack_pop_string(stack);
            slide = *ud;
        }
            break;
        case PLUMBER_COMPUTE:
            slide = *ud;
            sliders_act(slide);
            break;
        case PLUMBER_DESTROY:
            slide = *ud;
            fprintf(stderr, "cleaning up\n");
            sliders_clean(slide);
            free(slide);
            break;
    }
    return PLUMBER_OK;
}

plumber_dyn_func sporth_return_ugen()
{
    return sliders;
}
#endif

#ifdef PD
#include "pd.c"
#endif
