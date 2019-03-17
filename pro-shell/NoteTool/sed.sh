# merge odd and even line
sed -e 'N; s|\n| |'
# merge the whole file into a single line
sed -e ':T N; s|\n| |; bT'
# delete all the blank line
sed -e '/./!d'
# delete the beginning blank line
sed -e '/./,$!d'
# delete the blank-space at the end of line
sed -e 's/ \{1,\}$//'
# convert the multi-blank-space to a-single-blank at the begin of line
sed -e 's|^ \{1,\}| |'
# add a line at the begine of the file
sed -e '1i\content'