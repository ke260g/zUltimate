void sort_bubble(vector<int> &a) {
    for (int i = a.size(); i > 0; --i) {
        for (int j = 1; j < i; ++j) {
            if (a[j] < a[j-1])
                swap(a[j], a[j-1]);
        }
    }
}
