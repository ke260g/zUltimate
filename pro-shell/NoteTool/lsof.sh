p # pid 
-u # user
-t # show pid only

# network
losf -i            # means LISTENING & ESTABLISHED
lsof -i TCP:1-1024 # tcp port range 1-1024
lsof -i TCP:22     # tcp specific port  
lsof -i 4          # ipv6 socket
lsof -i 6          # ipv4 socket