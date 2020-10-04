// N 叉树的后续遍历;
// 等同二叉树; 从左往右遍历子树；再遍历自己

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
private:
    void postorderVisit(Node *root, vector<int> & res) {
        if (!root) return;
        for (int i = 0; i < root->children.size(); ++i)
            postorderVisit(root->children[i], res);
        res.push_back(root->val);
    }
public:
    vector<int> postorder(Node* root) {
        vector<int> res;
        postorderVisit(root, res);
        return res;
    }
};