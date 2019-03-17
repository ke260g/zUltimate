from FabricHelper import FabricSshBuilder
from pprint import pprint

# 123.58.17.218
config_wensenh5 = {
        'basic': {
            'user':'wensen',
            'port':'62919',
            },
        'extra': {
            'key_filename':['/tmp/wensen_rsa.rsa']
            }
        }

machine_configs_android = {
        'device': [ '192.168.11.' + suffix for suffix in [  '225', '240', '176' ] ],
        'common': {
            'user':'packer',
            'port':'22',
            'extra': { 'password':'commonsdkpacker' }
            }
        }

machine_configs_ios = {
        'device': [ '192.168.11.' + suffix for suffix in [ '201', '204', '210', '202' ] ],
        'common' : {
            'user':'qdazzle',
            'port':'22',
            'extra': {
                'password': '123456'
                }
            },
        'custom': {
            '192.168.11.202': {
                'extra': { 'password': '12345678' }
                },
            },
        }

def taskSvnUpS10(conn):
    svn_username='sdk_mac'
    svn_password='lfdalld'
    svn_config=' --non-interactive --username=sdk_mac --password=lfdalld --no-auth-cache '
    pathMotherPacks='~/workplace/common_ios/MotherPacks'
    with conn.cd(pathMotherPacks):
        conn.run('svn revert S10 --depth=infinity ' + svn_config)
        conn.run('svn up S10 ' + svn_config)

# do connection task
def doConnTask(configs, task):
    common_config = configs['common']
    custom_config = configs.get('custom')
    for host in configs['device']:
        config = common_config.copy()
        if custom_config and custom_config.get(host):
            config.update(custom_config.get(host) or {})

        print('[Symphony] host in %s'%(host))
        try:
            conn = FabricSshBuilder(host, config)
            task(conn)
        except:
            print('Error in: ' + host)

def taskDebug(conn):
    conn.run('uname -s')
doConnTask(machine_configs_android, taskDebug)
