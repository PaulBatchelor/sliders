enum {
SLIDER_OK,
SLIDER_NOT_OK
};

typedef struct {
    void (*func)(int type, int ctl, int val, void *);
    int run;
    pthread_t thread;
    int trigme, okaydone;
    int gate;
    int fid;
} griffin_d;

enum{
STATE=16,
DATA=20,
KNOB=2,
BUTTON=1
};

typedef struct {
    lo_address t;
    lo_server_thread st;
    int selected;
    SPFLOAT inc_sm;
    SPFLOAT inc_lg;
    sp_ftbl *vals;
    sp_data *sp;
    griffin_d gd;
    int begin;
} sliders_d;

int sliders_init(sliders_d *slide);
int sliders_led(sliders_d *slide, int x, int y, int s);
int sliders_led_row(sliders_d *slide, int row, int val);
int sliders_led_col(sliders_d *slide, int col, int val);
int sliders_press_handler(sliders_d *slide, int x, int y, int s);
int sliders_act(sliders_d *slide);
int sliders_set_val(sliders_d *slide);
int sliders_select(sliders_d *slide, int col);
int sliders_clean(sliders_d *slide);
int sliders_clear(sliders_d *slide);
SPFLOAT sliders_incr(sliders_d *slide);

int griffin_start(sliders_d *slide);
int griffin_stop(sliders_d *slide);
