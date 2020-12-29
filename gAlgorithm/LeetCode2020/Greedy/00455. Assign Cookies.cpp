// Assume you are an awesome parent and want to give your children some cookies. 
// But, you should give each child at most one cookie.

// Each child i has a greed factor g[i], which is the minimum size of a cookie 
// that the child will be content with; 
// and each cookie j has a size s[j]. If s[j] >= g[i],
// we can assign the cookie j to the child i, 
// and the child i will be content. 

// Your goal is to maximize the number of your content children and output the maximum number.

// Input: g = [1,2,3], s = [1,1]
// Output: 1
// Explanation: You have 3 children and 2 cookies. The greed factors of 3 children are 1, 2, 3. 
// And even though you have 2 cookies, since their size is both 1, you could only make the child whose greed factor is 1 content.
// You need to output 1.

// Input: g = [1,2], s = [1,2,3]
// Output: 2
// Explanation: You have 2 children and 3 cookies. The greed factors of 2 children are 1, 2. 
// You have 3 cookies and their sizes are big enough to gratify all of the children, 
// You need to output 2.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/assign-cookies
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        int count = 0, i = g.size()-1, j = s.size()-1; 
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        // 优先用较大的块 feed 胃口大的
        // 如果不能喂饱这个小孩 直接干掉 (不要用 大顶堆 维护半饱的小孩; 直接不喂了)
        while (i >= 0 && j >= 0) {
            // 先把胃口大地小孩干掉
            int need = g[i--];
            int feed = s[j];
            if (feed >= need) {
                count++;
                --j;
            }
        }
        return count;
    }
};