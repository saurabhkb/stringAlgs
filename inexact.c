#include <stdio.h>

#define MAX	100

int min(int a, int b) {
	return a > b ? b : a;
}

int min3(int a, int b, int c) {
	return min(a, min(b, c));
}

int min4(int a, int b, int c, int d) {
	return min(min(a, b), min(c, d));
}

int max(int a, int b) {
	return a > b ? a : b;
}

int max4(int a, int b, int c, int d) {
	return max(max(a, b), max(c, d));
}

int edit_distance(char s[], char t[]) {
	int i, j, slen, tlen;
	int E[MAX][MAX];

#define RIGHT	0
#define DOWN	1
#define DIAG	2

	int how[MAX][MAX];

	for(slen = 0; s[slen]; slen++);
	for(tlen = 0; t[tlen]; tlen++);

	for(i = 0; i < slen; i++)
		E[i][0] = i;
	for(i = 0; i < tlen; i++)
		E[0][i] = i;

	/* convert s to t */
	for(i = 1; s[i]; i++) {
		for(j = 1; t[j]; j++) {
			/* convert s[1...i] -> t[1...j] by deleting one character from s (1) and converting s[1...i-1] -> t[1...j] (E[i - 1][j]) */
			/* convert s[1...i] -> t[1...j] by converting s[1...i] -> t[1...j-1] (E[i][j - 1]) and inserting one character into t (1) */
			/* if s[i] == t[j], no edit cost */
			/* if s[i] != t[j], convert s[1...i] -> t[1...j] by converting s[1...i-1] to t[1...j-1] and substituting s[i] with t[j] */
			E[i][j] = min3(
				E[i - 1][j] + 1,
				E[i][j - 1] + 1,
				E[i - 1][j - 1] + (s[i] == t[j] ? 0 : 1)
			);

			E[i][j] = E[i - 1][j - 1] + (s[i] == t[j] ? 0 : 1);
			how[i][j] = DIAG;
			if(E[i - 1][j] + 1 < E[i][j]) {
				E[i][j] = E[i - 1][j] + 1;
				how[i][j] = DOWN;
			}
			if(E[i][j - 1] + 1 < E[i][j]) {
				E[i][j] = E[i][j - 1] + 1;
				how[i][j] = RIGHT;
			}
		}
	}
	for(i = 0; s[i]; i++) {
		for(j = 0; t[j]; j++) {
			printf("%d ", E[i][j]);
		}
		printf("\n");
	}
	i = slen - 1;
	j = tlen - 1;
	while(1) {
		switch(how[i][j]) {
			case DIAG: {
				printf("sub %c, %c\n", s[i], t[j]);
				i--;
				j--;
				break;
			}
			case DOWN: {
				printf("ins %c\n", t[j]);
				i--;
				break;
			}
			case RIGHT: {
				printf("del %c\n", s[i]);
				j--;
				break;
			}
		}
		if(i == 0 && j == 0) break;
	}
	return E[slen - 1][tlen - 1];
}

/*
SMITH-WATERMAN ALGORITHM
substring from each with best match
here, we are maximizing the scoring function (not minimizing the distance)
*/
int local_alignment(char s[], char t[]) {
	int i, j;
	int E[MAX][MAX];


	int how[MAX][MAX];

	for(i = 0; s[i]; i++)
		E[i][0] = 0;
	for(i = 0; t[i]; i++)
		E[0][i] = 0;

	for(i = 1; s[i]; i++) {
		for(j = 1; t[j]; j++) {
			E[i][j] = 0;
			E[i][j] = max4(
				/* we don't want to pay a penalty on a mismatch, since we are ok with terminating the string (since we are looking for substrings) */
				0,
				E[i - 1][j] - 1,
				E[i][j - 1] - 1,
				E[i - 1][j - 1] + (s[i] == t[j] ? 1 : -1)
			);
		}
	}

	for(i = 0; s[i]; i++) {
		for(j = 0; t[j]; j++) {
			printf("%d ", E[i][j]);
		}
		printf("\n");
	}

	int max = -1;
	for(i = 0; s[i]; i++) {
		for(j = 0; t[j]; j++) {
			max = (max > E[i][j]) ? max : E[i][j];
		}
	}
	return max;
}


#define INF	100000
int num_coins(int denom[], int n, int v) {
	/* c[i] => min num of coins to get value i. Therefore, we find c[v] */
	int c[MAX], i, j;
	for(i = 0; i <= v; i++) c[i] = INF;
	c[0] = 0;
	for(i = 1; i <= v; i++) {
		for(j = 0; j < n; j++) {
			/* try to add each denomination coin to an existing best score (we would do best, if we at most increment by 1) */
			if(i >= denom[j] && c[i - denom[j]] != INF) {
				c[i] = min(c[i], c[i - denom[j]] + 1);
			}
		}
	}
	return c[v];
}


int longest_common_subsequence(char s[], char t[]) {
	int i, j, slen, tlen;
	int lcs[MAX][MAX];	/* lcs[i][j] = lcs of s[0...i] & t[0...j] */
	for(slen = 0; s[slen]; slen++) lcs[slen][0] = 0;
	for(tlen = 0; t[tlen]; tlen++) lcs[0][tlen] = 0;

	for(i = 0; s[i]; i++) {
		for(j = 0; t[j]; j++) {
			/* lcs[i][j] = max(lcs[i - 1][j - 1] + 1 (if s[i] == t[j]), lcs[i - 1][j], lcs[i][j - 1]) */
			if(s[i] == t[j]) lcs[i][j] = 1 + lcs[i - 1][j - 1];
			else {
				lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);
			}
		}
	}
	return lcs[slen - 1][tlen - 1];
}

int main() {
	char s[] = "hello";
	char t[] = "relxx";
	printf("%d\n", edit_distance(s, t));
	return 0;
}
