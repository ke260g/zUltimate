# $1 $2 must be absolute-path
function cp_parents() {
    in=$1
    out=$2
    suffix=$3
    if [ ! -d $in ]; then
        echo "[Not a directory: $in]"
        return
    fi
    cd $in
    find . -name "*.${suffix}" | xargs -I {} cp --parents {} $out
    cd - >/dev/null
}

# $1 as ipath: a ecllipse project
# $2 as opath: a gradle module
function sync_module() {
    ipath=$1
    opath=$2

    rm -rf ${opath}/src/main/jniLibs/*
    cp_parents ${ipath}/libs/ ${opath}/src/main/jniLibs so

    rm -rf ${opath}/libs/*
    cp ${ipath}/libs/*.jar ${opath}/libs

    if [ -d ${ipath}/assets ] && [ ! -z $(ls ${ipath}/assets) ]; then
        rm -rf ${opath}/src/main/assets/*
        cp -r ${ipath}/assets/* ${opath}/src/main/assets
    fi

    rm -rf ${opath}/src/main/res/*
    cp -r ${ipath}/res/* ${opath}/src/main/res

    rm -rf ${opath}/src/main/java/*
    cp_parents ${ipath}/src ${opath}/src/main/java java

    rm -rf ${opath}/src/main/aidl/*
    cp_parents ${ipath}/src ${opath}/src/main/aidl aidl

    cp ${ipath}/AndroidManifest.xml ${opath}/src/main
}

function sync_liboutput() {
    pros=(
    x3dgame_sdk
    x3dgame_sdk_anti
    x3dgame_sdk_korea
    x3dgame_sdk_taiwan_zian
    x3dgame_sdk_xiaoy
    x3dgame_sdk_xinma
    )

    ipath_prefix=$(cd ../t7/liboutput ; pwd)
    opath_prefix=$(cd ./liboutput/t7/ ; pwd)

    for pro in ${pros[@]}; do
        echo "[pro: ${pro}]"

        ipath=${ipath_prefix}/${pro}
        opath=${opath_prefix}/${pro}/liboutput
        echo From:${ipath} To:${opath}
        sync_module ${ipath} ${opath}
    done
}

function sync_x3dgame() {
    pros=(
    # commonsdk         # finish
    # commonsdk_haiwai  # finish
    # commonsdk_xiaoy   # finish
    # commonsdk_xinma   # finish
    # korea_google      # finish
    # korea_onestore    # finish
    )

    ipath_prefix=$(cd ../t7/x3dgame-exe/; pwd)
    opath_prefix=$(cd  ./x3dgame-exe/t7/; pwd)
    for pro in ${pros[@]}; do
        echo "[pro: ${pro}]"
        # module
        mods=$(cat ${opath_prefix}/${pro}/android/settings.gradle | sed -e "s|\r$||" | sed -n -e "/include/s|^.*:\([^']*\)'|\1|p" | egrep -v -e "^app$|^liboutput$" | sed -e ":T N; s|\n| |; bT")

        for mod in ${mods[@]}; do
            ipath=${ipath_prefix}/${pro}/sdk_library/${mod}
            opath=${opath_prefix}/${pro}/android/${mod}

            if [[ ${pro} =~ korea ]]; then
                ipath=${ipath_prefix}/${pro}/${mod}
            fi
            echo ${mod}

            echo From:${ipath} To:${opath}
            sync_module ${ipath} ${opath}
        done

        # app
        ipath=${ipath_prefix}/${pro}/android
        opath=${opath_prefix}/${pro}/android/app
        echo From:${ipath} To:${opath}
        sync_module ${ipath} ${opath}
    done
}
# sync_liboutput
# sync_x3dgame