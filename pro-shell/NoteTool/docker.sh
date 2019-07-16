# export a docker's ip with DNAT
# 172.17.0.15 is docker's ip
iptables -t nat -A DOCKER -p tcp --dport 7080 -j DNAT --to-destination 172.17.0.15:7080

# enable strace
sudo docker run --security-opt seccomp:unconfined --name yhr_nhw -it hqj_sw_img /bin/bash
docker run --name yhr_sw -it hqj_sw_img /bin/bash  [create container]
docker start yhr_sw
docker exec -it yhr_sw /bin/bash

# show all ip of dockers, but can not show custom binding ip 
docker inspect -f='{{.Name}} {{.NetworkSettings.IPAddress}} {{.HostConfig.PortBindings}}' $(docker ps -aq)

# create custom network
docker network create --subnet=172.18.0.0/16 docker1
# create container with fixed port and ip
docker run -itd --name yhr_wac --net docker1 --ip 172.18.0.201 -p 17781:80 --security-opt seccomp:unconfined wns_base:latest /bin/bash