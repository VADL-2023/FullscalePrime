# Default values
outputfile="sdr1_output.txt"
frequency="144.97M"
device="0"

# Read the flags and perform the necessary assignemnts
while getopts f:o:d: flag
do
    case "${flag}" in
        f)
            frequency=${OPTARG}
            ;;
        o)
            outputfile=${OPTARG}
            ;;
        d)
            device=${OPTARG}
            ;;
        ?)
            echo "script usage: $(basename \$0) [-f frequency] [-o output file] [-d device serial number]" >&2
            exit 1
            ;;
    esac
done

# Start rtl_fm and direwolf
(rtl_fm -M fm -f $frequency -s 24000 -d 00000100 -g 42 - 2>/dev/null & echo $! > rtl_fm_pid) | direwolf -c /home/vadl/FullscalePrime/sdr1.conf -r 24000 -D 1 -B 1200 - > $outputfile &

# Print out the process id (pid)
cat rtl_fm_pid