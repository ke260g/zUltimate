void sort_select(vector<int> &a) {
    for (int i = 0; i < a.size(); ++i) {
        int j = i;
        int mmin = j;
        for (j++; j < a.size(); ++j)
            if (a[j] < a[mmin])
                mmin = j;
        swap(a[i], a[mmin]);
    }
}
