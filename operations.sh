# Operations script to make working with this repository easier.
# Written by Tiger Sachse.

DRIVER_NAME="timer"
ANALYZER_NAME="analyzer.py"
BUILD_DIR="/tmp/temporary_build_dir_parallel_divsufsort"

# Install necessary dependencies for this repository to function.
install_dependencies() {
    if [[ $EUID -ne 0 ]]; then
        echo "This operation must be run as root." 
        exit 1
    fi

    # This function needs these packages to build the required divsufsort library.
    apt install git cmake make g++-7 python3-matplotlib

    # Override any existing compilers and use the g++-7 compiler.
    export CC="g++-7;$CC"
    export CXX="g++-7;$CXX"

    # Clone the divsufsort library repository into a temporary build directory.
    rm -r $BUILD_DIR
    git clone https://github.com/jlabeit/parallel-divsufsort.git $BUILD_DIR
    cd $BUILD_DIR

    mkdir build
    cd build

    # Install the divsufsort library using cmake.
    cmake ..
    make
    make install
}

# Run the repository driver to test the divsufsort library.
run() {
    g++-7 -fcilkplus source/$DRIVER_NAME.cpp -o $DRIVER_NAME \
        -l cilkrts -l divsufsort -l libprange &&
    ./$DRIVER_NAME "$@"
    rm -rf $DRIVER_NAME
}

# Analyze the results using the provided Python script.
analyze() {
    python3 source/$ANALYZER_NAME $1
}

# Main entry point of this script.
case "$1" in
    "--install-deps")
        install_dependencies
        ;;
    "--run")
        run "${@:2}"
        ;;
    "--analyze")
        analyze $2
        ;;
esac
