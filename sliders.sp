_sliders "sliders.so" fl
_t _sliders fe
_sliders fc

_shm "shm_server.so" fl

_s0 5678 _shm fe
_s1 5679 _shm fe
_s2 5680 _shm fe
_s3 5681 _shm fe
_s4 5682 _shm fe
_s5 5683 _shm fe
_s6 5684 _shm fe
_s7 5685 _shm fe

0 _t tget _s0 set
1 _t tget _s1 set
2 _t tget _s2 set
3 _t tget _s3 set
4 _t tget _s4 set
5 _t tget _s5 set
6 _t tget _s6 set
7 _t tget _s7 set

_shm fc
0
