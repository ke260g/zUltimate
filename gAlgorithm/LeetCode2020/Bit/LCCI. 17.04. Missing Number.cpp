// An array contains all the integers from 0 to n, 
// except for one number which is missing.  
// Write code to find the missing integer. Can you do it in O(n) time?

// Note: This problem is slightly different from the original one the book.

class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int ans = 0;
        for (int i = 0; i < nums.size(); ++i)
            ans ^= (i+1)^nums[i];
        return ans;
    }
};