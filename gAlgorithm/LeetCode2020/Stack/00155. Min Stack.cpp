// 最小栈: 常数复杂度返回最小值
// 即 O(1) 
// 本质上就是在 push / pop 的时候维护最小值
class MinStack {
    vector<int> stk;
    int _min;
public:
    /** initialize your data structure here. */
    MinStack() {
        _min = 0;
    }
    
    void push(int x) {
        stk.push_back(x);
        if (stk.size() == 1)
            _min = x;
        else
            _min = min(_min, x);
    }
    
    void pop() {
        if (stk.empty())
            return;
        int x = stk[stk.size()-1];
        stk.pop_back();
        if (stk.empty()) {
            _min = 0;
        } else if (x == _min) {
            _min = stk[0];
            for (int i = 1; i < stk.size(); ++i)
                _min = min(_min, stk[i]);
        }
    }
    
    int top() {
        return stk[stk.size()-1];
    }
    
    int getMin() {
        return _min;
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */