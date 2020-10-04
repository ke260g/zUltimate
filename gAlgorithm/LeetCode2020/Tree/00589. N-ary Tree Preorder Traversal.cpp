// N叉树的前序遍历; 先访问自己; 再从左往右访问子节点
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
public:
    void preorderTravel(Node *root, vector<int> &res) {
        if (!root) return;
        res.push_back(root->val);
        for (int i = 0; i < root->children.size(); ++i)
            preorderTravel(root->children[i], res);
        return;
    }
    vector<int> preorder(Node* root) {
        vector<int> res;
        preorderTravel(root, res);
        return res;
    }
};