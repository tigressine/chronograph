# An operations script that makes working with this repository easier.
# Written by Tiger Sachse.

SAMPLE_DIR="samples"
ANALYZER_NAME="analyzer"
GRAPHER_NAME="grapher.py"
SAMPLES="code/sources
         music/pitches
         protein/proteins
         dna/dna
         nlang/english.1024MB
         xml/dblp.xml"

# Download sample text files from the Internet.
download_samples() {
    mkdir $SAMPLE_DIR

    for SAMPLE in $SAMPLES; do
        wget http://pizzachili.dcc.uchile.cl/texts/$SAMPLE.gz -P $SAMPLE_DIR
    done
        
    gzip -d $SAMPLE_DIR/*
}

# Install the project.
install_project() {
    if [[ $EUID -ne 0 ]]; then
        echo "This operation must be run as root." 
        exit 1
    fi

    # This function needs these packages to build the required divsufsort library.
    apt install gzip git cmake make g++-7 python3-matplotlib

    # Override any existing compilers and use the g++-7 compiler.
    export CC="g++-7;$CC"
    export CXX="g++-7;$CXX"

    rm -rf build
    mkdir build
    cd build

    # Install this project using cmake.
    cmake ..
    make
    make install

    cd ..
    rm -rf build
}

# Run the repository driver to test the divsufsort library.
analyze_text() {
    compile_analyzer &&
    ./$ANALYZER_NAME "$@"
    rm -rf $ANALYZER_NAME
}

# Compile the analyzer.
compile_analyzer() {
    g++-7 -fcilkplus timer/$ANALYZER_NAME.cpp -o $ANALYZER_NAME \
        -l cilkrts -l divsufsort -l libprange
}

# Analyze the results using the provided Python script.
graph_data() {
    python3 timer/$GRAPHER_NAME $1
}

# Main entry point of this script.
case "$1" in
    "--download-samples")
        download_samples
        ;;
    "--install")
        install_project
        ;;
    "--analyze")
        analyze_text "${@:2}"
        ;;
    "--graph")
        graph_data $2
        ;;
    "--compile")
        compile_analyzer
        ;;
esac
