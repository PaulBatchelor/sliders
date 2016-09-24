# sliders: 
# 0: brightness
# 1: release time
# 2: reverb level
# 3: tempo
# 4: volume
(
_sliders "./sliders.so" fl
_vals _sliders fe
_clk var
_tab 4096 gen_sine

_seq '60 65 70 72' gen_vals

# set clock to p-reg 0
3 _vals tget 20 180 scale 4 * bpm2dur dmetro 0 pset

# set exponential envelope to p-reg 1
0 p 0.002 0.01
# lfo-based release time
1 _vals tget 0.005 0.5 scale
tenvx 1 pset

# sequencer
((((0 p 0 _seq tseq) 8 0.1 sine +)
# fifth leaps and throws via preg 3
0 p 0.2 maygate dup 3 pset 7 * +
0.002 port) mtof) 0 phasor


# apply phasor through phase distortion

# input signal is reverse envelope
(1 1 p - )
# amount
(1 0 _vals tget - -0.9 0.01 scale)
0 scale pdhalf

# put through interpolated table lookup
1 0 0 _tab tabread  4 _vals tget 0 0.7 scale *

# ampltidue envelope
1 p *

# reverb throw, smooth out the gate signal
dup (3 p 0.001 port *) dup
20 20 3000 zrev drop 2 _vals tget 0.1 0.5 scale * +

dup
)

