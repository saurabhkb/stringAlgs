#include <stdio.h>
#include <string.h>

#define MAX	20

/*
*	zi = longest common prefix of S[i...] and S
	a	a	b	c	a	a	b	x	a	a	a
	0	1	0	0	3	1	0	0	2	2	1
*/

int naiveZ(char s[], int z[]) {
	int i = 0, j, zval;
	z[i++] = 0;
	for(; s[i]; i++) {
		zval = 0;
		for(j = i; s[j]; j++) {
			if(s[j] == s[j - i]) zval++;
			else break;
		}
		z[i] = zval;
	}
}

int fastZ(char s[], int z[]) {
	int i, j, k, l, maxReach, zval;
	z[0] = 0;
	j = 0;
	maxReach = 0 + z[0];
	for(i = 1; s[i]; i++) {
		if(maxReach > i) {
			k = i - j;
			if(z[k] > z[j] - k) {
				z[i] = z[j] - k;
			} else if(z[k] < z[j] - k) {
				z[i] = z[k];
			} else {
				// match uptil z[k]. just check beyond that
				zval = z[k];
				for(l = i + z[k]; s[l]; l++) {
					if(s[l] == s[l - i]) zval++;
					else break;
				}
				z[i] = zval;
				if(i + zval > maxReach) {
					j = i;
					maxReach = i + zval;
				}
			}
		} else {
			zval = 0;
			for(l = i; s[l]; l++) {
				if(s[l] == s[l - i]) zval++;
				else break;
			}
			z[i] = zval;
			if(i + zval > maxReach) {
				j = i;
				maxReach = i + zval;
			}
		}
	}
}

void print(int arr[], int n) {
	int i;
	for(i = 0; i < n; i++) printf("%d ", arr[i]);
	printf("\n");
}

int zMatch(char haystack[], char needle[]) {
	char container[2 * MAX] = "";
	strcat(container, needle);
	strcat(container, "$");
	strcat(container, haystack);

	int z[2 * MAX], i;
	fastZ(container, z);
	for(i = 0; container[i]; i++) {
		if(z[i] == strlen(needle)) return i;
	}
	return -1;
}

int main() {
//	char s[] = "abacabad";
//	char t[] = "abc";
//	printf("%d\n", zMatch(s, t));
	char s[] = "abababd";
	int z[MAX];
	naiveZ(s, z);
	print(z, 7);
}

