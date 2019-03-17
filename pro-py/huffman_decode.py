import json
# 'd' means data or raw-data-char
# 'l' means left leaf
# 'r' means right leaf
HuffTree =  {
    "l": {
        "l": {
            "d":"E"
        }, 
        "r": {
            "d":"R"
        }
    }, 
    "r": {
        "l": {
            "l": {
                "l": {
                    "d":"F"
                }, 
                "r": {
                    "d":"D"
                }
            }, 
            "r": {
                "d":"T"
            }
        }, 
        "r": {
            "d":"A"
        }
    }
}

def Decode(Node, Buffer, N, Result, M):
    if not Node:
        return N, M
    d = Node.get('d')
    if d:
        Result[M] = (d)
        return N, M + 1
        # return # as it is leaf
    b = Buffer[N]
    if b == '0':
        return Decode(Node.get('l'), Buffer, N + 1, Result, M)
    elif b == '1':
        return Decode(Node.get('r'), Buffer, N + 1, Result, M)
    
def DecodeMain(Node, Buffer, length):
    N = 0
    M = 0
    Result = [None] * length
    # shortest-first-match
    while N < len(Buffer):
        N, M = Decode(Node, Buffer, N, Result, M)
    return ''.join(Result)
        
RawData = 'AFTERDATAEARAREARTAREA'
# build Huff Tree Map
HuffMap = {'E':'00', 'R':'01', 'F':'1000', 'D':'1001', 'T':'101', 'A':'11' }
CompressData = ''
for c in RawData:
    CompressData += HuffMap[c]
print(CompressData)

Result = DecodeMain(HuffTree, CompressData, len(RawData))
print(Result)
# CompressData = '111000101000110011110111001101110100110110111010011'
# more practical codes need to perform bit-operation