from fabric import Connection, Config
from invoke import Responder
def FabricSshBuilder(host, config):
    # basic, http://docs.fabfile.org/en/2.4/api/connection.html#module-fabric.connection
    # extra, http://docs.paramiko.org/en/latest/api/client.html#paramiko.client.SSHClient.connect
    return Connection(host=host,
            user=config['user'],
            port=config['port'],
            connect_kwargs=config['extra'])
