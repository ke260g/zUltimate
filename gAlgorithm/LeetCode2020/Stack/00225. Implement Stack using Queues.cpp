// 用 c++ 的队列实现栈
// push: 先push到一个新的队列中; 再把之前队列逐一push新的队列中; 最后swap
// top:  就是队列的front
class MyStack {
    queue<int> q;
public:
    /** Initialize your data structure here. */
    MyStack() {  }
    
    /** Push element x onto stack. */
    void push(int x) {
        queue<int> p;
        p.push(x);
        while(!q.empty())  {
            p.push(q.front());
            q.pop();
        }
        swap(p, q);
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int x = q.front();
        q.pop();
        return x;
    }
    
    /** Get the top element. */
    int top() {
        return q.front();
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */