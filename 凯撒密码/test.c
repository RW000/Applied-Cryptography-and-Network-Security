#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(char c[], int len, int k, char e[], FILE *fp) {
	int i = 0;
	for (i = 0; i < len; i++) {
		if (c[i] >= 'a' && c[i] <= 'z' - k) {
			e[i] = c[i] + k;
		} else if (c[i] >= 'a' && c[i] > 'z' - k) {
			e[i] = c[i] + k - 26;
		} else if (c[i] >= 'A' && c[i] <= 'Z' - k) {
			e[i] = c[i] + k;
		} else if (c[i] >= 'A' && c[i] > 'Z' - k) {
			e[i] = c[i] + k - 26;
		} else {
			e[i] = c[i];
		}
	}
	fputs(e, fp);
	printf("���ܳɹ���\n");
}

void decrypt(char c[], int len, int k, char d[], FILE *fp) {
	int i = 0;
	for (i = 0; i < len; i++) {
		if (c[i] >= 'a' + k && c[i] <= 'z') {
			d[i] = c[i] - k;
		} else if (c[i] >= 'a' && c[i] < 'a' + k) {
			d[i] = c[i] - k + 26;
		} else if (c[i] >= 'A' + k && c[i] <= 'Z') {
			d[i] = c[i] - k;
		} else if (c[i] >= 'A' && c[i] < 'A' + k) {
			d[i] = c[i] - k + 26;
		} else {
			d[i] = c[i];
		}
	}
	fputs(d, fp);
	printf("���ܳɹ���\n");
}

int main() {
	FILE *fpr, *fpw;
	int m = 0;
	do {
		printf("����:1 ����:2 ��������:0\n");
		scanf("%d", &m);
		if (m == 0)
			break;
		else if (m != 0 && m != 1 && m != 2)
			printf("�����������������룡\n");
		else {
			int len = 0, i = 0, k = 0;
			printf("��������Կk\n");
			scanf("%d", &k);
			if (m == 1) {
				char ch, c[10000] = {0}, e[10000] = {0};
				fpr = fopen("ԭ��.txt", "r");
				fpw = fopen("encrypt.txt", "w");
				if (fpr == NULL) {
					printf("open file error\n");
					return 1;
				}
				while (fscanf(fpr, "%c", &ch) != EOF) {
					c[i] = ch;
					i++;
					len++;
				}
				//printf("%s",c);
				encrypt(c, len, k, e, fpw);
				fclose(fpr);
				fclose(fpw);
			} else if (m == 2) {
				char ch, c[10000] = {0}, d[10000] = {0};
				fpr = fopen("encrypt.txt", "r");
				fpw = fopen("decrypt.txt", "w");
				if (fpr == NULL) {
					printf("open file error\n");
					return 1;
				}
				while (fscanf(fpr, "%c", &ch) != EOF) {
					c[i] = ch;
					i++;
					len++;
				}
				decrypt(c, len, k, d, fpw);
				fclose(fpr);
				fclose(fpw);
			}
		}
	} while (m != 0);
	return 0;
}
