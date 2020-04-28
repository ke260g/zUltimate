# manual lacp:
When detect physical port is up, then begin to send to peer-port.

# static lacp:
When detect physical port is up, then begin to negotiate, wait for the response from peer-port.

1. each port is **disable** and **block** before boot up
2. each port is **enable** and **block** after first initialize
3. port become **up** and **block** after insert wire

`insert wire`
port is  **up** and **block** >
hardware(up) > sdk(up) > switchd > ovsdb(up) >
lacp-daemon >
lacp-daemon negotiate with peer-port >
lacp negotiation is finished >
ovsdb(forward) > switchd > sdk(forward) > hardware(forward) >
port begin to forward

port (up & block): can recv packet matching acl only.
port (up & block): can send packet by kernel-driver, but not forward via asic
port (up & forward): can send/recv packet without restrict