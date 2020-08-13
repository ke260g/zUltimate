apt install wavpack cuetools flac mac shntool abcde mkcue

# step_1: extract flac from a cd
abcde -1 -o flac

# step_2: create a cue-template
mkcue > input.cue

# step_3: edit input.cue
nano input.cue

# step_4: split flac/wav from cue
shntool split -t "%t.%p" -o flac -d out_dir -f input.cue input.flac
### -t "%p" format the filename of the output *.flac
