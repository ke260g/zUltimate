rsync -rdt rsync://IPADDR:RsyncPort/

rsync -azv $src $dst
rsync -azv --delete $src $dst # delete dst extract files