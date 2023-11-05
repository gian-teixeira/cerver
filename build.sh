PAGES_SRC="\"$(pwd)/pages\""
BUFFER_SIZE=1024

cc='gcc'
flags="-I./src/shared -fopenmp -DPAGES_SRC=${PAGES_SRC} -DBUFFER_SIZE=${BUFFER_SIZE}"
src_folder='src'
build_folder='build'
bin_folder='bin'

mkdir -p $build_folder
mkdir -p $bin_folder

target_folder=${!OPTIND}
target_build_folder="${build_folder}/${target_folder}"
mkdir -p $target_build_folder

function build {
    for cfile in $(find "${src_folder}/$1" -name '*.c'); do
        filename=$(basename $cfile .c)
        obj="$target_build_folder/$filename.o"
        $cc $flags -c $cfile -o $obj
    done
}

build "shared"
build "${target_folder}"

$cc $flags $(find $target_build_folder -name '*.o') -o "${bin_folder}/${target_folder}"