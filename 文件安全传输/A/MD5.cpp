#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//! ����MD5״̬���ݽṹ����
typedef struct {
	unsigned   int      state[4];		// ��ʼ���ӱ���;����16�ֽ�ժҪ
	unsigned   int      count[2];		// ����λ��(��64λ���棬count[0]��ʾ��32λ��count[1]��ʾ��32λ)
	unsigned char		PADDING[64];	// ���λ�����64*8λ
	unsigned char		buffer[64];		// ���뻺�壨�ݴ�512λ���ģ�
} MD5_State;


//! F, G, H and I ����MD5����
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

//! ��xѭ������nλ
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

//! 4��������FF(��1��), GG(��2��), HH(��3��), and II(��4��)ת��
void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned ac) {
	a += F ((b), (c), (d)) + (x) + (unsigned  int)(ac);
	a = ROTATE_LEFT ((a), (s));
	a += (b);
}

void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned ac) {
	a += G ((b), (c), (d)) + (x) + (unsigned  int)(ac);
	a = ROTATE_LEFT ((a), (s));
	a += (b);
}

void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned ac) {
	a += H ((b), (c), (d)) + (x) + (unsigned  int)(ac);
	a = ROTATE_LEFT ((a), (s));
	a += (b);
}

void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned ac) {
	a += I ((b), (c), (d)) + (x) + (unsigned  int)(ac);
	a = ROTATE_LEFT ((a), (s));
	a += (b);
}

/******************************************************************************/
//	���ƣ�Encode
//	���ܣ����������ֻ�(unsigned  long int -> unsigned char)(δ����ǰ���ĵĳ���ת��)
//  ������output:	ָ��unsigned char�������������
//		 input:	ָ��unsigned long int
/******************************************************************************/
void Encode( unsigned char *output, unsigned   int *input, unsigned int len ) {
	//unsigned   intΪ32λ����Ҫת���浽ָ��char������
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] =   (unsigned char)(input[i] & 0xff);        //���ȵĵ�8λ��һ���ֽڣ�
		output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff); //���ȵ��м�8λ
		output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff); //�θ�8λ
		output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff); //��8λ
	}
}


/******************************************************************************/
//���ܣ���������ת��(unsigned char -> unsigned  int)�����������ķ��飨512λ�ֳ�16�飬ÿ��32λ����
//������ output:	ָ��unsigned int�������뻺����
//		input:	ָ��unsigned char
/******************************************************************************/
void Decode( unsigned  int *output, unsigned char *input, unsigned int len ) {
	//�����ģ�char ���飩��ÿ32λΪһ��浽һ��32λ��unsigned  int�����У����潫������������
	unsigned int i, j;
	for ( i = 0, j = 0; j < len; i++, j += 4 ) {
		output[i] = ((unsigned  int)input[j]) | (((unsigned  int)input[j + 1]) << 8) |
		            (((unsigned  int)input[j + 2]) << 16) | (((unsigned  int)input[j + 3]) << 24);
	}
}

/******************************************************************************/
//	���ƣ�MD5Init
//	���ܣ���ʼ���ӱ�����ֵ����ʼ�����λ
//   ������ָ��MD5״̬���ݱ���
//	���أ���
//   ��ע�����λ��1λΪ1,����λΪ0

/******************************************************************************/
void MD5_Init( MD5_State *s ) {
	s->count[0] = s->count[1] = 0;
	//! ��ʼ���ӱ���
	s->state[0] = 0x67452301;
	s->state[1] = 0xefcdab89;
	s->state[2] = 0x98badcfe;
	s->state[3] = 0x10325476;

	//! ��ʼ���λ(Ŀ����ʽ: 0x80000000......������512λ)
	memset( s->PADDING, 0, sizeof(s->PADDING) );
	*(s->PADDING) = 0x80; //��һλΪ1������Ϊ0��1000 0000 0000 0000 0000 0000 ....��
	//  s->PADDING = {
	//	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	};
}

/******************************************************************************/
//	���ƣ�MD5Transform
//	���ܣ�MD5 4������
//  ������state: ���ӱ�����block: �����ķ���
//	���أ���
//  ��ע��4�ֹ���64������

/******************************************************************************/
void MD5_Transform( unsigned   int state[4], unsigned char block[64] ) {
	unsigned  int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode( x, block, 64 );  //��512λ���ķֳ�16��

	//! ��1��

	FF (a, b, c, d, x[0],  7,  0xd76aa478);  // 1
	FF (d, a, b, c, x[ 1], 12, 0xe8c7b756);  // 2
	FF (c, d, a, b, x[ 2], 17, 0x242070db);  // 3
	FF (b, c, d, a, x[ 3], 22, 0xc1bdceee);  // 4
	FF (a, b, c, d, x[ 4], 7,  0xf57c0faf);  // 5
	FF (d, a, b, c, x[ 5], 12, 0x4787c62a);  // 6
	FF (c, d, a, b, x[ 6], 17, 0xa8304613);  // 7
	FF (b, c, d, a, x[ 7], 22, 0xfd469501);  // 8
	FF (a, b, c, d, x[ 8], 7,  0x698098d8);  // 9
	FF (d, a, b, c, x[ 9], 12, 0x8b44f7af);  // 10
	FF (c, d, a, b, x[10], 17, 0xffff5bb1);  // 11
	FF (b, c, d, a, x[11], 22, 0x895cd7be);  // 12
	FF (a, b, c, d, x[12], 7,  0x6b901122);  // 13
	FF (d, a, b, c, x[13], 12, 0xfd987193);  // 14
	FF (c, d, a, b, x[14], 17, 0xa679438e);  // 15
	FF (b, c, d, a, x[15], 22, 0x49b40821);  // 16

	//! ��2��
	GG (a, b, c, d, x[ 1], 5,  0xf61e2562);  // 17
	GG (d, a, b, c, x[ 6], 9,  0xc040b340);  // 18
	GG (c, d, a, b, x[11], 14, 0x265e5a51);  // 19
	GG (b, c, d, a, x[ 0], 20, 0xe9b6c7aa);  // 20
	GG (a, b, c, d, x[ 5], 5,  0xd62f105d);  // 21
	GG (d, a, b, c, x[10], 9,  0x2441453);   // 22
	GG (c, d, a, b, x[15], 14, 0xd8a1e681);  // 23
	GG (b, c, d, a, x[ 4], 20, 0xe7d3fbc8);  // 24
	GG (a, b, c, d, x[ 9], 5,  0x21e1cde6);  // 25
	GG (d, a, b, c, x[14], 9,  0xc33707d6);  // 26
	GG (c, d, a, b, x[ 3], 14, 0xf4d50d87);  // 27
	GG (b, c, d, a, x[ 8], 20, 0x455a14ed);  // 28
	GG (a, b, c, d, x[13], 5,  0xa9e3e905);  // 29
	GG (d, a, b, c, x[ 2], 9,  0xfcefa3f8);  // 30
	GG (c, d, a, b, x[ 7], 14, 0x676f02d9);  // 31
	GG (b, c, d, a, x[12], 20, 0x8d2a4c8a);  // 32

	//! ��3��
	HH (a, b, c, d, x[ 5], 4,  0xfffa3942);  // 33
	HH (d, a, b, c, x[ 8], 11, 0x8771f681);  // 34
	HH (c, d, a, b, x[11], 16, 0x6d9d6122);  // 35
	HH (b, c, d, a, x[14], 23, 0xfde5380c);  // 36
	HH (a, b, c, d, x[ 1], 4,  0xa4beea44);  // 37
	HH (d, a, b, c, x[ 4], 11, 0x4bdecfa9);  // 38
	HH (c, d, a, b, x[ 7], 16, 0xf6bb4b60);  // 39
	HH (b, c, d, a, x[10], 23, 0xbebfbc70);  // 40
	HH (a, b, c, d, x[13], 4,  0x289b7ec6);  // 41
	HH (d, a, b, c, x[ 0], 11, 0xeaa127fa);  // 42
	HH (c, d, a, b, x[ 3], 16, 0xd4ef3085);  // 43
	HH (b, c, d, a, x[ 6], 23, 0x4881d05);   // 44
	HH (a, b, c, d, x[ 9], 4,  0xd9d4d039);  // 45
	HH (d, a, b, c, x[12], 11, 0xe6db99e5);  // 46
	HH (c, d, a, b, x[15], 16, 0x1fa27cf8);  // 47
	HH (b, c, d, a, x[ 2], 23, 0xc4ac5665);  // 48

	//! ��4��
	II (a, b, c, d, x[ 0], 6,  0xf4292244);  // 49
	II (d, a, b, c, x[ 7], 10, 0x432aff97);  // 50
	II (c, d, a, b, x[14], 15, 0xab9423a7);  // 51
	II (b, c, d, a, x[ 5], 21, 0xfc93a039);  // 52
	II (a, b, c, d, x[12], 6,  0x655b59c3);  // 53
	II (d, a, b, c, x[ 3], 10, 0x8f0ccc92);  // 54
	II (c, d, a, b, x[10], 15, 0xffeff47d);  // 55
	II (b, c, d, a, x[ 1], 21, 0x85845dd1);  // 56
	II (a, b, c, d, x[ 8], 6,  0x6fa87e4f);  // 57
	II (d, a, b, c, x[15], 10, 0xfe2ce6e0);  // 58
	II (c, d, a, b, x[ 6], 15, 0xa3014314);  // 59
	II (b, c, d, a, x[13], 21, 0x4e0811a1);  // 60
	II (a, b, c, d, x[ 4], 6,  0xf7537e82);  // 61
	II (d, a, b, c, x[11], 10, 0xbd3af235);  // 62
	II (c, d, a, b, x[ 2], 15, 0x2ad7d2bb);  // 63
	II (b, c, d, a, x[ 9], 21, 0xeb86d391);  // 64

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	memset( (unsigned char *)x, 0, sizeof (x) );
}

/******************************************************************************/
//	���ƣ�MD5_Update
//	���ܣ�������䣬���ķ��飬16�������ķ���
//  ������ָ��SHA״̬����
//	���أ���

/******************************************************************************/
void MD5_Update( MD5_State *s, unsigned char *input, unsigned int inputLen ) {
	unsigned int i, index, partLen;

	//! �������

	//! �ֽ��� mod 64����ǰӵ�е������ֽ�����
	index = (unsigned int)((s->count[0] >> 3) & 0x3F);//һ���ַ�һ���ֽ�ռ8λ��count��������λ����������Ҫת��Ϊ�ֽ�����
	//index = (unsigned int)((s->count[0] >> 3)%64);

	//! ����λ��
	if ((unsigned long int)(s->count[0] += ((unsigned long int)inputLen << 3)) < ((unsigned long int)inputLen <<
	        3)) { //inputLen���ֽ�����ת��Ϊλ���Ƚ�
		//�������������Ҫ��λ(�޷�������Maxֵ+1==0)
		s->count[1]++; //��λ
	}
	s->count[1] += ((unsigned  int)inputLen >> 29);    //����ĸ�λ�ֽ��� inputlen*8 >> 32
	partLen = 64 - index; //����������ֽ���

	//! MD5 4������
	if (inputLen >= partLen) { //��������ַ����������ṩ������ַ����ȣ������ִ��
		memcpy( (unsigned char *)&s->buffer[index],  (unsigned char *)input, partLen ); //����������Ŀ������������ȴ�����
		MD5_Transform( s->state, s->buffer );   //����

		for ( i = partLen; i + 63 < inputLen; i += 64 ) //�����Ƿ������һ��64�ֽڵ�����
			MD5_Transform( s->state, &input[i] );      //�����������

		index = 0;
	} else
		i = 0;

	memcpy ((unsigned char *)&s->buffer[index], (unsigned char *)&input[i],
	        inputLen - i); //�Բ���64�ֽڵ������ַ������ȿ����������ȴ�����������512λ����
}

/******************************************************************************/
//	���ƣ�MD5_Final
//	���ܣ�MD5���任
//  ������strContent:ָ���ļ����ݻ�����; iLength:�ļ����ݳ���; output:ժҪ���������
//	���أ���

/******************************************************************************/
void MD5_Final( MD5_State *s, unsigned char digest[16] ) {
	unsigned char bits[8];      //��¼δ����ǰ�����ĵĳ���
	unsigned int index, padLen;

	Encode (bits, s->count, 8); //��δ����ǰ�����ĳ���ת����bits������

	//! ����С��448λ(mod 512��64���ֽڣ�),�����Ľ������(448λΪ56���ֽ�)
	index = (unsigned int)((s->count[0] >> 3) & 0x3f);   //�������е�λ��
	padLen = (index < 56) ? (56 - index) : (120 - index);//����˵����5��(���㻹���λ�� 64-index+56=120-index)
	MD5_Update( s, s->PADDING, padLen );                 //��仹��λ��������

	MD5_Update( s, bits, 8);         //����δ����ǰ�����ĵĳ���
	Encode( digest, s->state, 16 );  //��MD5ժҪת���������������

	//��ʼ�����ʼ��״̬��������һ������
	memset ((unsigned char *)s, 0, sizeof (*s));
	MD5_Init( s );
}


/******************************************************************************/
//	���ƣ�SHA_digest
//	���ܣ������ļ�ժҪ
//  ������strContent:ָ���ļ����ݻ�����; iLength:�ļ����ݳ���; output:ժҪ���������
//	���أ���

/******************************************************************************/
void md5_digest( void const *strContent, unsigned int iLength, unsigned char output[16] ) {
	unsigned char *q = (unsigned char *)strContent;
	MD5_State s;
	MD5_Init( &s );               //��ʼ��MD5״̬���ݽṹ
	MD5_Update( &s, q, iLength ); //����64*n���ֽڵ����ģ���ÿ512λ�����Ľ��д����п���û��64���ֽڣ�
	MD5_Final( &s, output );      //���������Ҫ��������
}




int main( int argc, char **argv ) {
	unsigned char buff[16];
	char str[100];
	FILE *fp_in, *fp_out;

	//��ԭ��.txt�ļ�
	fp_in = fopen("����.txt", "r");
	if (fp_in == NULL) {
		printf("�޷�������.txt�ļ�\n");
		return 1;
	}

	//������.txt�ļ�
	fp_out = fopen("�ļ�ժҪ.txt", "w");
	if (fp_out == NULL) {
		printf("�޷����ļ�ժҪ.txt�ļ�\n");
		return 1;
	}

	while (fgets(str, sizeof(str), fp_in) != NULL) {
		md5_digest(str, (unsigned int)strlen(str), buff);


		//MD5ժҪ��128λ����16���Ƶ���ʽ���
		for (int j = 0; j < 16; j++) {
			fprintf(fp_out, "%x", (buff[j] & 0xF0) >> 4);
			fprintf(fp_out, "%x", buff[j] & 0x0F);
		}
		fprintf(fp_out, "\n\n");
	}

	fclose(fp_in);
	fclose(fp_out);

	return 0;
}


//http://www.cmd5.com�ϲ��Ե����ݽ�����֤
/*
  md5(a,32) = 0cc175b9c0f1b6a831c399e269772661
  md5(ab,32) = 187ef4436122d1cc2f40dc2b92f0eba0
  md5(md5,32) = 1bc29b36f623ba82aaf6724fd3b16718
  md5(hello word!,32) = 39505368546302be2704b3d53b24203c
*/