BUILD_DIR="/tmp/temporary_build_dir_parallel_divsufsort"

install_dependencies() {
    if [[ $EUID -ne 0 ]]; then
        echo "This operation must be run as root." 
        exit 1
    fi

    apt install git cmake make g++-7

    export CC="g++-7;$CC"
    export CXX="g++-7;$CXX"

    rm -r $BUILD_DIR
    git clone https://github.com/jlabeit/parallel-divsufsort.git $BUILD_DIR
    cd $BUILD_DIR

    mkdir build
    cd build

    cmake ..
    make
    make install
}

case "$1" in
    "--install-deps")
        install_dependencies
        ;;
    "--run")
        ;;
    "--analyze")
        ;;
esac
