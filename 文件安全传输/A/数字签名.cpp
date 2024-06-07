#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>

using namespace std;

typedef int Elemtype;
Elemtype p, q, e, d, n, fn;
Elemtype m, c;
int flag = 0;

struct PU {
	Elemtype e;
	Elemtype n;
} pu;

struct PR {
	Elemtype d;
	Elemtype n;
} pr;

map<char, int> char_to_num = {
	{'A', 1}, {'B', 2}, {'C', 3}, {'D', 4},
	{'E', 5}, {'F', 6}, {'G', 7}, {'H', 8},
	{'I', 9}, {'J', 10}, {'K', 11}, {'L', 12},
	{'M', 13}, {'N', 14}, {'O', 15}, {'P', 16},
	{'Q', 17}, {'R', 18}, {'S', 19}, {'T', 20},
	{'U', 21}, {'V', 22}, {'W', 23}, {'X', 24},
	{'Y', 25}, {'Z', 26}, {'0', 27}, {'1', 28}, {'2', 29}, {'3', 30},
	{'4', 31}, {'5', 32}, {'6', 33}, {'7', 34},
	{'8', 35}, {'9', 36}, {'a', 37}, {'b', 38}, {'c', 39}, {'d', 40},
	{'e', 41}, {'f', 42}
};

map<int, char> num_to_char = {
	{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'},
	{5, 'E'}, {6, 'F'}, {7, 'G'}, {8, 'H'},
	{9, 'I'}, {10, 'J'}, {11, 'K'}, {12, 'L'},
	{13, 'M'}, {14, 'N'}, {15, 'O'}, {16, 'P'},
	{17, 'Q'}, {18, 'R'}, {19, 'S'}, {20, 'T'},
	{21, 'U'}, {22, 'V'}, {23, 'W'}, {24, 'X'},
	{25, 'Y'}, {26, 'Z'}, {27, '0'}, {28, '1'}, {29, '2'}, {30, '3'},
	{31, '4'}, {32, '5'}, {33, '6'}, {34, '7'},
	{35, '8'}, {36, '9'}, {37, 'a'}, {38, 'b'}, {39, 'c'}, {40, 'd'},
	{41, 'e'}, {42, 'f'}
};

// 辅助函数：模幂运算
Elemtype modular_exponentiation(Elemtype base, Elemtype exponent, Elemtype modulus) {
	Elemtype result = 1;
	while (exponent > 0) {
		if (exponent % 2 == 1) {
			result = (result * base) % modulus;
		}
		exponent = exponent >> 1;
		base = (base * base) % modulus;
	}
	return result;
}

// 加密函数
void encrypt_string(const string &plaintext, vector<Elemtype> &encrypted, const PU &pu) {
	for (char ch : plaintext) {
		if (char_to_num.find(ch) != char_to_num.end()) {
			Elemtype num = char_to_num[ch];
			encrypted.push_back(modular_exponentiation(num, pu.e, pu.n));
		}
	}
}

// 解密函数
string decrypt_string(const vector<Elemtype> &encrypted, const PR &pr) {
	string decrypted;
	for (Elemtype num : encrypted) {
		Elemtype decrypted_num = modular_exponentiation(num, pr.d, pr.n);
		if (num_to_char.find(decrypted_num) != num_to_char.end()) {
			decrypted.push_back(num_to_char[decrypted_num]);
		}
	}
	return decrypted;
}

int main() {
	int d = 23, e = 7, n = 187;

	pu = {e, n};
	pr = {d, n};

	// 输出公钥和私钥
	cout << "私钥PR.d: " << pr.d << "   PR.n: " << pr.n << endl;
	cout << "公钥PU.e: " << pu.e << "   PU.n: " << pu.n << endl;

	// 读取明文文件
	ifstream fin("文件摘要.txt");
	if (!fin.is_open()) {
		cerr << "无法打开文件文件摘要.txt" << endl;
		exit(1);
	}

	string plaintext;
	getline(fin, plaintext);
	fin.close();

	// 加密
	vector<Elemtype> encrypted;
	encrypt_string(plaintext, encrypted, pu);



	// 将加密结果写入密文.txt
	ofstream fout_encrypted("数字签名.txt");
	if (!fout_encrypted.is_open()) {
		cerr << "无法打开文件数字签名.txt" << endl;
		exit(1);
	}
	for (Elemtype num : encrypted) {
		fout_encrypted << num << " ";
	}
	fout_encrypted.close();
	cout << "加密结果已写入数字签名.txt" << endl;





}


