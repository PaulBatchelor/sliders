# Sliders

A control interface using a 8x8 monome and the griffin USB knob,
built to be run inside of Sporth.

## Running
0. Make sure that [sporth-shm](
https://www.github.com/paulbatchelor/sporth-shm) is installed. Liblo is also
needed. Also Sporth (duh). 
1. Start monomeserial 
2. Run "./start_sliders.sh", which is really just:

    sporth -d 0 -n sliders.sp
