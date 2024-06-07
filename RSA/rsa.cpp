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
	{'Y', 25}, {'Z', 26}
};

map<int, char> num_to_char = {
	{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'},
	{5, 'E'}, {6, 'F'}, {7, 'G'}, {8, 'H'},
	{9, 'I'}, {10, 'J'}, {11, 'K'}, {12, 'L'},
	{13, 'M'}, {14, 'N'}, {15, 'O'}, {16, 'P'},
	{17, 'Q'}, {18, 'R'}, {19, 'S'}, {20, 'T'},
	{21, 'U'}, {22, 'V'}, {23, 'W'}, {24, 'X'},
	{25, 'Y'}, {26, 'Z'}
};

// ����������ģ������
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

// ���ܺ���
void encrypt_string(const string &plaintext, vector<Elemtype> &encrypted, const PU &pu) {
	for (char ch : plaintext) {
		if (char_to_num.find(ch) != char_to_num.end()) {
			Elemtype num = char_to_num[ch];
			encrypted.push_back(modular_exponentiation(num, pu.e, pu.n));
		}
	}
}

// ���ܺ���
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
	// ��ȡ�û������p, q, e
	cout << "����������p��q: ";
	cin >> p >> q;
	n = p * q;
	fn = (p - 1) * (q - 1);

	cout << "�����빫Կָ��e: ";
	cin >> e;
	// ����˽Կָ��d
	// ����򻯴���ʵ��Ӧ������Ҫʹ����չŷ������㷨
	d = modular_exponentiation(e, fn - 1, fn);

	pu = {e, n};
	pr = {d, n};

	// �����Կ��˽Կ
	cout << "˽ԿPR.d: " << pr.d << "   PR.n: " << pr.n << endl;
	cout << "��ԿPU.e: " << pu.e << "   PU.n: " << pu.n << endl;

	// ��ȡ�����ļ�
	ifstream fin("����.txt");
	if (!fin.is_open()) {
		cerr << "�޷����ļ�����.txt" << endl;
		exit(1);
	}

	string plaintext;
	getline(fin, plaintext);
	fin.close();

	// ����
	vector<Elemtype> encrypted;
	encrypt_string(plaintext, encrypted, pu);

	// �����ܽ��д������.txt
	ofstream fout_encrypted("����.txt");
	if (!fout_encrypted.is_open()) {
		cerr << "�޷����ļ�����.txt" << endl;
		exit(1);
	}
	for (Elemtype num : encrypted) {
		fout_encrypted << num << " ";
	}
	fout_encrypted.close();
	cout << "���ܽ����д������.txt" << endl;

	// ����
	string decrypted = decrypt_string(encrypted, pr);

	// �����ܽ��д�����.txt
	ofstream fout_decrypted("����.txt");
	if (!fout_decrypted.is_open()) {
		cerr << "�޷����ļ�����.txt" << endl;
		exit(1);
	}
	fout_decrypted << decrypted;
	fout_decrypted.close();
	cout << "���ܽ����д�����.txt" << endl;


	ofstream fout_public("��Կ.txt");
	if (!fout_public.is_open()) {
		cerr << "�޷����ļ���Կ.txt" << endl;
		exit(1);
	}
	fout_public << "PU.e: " << pu.e << "\nPU.n: " << pu.n;
	fout_public.close();
	cout << "��Կ��д�빫Կ.txt" << endl;

	// ��˽Կд��˽Կ.txt
	ofstream fout_private("˽Կ.txt");
	if (!fout_private.is_open()) {
		cerr << "�޷����ļ�˽Կ.txt" << endl;
		exit(1);
	}
	fout_private << "PR.d: " << pr.d << "\nPR.n: " << pr.n;
	fout_private.close();
	cout << "˽Կ��д��˽Կ.txt" << endl;
	return 0;
}


