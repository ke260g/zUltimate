// 以list 和 vector 举例; 适用于几乎所有容器
construtor_and_initializor() {
    vector<int> vector_ten_elememnts(10); // 10个未初始化的成员 
    vector<int> vector_minus_one(10, -1); // 10个 -1 成员 
    vector<int> vector_zero(10, -1);      // 10个  0 成员
    vector<vector<int>> matrix(column_count, vector<int>(row_count));    //  matrix[y][x];
    vector<vector<int>> cube(Z, vector<vector<int>>(Y, vector<int>(X))); // cube[z][y][x];
    // 复制一个vector
    vector<int> vector_dup(vector_zero);
    // 复制一个范围的vector 通过iterator(全类型) 指定范围; 左闭右开
    vector<int> vector_dup_range(iterator_begin+i, iterator_end+j);
    // 复制一个范围的vector 数组指定; 左闭右开
    vector<int> vector_dup_array(array+i, array+j);

    list<int> list_ten_elememnts(10); // 10个未初始化的成员 
    list<int> list_minus_one(10, -1); // 10个 -1 成员 
    list<int> list_zero(10, -1);      // 10个  0 成员
    list<list<int>> list_2d(Y, list<int>(X)); // 二维单链
    // 复制一个list
    list<int> list_dup(list_zero);
    // 复制一个范围的list 通过iterator(全类型) 指定范围; 左闭右开
    list<int> list_dup_range(iterator_begin+i, iterator_end+j);
    // 复制一个范围的list 数组指定; 左闭右开
    list<int> list_dup_array(array+i, array+j);
}

// 1. 支持 list/ vector / deque / string/
// 2. 不支持 stack / queue / set / map
assign() {
    // assign 重新初始化；
    vector<int> vec;
    vec.assign(10, -1); // 初始化 10个 -1成员
    vec.assign(array+i, array+j); // 初始化; 值由数组确定; 左闭右开
    vec.assign(iteractor+i, iterator+j); // 左开右闭 区间

    list<int> list;
    list.assign(10, -1); // 初始化 10个 -1成员
    list.assign(array+i, array+j); // 初始化; 值由数组确定; 左闭右开
    list.assign(iteractor+i, iterator+j); // 左开右闭 区间
}

// 1 支持 list / vector /  deque / set / map
// 2 不支持 stack / queue
insert_and_erase() {
    // vector 的 insert 和 erase 都是相当耗时的操作
    vec.insert(iterator, 1000); // 新成员1000插入到 iterator 前
    vec.insert(iterator, 5, 100); // 5个新成员100插入到 iterator 前
    vec.insert(iterator, obj_iterator+i, obj_iterator+j); // vec2 左开右闭区间插入到 vec的iterator前
    vec.insert(iterator, array+i, array+j); // 数组 左开右闭区间 插入到 vec的iteractor前
    vec.erase(iteractor+i, iterator+j); // 左开右闭区间 的元素删除
    vec.erase(iteractor);               // 单个元素删除

    // list 的 insert 和 erase 接近O(k), 待操作的元素
    l1.insert(iterator, 1000); // 新成员1000插入到 iterator 前
    v1.insert(iterator, 5, 100); // 5个新成员100插入到 iterator 前
    l1.insert(iterator, obj_iterator+i, obj_iterator+j); // vec2 左开右闭区间插入到 vec的iterator前
    l1.insert(iterator, array+i, array+j); // 数组 左开右闭区间 插入到 vec的iteractor前
    l1.erase(iteractor+i, iterator+j); // 左开右闭区间 的元素删除
    l1.erase(iteractor);               // 单个元素删除
}

