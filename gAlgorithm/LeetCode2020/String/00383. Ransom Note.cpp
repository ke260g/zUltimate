// Given an arbitrary ransom note string and another string containing letters from all the magazines,
// write a function that will return true if the ransom note can be constructed from the magazines ; 
// otherwise, it will return false.

// Each letter in the magazine string can only be used once in your ransom note.

// Input: ransomNote = "a", magazine = "b"
// Output: false

// Input: ransomNote = "aa", magazine = "ab"
// Output: false

// Input: ransomNote = "aa", magazine = "aab"
// Output: true

// 判断 magazine 中的字符能否组成 ransomNote 这个字符串


// 自己的做法: 双40%
// 1. 遍历 ransomNote; 构造 ransomHash, 记录每个子符需要的个数
// 2. 遍历 magazine; 在 ransomHash 中找到相应字符, 然后 --
//    说明可以抵消掉一个
//    如果 ransomHash[c] == 0; 则从 ransomHash 中干掉
// 3. 最后; 如果 ransomHash 是 empty(); 说明满足条件
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        map<char, int> ransomHash;
        for (auto c: ransomNote) {
            if (ransomHash.find(c) != ransomHash.end())
                ransomHash[c]++;
            else
                ransomHash[c]=1;
        }
        for (auto c: magazine) {
            if (ransomHash.find(c) != ransomHash.end()) {
                ransomHash[c]--;
                if (ransomHash[c] == 0)
                    ransomHash.erase(c);
            }
            if (ransomHash.empty())
                break;
        }
        return ransomHash.empty();
    }
};

// 别人的; 60%, 6%
// 1. 分别计算 ransomNote 和 magazine 的 26个字母计数
// 2. 遍历26个字母;
// 3. 如果 ransomNote 字母计数 > magazine字母计数
//    返回 false
// 4. 最后 返回 true
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        vector<int> rTable(26, 0), mTable(26, 0); 
        for (auto c: ransomNote)
            rTable[c-'a']++;
        for (auto c: magazine)
            mTable[c-'a']++;
        for (int i = 0; i < 26; ++i) {
            if (rTable[i] > mTable[i])
                return false;
        }
        return true;
    }
};