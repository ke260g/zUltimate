def InPost2Pre(inorder, postorder):
    if len(inorder) == 0 or len(postorder) == 0:
        return
    curr = postorder[-1]
    i = inorder.index(curr)
    l = InPost2Pre(inorder[:i], postorder[:i])
    r = InPost2Pre(inorder[i+1:], postorder[i:-1])
    result = []
    result.extend(curr)
    l != None and result.extend(l)
    r != None and result.extend(r)
    return result

def InPre2Post(inorder, preorder):
    if len(inorder) == 0 or len(preorder) == 0:
        return
    curr = preorder[0]
    i = inorder.index(curr) 
    l = InPre2Post(inorder[0:i], preorder[1:i+1])
    r = InPre2Post(inorder[i+1:], preorder[i+1:])
    result = []
    l != None and result.extend(l)
    r != None and result.extend(r)
    result.extend(curr)
    return result

def getHeight(tree):
    if tree == None:
        return 0
    return max(getHeight(tree.l), getHeight(tree.r)) + 1

class BSTree:
    def delNode(v):
        pass
    def addNode(v):
        pass
    def getNode(v):
        pass

# test units
inorder='ADEFGHMZ'
preorder='GDAFEMHZ'
outorder='AEFDHZMG'

result = InPre2Post(inorder, preorder)
print(''.join(result))

result = InPost2Pre(inorder, outorder)
print(''.join(result))