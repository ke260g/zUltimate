export PATH=$PATH:/Users/qdazzle/.Haorong/local/bin

function selfupdate() {
    ftpAddr=192.168.15.15
    ftpPort=22223
    selfPos="env/mac_env.sh"
}

function v() {
    PYTHONPATH=/usr/local/lib/python3.6/site-packages vim $@
}

function xcode-ver() {
    local ver=''
    if [[ $# -ne 1 ]]; then
        echo "usage: xcode-ver < 7 | 8 | 9 >"
    fi
    case $1 in
        "7")
            ver=''
            ;;
        "8" | "9")
            ver=$1
            ;;
        *)
            echo version $1 is not suppport
            return
            ;;
    esac

    cmd="/usr/bin/sudo xcode-select -s /Applications/Xcode${ver}.app/Contents/Developer"
    echo $cmd
    `echo $cmd`
}

function svnInfo() {
    local _USERNAME="浩荣"
    local _PASSWORD="ZDc3ZTQ0MjY4N2NjZDc1Zjk5NzAxMmY0"
    echo "--username ${_USERNAME} --password ${_PASSWORD} --no-auth-cache"
}
