static t_class *sliders_class;

typedef struct _sliders
{
    t_object x_obj; 	/* obligatory header */
    t_float x_f;    	/* place to hold inlet's value if it's set by message */
    sliders_d slide;
    t_outlet *out[8];
    t_outlet *gate;
} t_sliders;

static t_int *sliders_perform(t_int *w)
{
    t_sliders *x = (t_sliders *)(w[1]);
    SPFLOAT tmp;
    int n = (int)(w[2]);
    int c = 0;
    int a;
    sliders_act(&x->slide);

    return (w+3);
}

static void sliders_dsp(t_sliders *x, t_signal **sp)
{
    dsp_add(sliders_perform, 2, x, 
            sp[0]->s_n
            );

}

static void *sliders_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_sliders *x = (t_sliders *)pd_new(sliders_class);
    x->x_f = 0;
    sliders_init(&x->slide);
    for(i = 0; i < 8; i++) {
        x->out[i] = outlet_new(&x->x_obj, &s_float);
    }
    x->gate = outlet_new(&x->x_obj, &s_float);
    return (x);
}

static void sliders_free(t_sliders *x) 
{
    sliders_clean(&x->slide);
}

static void sliders_bang(t_sliders *x)
{
    int i;
    sliders_act(&x->slide);
    for(i = 0; i < 8; i++) {
        outlet_float(x->out[i], x->slide.vals->tbl[i]);
    }
    outlet_float(x->gate, x->slide.gd.gate);
}

static void sliders_float(t_sliders *x, t_float f)
{
    sliders_set_incr(&x->slide, f);
}

void sliders_setup(void)
{
    sliders_class = class_new(
            gensym("sliders"), 
            (t_newmethod)sliders_new, 
            (t_method)sliders_free, 
    	    sizeof(t_sliders), 
            0, 
            A_DEFFLOAT, 
            0);

    CLASS_MAINSIGNALIN(sliders_class, t_sliders, x_f);
    class_addbang(sliders_class, sliders_bang);
    class_addfloat(sliders_class, sliders_float);
}
