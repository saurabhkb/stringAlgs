#include <stdio.h>
#include <string.h>

#define MAX	20

/*

P:		a b a c a b a d
Z:		0 0 1 0 3 0 1 0
SP:		0 0 1 0 1 2 3 0
SP':	0 0 1 0 0 0 3 0
*/

//extern fastZ(char [], int []);
//extern print(int [], int);

int max(int a, int b) {
	return (a > b) ? a : b;
}

void spFromZ(int z[], int n, int sp[], int spdash[]) {
	int i, j, k;
	for(i = 0; i < n; i++)
		spdash[i] = 0;

	for(i = n - 1; i > 0; i--) {
		spdash[i + z[i] - 1] = z[i];
	}

	sp[n - 1] = spdash[n - 1];
	for(i = n - 2; i > 0; i--) {
		sp[i] = max(sp[i + 1] - 1, spdash[i]);
	}
}

void spRecur(char str[], int sp[]) {
	int i, t;
	sp[0] = 0;
	for(i = 0; i < strlen(str) - 1; i++) {
		sp[i + 1] = -1;
		t = i;
		while(t >= 0) {
			if(str[sp[t] + 1 - 1] == str[i + 1]) {
				sp[i + 1] = sp[t] + 1;
				break;
			}
			t = sp[t] - 1;
		}
		if(sp[i + 1] == -1) {
			sp[i + 1] = (str[i + 1] == str[0]) ? 1 : 0;
		}
	}
}

int kmpMatch(char haystack[], char needle[]) {
	int sp[MAX], start = 0, j, match;
	spRecur(needle, sp);

	while(start <= strlen(haystack) - strlen(needle)) {
		match = 1;
		for(j = 0; needle[j]; j++) {
			if(needle[j] != haystack[start + j]) {
				start += j - sp[j] + 1;
				match = 0;
				break;
			}
		}
		if(match) return start;
	}
	return -1;
}

int main() {
	//char s[] = "abacabad";
	char s[] = "abacaba";
	int n = strlen(s);
	int z[MAX], sp[MAX], spdash[MAX];
	fastZ(s, z);
	print(z, n);
	spFromZ(z, n, sp, spdash);
	print(sp, n);
	spRecur(s, sp);
	print(sp, n);

	printf("%d\n", kmpMatch(s, "caba"));
	return 0;
}

