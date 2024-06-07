#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encrypt(char *k) {
	FILE *fp1 = fopen("Ô­ÎÄ.txt", "r");
	FILE *fp2 = fopen("encrypt.txt", "w");
	if (fp1 == NULL) {
		printf("error!\n");
		exit;
	}
	int i = 0, len = 0;
	char a[10000], ch;
	while (fscanf(fp1, "%c", &ch) != EOF) {
		if (ch >= 'a' && ch <= 'z')
			a[i] = toupper(ch);
		else
			a[i] = ch;
		i++;
		len++;
	}
	// printf("%d\n",len);
	// printf("%s\n",a);
	for (int i = 0; i < len; i++) {
		if (a[i] >= 'A' && a[i] <= 'Z')
			a[i] = k[a[i] - 'A'];
	}
	// printf("%s\n",a);
	fputs(a, fp2);
	fclose(fp1);
	fclose(fp2);
}


int main() {
	FILE *fp1, *fp2;
	char *k, *c, *s;
	k = (char *)malloc(sizeof(char) * 26);
	c = (char *)malloc(sizeof(char) * 100);
	s = (char *)malloc(sizeof(char) * 100);
	printf("ÇëÊäÈëÃÜÔ¿£º\n");
	scanf("%[^\n]", c);
	int len = strlen(c);
	printf("%d\n", len);
	int appeared[26] = {0};
	int j = 0;
	for (int i = 0; i < len; i++) {
		if (isupper(c[i]) && !appeared[c[i] - 'A']) {
			s[j++] = c[i];
			appeared[c[i] - 'A'] = 1;
		} else if (islower(c[i]) && !appeared[c[i] - 'a']) {
			s[j++] = toupper(c[i]);
			appeared[c[i] - 'a'] = 1;
		}
	}
	s[j] = '\0';
	printf("%s\n", s);
	len = strlen(s);
	printf("%d\n", len);
	j = 0;
	for (int i = 0; i < len; i++) {
		k[j++] = s[i];
	}
	for (int i = 0; i < 26; i++) {
		if (!appeared[i]) {
			k[j++] = 'A' + i;
		}
	}
	k[j] = '\0';
	printf("%s\n", k);
	encrypt(k);

	free(c);
	free(s);
	free(k);


	return 0;
}

