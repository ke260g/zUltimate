class Solution {
public:
    int convertInteger(int A, int B) {
        unsigned n = A^B;
        int count = 0;
        while (n) {
            n &= n - 1;
            count++;
        }
        return count;
    }
};