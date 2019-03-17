from fabric import Connection
from fabric import Config

mConn = Connection(
        host='192.168.11.228',
        user='qdazzle',
        connect_kwargs={
            'password': '123456'
        })
mConn.run('cd workplace/t_game_packer/build_scripts/srv/ && pwd')