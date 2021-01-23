/*
 * @lc app=leetcode id=1700 lang=cpp
 *
 * [1700] Number of Students Unable to Eat Lunch
 *
 * https://leetcode.com/problems/number-of-students-unable-to-eat-lunch/description/
 *
 * algorithms
 * Easy (70.18%)
 * Total Accepted:    8.7K
 * Total Submissions: 12.4K
 * Testcase Example:  '[1,1,0,0]\n[0,1,0,1]'
 *
 * The school cafeteria offers circular and square sandwiches at lunch break,
 * referred to by numbers 0 and 1 respectively. All students stand in a queue.
 * Each student either prefers square or circular sandwiches.
 * 
 * The number of sandwiches in the cafeteria is equal to the number of
 * students. The sandwiches are placed in a stack. At each step:
 * 
 * 
 * If the student at the front of the queue prefers the sandwich on the top of
 * the stack, they will take it and leave the queue.
 * Otherwise, they will leave it and go to the queue's end.
 * 
 * 
 * This continues until none of the queue students want to take the top
 * sandwich and are thus unable to eat.
 * 
 * You are given two integer arrays students and sandwiches where sandwiches[i]
 * is the type of the i^​​​​​​th sandwich in the stack (i = 0 is the top of the
 * stack) and students[j] is the preference of the j^​​​​​​th student in the
 * initial queue (j = 0 is the front of the queue). Return the number of
 * students that are unable to eat.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: students = [1,1,0,0], sandwiches = [0,1,0,1]
 * Output: 0 
 * Explanation:
 * - Front student leaves the top sandwich and returns to the end of the line
 * making students = [1,0,0,1].
 * - Front student leaves the top sandwich and returns to the end of the line
 * making students = [0,0,1,1].
 * - Front student takes the top sandwich and leaves the line making students =
 * [0,1,1] and sandwiches = [1,0,1].
 * - Front student leaves the top sandwich and returns to the end of the line
 * making students = [1,1,0].
 * - Front student takes the top sandwich and leaves the line making students =
 * [1,0] and sandwiches = [0,1].
 * - Front student leaves the top sandwich and returns to the end of the line
 * making students = [0,1].
 * - Front student takes the top sandwich and leaves the line making students =
 * [1] and sandwiches = [1].
 * - Front student takes the top sandwich and leaves the line making students =
 * [] and sandwiches = [].
 * Hence all students are able to eat.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: students = [1,1,1,0,0,1], sandwiches = [1,0,0,0,1,1]
 * Output: 3
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= students.length, sandwiches.length <= 100
 * students.length == sandwiches.length
 * sandwiches[i] is 0 or 1.
 * students[i] is 0 or 1.
 * 
 * 
 */
class Solution {
public:
    int countStudents(vector<int>& students, vector<int>& sandwiches) {
        // 场景模拟法; 记录重新排队人数
        // 1. 把学生vector转queue
        // 2. 当学生queue 非空
        // 2.1 如果能够满足首个学生; 
        //     消耗 sandwich
        //     重新排队人数 = 0
        // 2.2 如果不能满足首个学生; 
        //     重新排队
        //     重新排队人数++
        // 2.3 无论是否满足; 干掉首个学生
        // 2.4 当重新排队人数 == 队列长度
        //     说明一整个队伍都没满足
        //     终止循环
        queue<int> qStudents;
        for (auto s: students)
            qStudents.push(s);
        int waiting = 0;
        int i = 0;
        while (!qStudents.empty()) {
            if (qStudents.front() == sandwiches[i]) {
                i++;
                waiting = 0;
            } else {
                qStudents.push(qStudents.front());
                waiting++;
            }
            qStudents.pop();
            if (waiting == qStudents.size())
                break;
        }
        return qStudents.size();
    }
};
