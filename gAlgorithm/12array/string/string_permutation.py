import pprint

def ary2str(a):
    return ''.join(a)

def permutation(s, begin, end):
    uniq = False
    if begin == end:
        print(ary2str(s))
        return

    marked = {}
    for i in range(begin, end):
        if uniq == True:
            if i != begin and marked.get(s[i]) == True:
                continue
            marked[s[i]] = True
        s[begin], s[i] = s[i], s[begin]
        permutation(s, begin + 1, end)
        s[i], s[begin] = s[begin], s[i]

# s = [ c for c in '21223' ]
# s = [ c for c in 'abcd' ]
s = [ c for c in 'XYYZZZZ' ]
permutation(s, 0, len(s))
