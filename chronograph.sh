# An operations script that makes working with this repository easier.
# Written by Tiger Sachse.

SAMPLE_DIR="samples"
ANALYZER_NAME="analyzer"
GRAPHER_NAME="grapher.py"
BUILD_DIR="/tmp/temporary_build_dir_parallel_divsufsort"
SAMPLES="code/sources
         music/pitches
         protein/proteins
         dna/dna
         nlang/english.1024MB
         xml/dblp.xml"

# Download sample text files from the Internet.
download_samples() {
    rm -rf $SAMPLE_DIR
    mkdir $SAMPLE_DIR

    for SAMPLE in $SAMPLES; do
        wget http://pizzachili.dcc.uchile.cl/texts/$SAMPLE.gz -P $SAMPLE_DIR
    done
        
    gzip -d $SAMPLE_DIR/*
}

# Install necessary dependencies for this repository to function.
install_dependencies() {
    if [[ $EUID -ne 0 ]]; then
        echo "This operation must be run as root." 
        exit 1
    fi

    # This function needs these packages to build the required divsufsort library.
    apt install gzip git cmake make g++-7 python3-matplotlib

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
analyze_text() {
    g++-7 -fcilkplus source/$ANALYZER_NAME.cpp -o $ANALYZER_NAME \
        -l cilkrts -l divsufsort -l libprange &&
    ./$ANALYZER_NAME "$@"
    rm -rf $ANALYZER_NAME
}

# Analyze the results using the provided Python script.
graph_data() {
    python3 source/$GRAPHER_NAME $1
}

# Main entry point of this script.
case "$1" in
    "--download-samples")
        download_samples
        ;;
    "--install-deps")
        install_dependencies
        ;;
    "--analyze")
        analyze_text "${@:2}"
        ;;
    "--graph")
        graph_data $2
        ;;
esac
