apt install wavpack cuetools flac mac shntool


# split flac/wav from cue
shntool split -t "%p" -o flac -d out_dir -f input.cue input.flac
### -t "%p" format the filename of the output *.flac