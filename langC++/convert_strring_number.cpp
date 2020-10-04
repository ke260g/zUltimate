// 字符串转数值
/**
 * @str: 需要转换地字符串
 * @idx: 传出整数字符串后的首个不参与构成数值的字符的索引 
 * @base: 进制; 一般用 10 16 2 设置为0自动检测
 */
int stoi(const string&  str, size_t* idx = 0, int base = 10);
float stof(const string&  str, size_t* idx = 0, int base = 10);
long stol(const string&  str, size_t* idx = 0, int base = 10);
long long stoll(const string&  str, size_t* idx = 0, int base = 10);
unsigned long stoul(const string&  str, size_t* idx = 0, int base = 10);
unsigned long long stoull(const string&  str, size_t* idx = 0, int base = 10);
double stod(const string&  str, size_t* idx = 0, int base = 10);
long double stold(const string&  str, size_t* idx = 0, int base = 10);

// 数值转字符串
string to_string(int val);
string to_string(float val);
string to_string(long val);
string to_string(long long val);
string to_string(unsigned val);
string to_string(unsigned long val);
string to_string(unsigned long long val);
string to_string(double val);
string to_string(long double val);