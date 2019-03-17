import redis
import requests

#### pack list ###########
REDIS_HOST='192.168.11.240';
REDIS_PORT='4581'
REDIS_PASSWORD='qdazzle'

PACK_TASK_REDIS_KEY='t_game_packer'
##########################

#### backend ##############
BACKEND_HOST='http://backend.test'
PARAMETER_ROUTER='/api/packlist/parameters'
End_PACK_ROUTER='/api/packlist/end'
BEGIN_PACK_ROUTER='/api/packlist/begin'
###########################

task_list = redis.Redis( host=REDIS_HOST, 
                        port=REDIS_PORT, 
                        password=REDIS_PASSWORD)

LOG_FILE=' ../tmp/log/latest.log '

# 1. block to wait task, listen on redis-db
# 2. get id from redis-db
# 3. tell backend that self begin to pack   [post]
# 4. get parameters from backen             [post]
# 5. parse parameters to build a shell_exec command 
# 6. run shell_exec command
# 7. tell backend self end packing          [post]

#### need util
## 1. shell exec
## 2. log util
## 3. requests wrapper

while True:
    print('Begin Waiting Pack Task')
    print('--------------')
    packid=-1
    
    try:
        task = task_list.blpop(PACK_TASK_REDIS_KEY, 0)
        print('task get from redis = ' + str(task))
        
        packid=int(task[1])
        print('packid = %d'%(packid))
        
    except IndexError:
        print('obtain task error')
        continue;
    
    r = None
    
    # tell backend, start to pack
    try:
        r = requests.post(BACKEND_HOST + BEGIN_PACK_ROUTER, data={ "id": packid })
    except Exception as e:
        print('request error, packid = %d'%(packid))
        print(e)
        continue
    
    # obtain parameters
    try:
        r = requests.post(BACKEND_HOST + PARAMETER_ROUTER, data={ "id": packid })
    except Exception as e:
        print('request error, packid = %d'%(packid))
        print(e)
        continue
    
    print("result status code = %d"%(r.status_code))
    
    if r.status_code != 200:
        print('r.status.code not succeeded')
        continue
    
    result_json = r.json()
    print('request parameters = ' + str(result_json))
    

    
    try:
        if result_json['code'] != 0:
            print(u"result_json['code'] not succeeded")
            continue
        
        data = result_json['data']
        
        shell_exec='bash ./build.sh '

        for key in data:
            shell_exec += ' --%s %s '%(key, data[key])
        
        # shell_exec += ' --auto_upload 2>&1 | tee ../tmp/log/latest.log
        shell_exec += ' --auto_upload 2>&1 > ' + LOG_FILE
        
        # exec shell
        
        # get log file
        
        # end package
        try:
            r = requests.post(BACKEND_HOST + End_PACK_ROUTER, data={ "id": packid })
        except Exception as e:
            print('request error, packid = %d'%(packid))
            print(e)
            continue

    except KeyError:
        print('parse parameters error')
        continue