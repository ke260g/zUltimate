# {} 记录上一次出现的字符索引
# 记录 head, 子字符串的开始
# 记录 result, 最长的字符串长度
# 遍历字符串
#   if 每次遇到重复字符时, 当上一次出现字符的索引 > head
#      则 head 记录为 上一次出现的字符索引的 + 1 表示新的字符串开始
#   else 求当前索引 与 开始索引的差 + 1得当前子字符串长度
#        比较 且 更新最长的字符串长度
#   更新当前字符的索引到{}中
class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        result_size = 0
        temped_head = 0
        remembered = { }
        for i in range(len(s)):
            if s[i] in remembered and remembered[s[i]] >= temped_head:
                temped_head = remembered[s[i]] + 1
            else:
                result_size = max(result_size, i - temped_head + 1)
            remembered[s[i]] = i
        return result_size
                
            