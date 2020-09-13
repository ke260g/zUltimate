class Solution(object):
    def findMedianSortedArrays(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: float
        """
        idx1 = 0
        idx2 = 0
        i = 0
        result = [0] * (len(nums1) + len (nums2))
        while idx1 < len(nums1) and idx2 < len(nums2):
            if nums1[idx1] <= nums2[idx2]:
                result[i] = nums1[idx1]
                idx1 += 1
            else:
                result[i] = nums2[idx2]
                idx2 += 1
            i += 1
        if idx1 != len(nums1):
            for j in range(idx1, len(nums1)):
                result[i] = nums1[j]
                i += 1
        if idx2 != len(nums2):
            for j in range(idx2, len(nums2)):
                result[i] = nums2[j]
                i += 1
        if len(result) % 2 == 1:
            return result[(len(result) - 1) / 2]
        else:
            return (result[len(result) / 2] + result[len(result)/2 - 1]) / 2.0