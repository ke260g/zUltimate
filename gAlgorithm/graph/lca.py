'''
# example of tree
{
    'v':10,
    'l':{
        'v':5,
        'l':{},
        'r':{},
    },
    'r':{

    },
}
'''

def LCA(node, lv, rv):
    if node == None:
        return None
    if node.v == lv or node.v == rv: # 找到当前点
        return node
    
    l = LCA(node.l, lv, rv)
    r = LCA(node.r, lv, rv)
    if l and r:
        return node
    if l:
        return l
    if r:
        return r
    else:
        return None
