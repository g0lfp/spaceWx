# spaceWx
A simple two line LCD displaying current space weather condx.

The two line display shows, on the top line.. Time HH:MM in UTC. followed by XRAY:Fn.n where F is the current flare class. n.n is the current value of the flare. This is a logarithmic scale. Range A-B is background, C is the lowest flare class, M is a Medium flare, and finally X is an eXtreme flare.
Second line rotates every few seconds...
1. SFI + A + K indices
2. Sunspot number
3. Bz and Bt
4. Solar Wind Speed.

Hardware: www.doit.am ESPduino and a 2x16 LCD with I2C daughter board. This keeps the connections simple in that it only needs 4 wires!

Software: Based on my 'Boilerplate' code. 
