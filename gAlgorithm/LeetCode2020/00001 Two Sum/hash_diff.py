class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        h = { } # key = num, value = idx

        for idx, num in enumerate(nums):
            diff = target - num
            if h.get(diff) == None:
                h[num] = idx
            else:
                return [h[diff], idx]