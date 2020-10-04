// 需要两个栈才能实现一个队列的; 脑补一下push方法的 ABC 插入过程
class MyQueue {
    stack<int> s;
public:
    /** Initialize your data structure here. */
    MyQueue() {

    }
    
    /** Push element x to the back of queue. */
    void push(int x) {
        stack<int> t;
        stack<int> _t;
        t.push(x);
        while (!s.empty()) {
            _t.push(s.top());
            s.pop();
        }
        while (!_t.empty()) {
            t.push(_t.top());
            _t.pop();
        }
        swap(s, t);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        int x = s.top();
        s.pop();
        return x;
    }
    
    /** Get the front element. */
    int peek() {
        return s.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return s.empty();
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */