#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;
#define MAX 50000
int L[MAX], R[MAX], A[MAX], B[MAX], n, m;
struct query {
	int A, B, i, ans, index;
} Q[MAX];
bool cmp(query a, query b) {
	int blksz = (int)sqrt(n);
	if (a.A / blksz < b.A / blksz) return true;
	if (a.A / blksz == b.A / blksz) {
		return a.B < b.B;
	}
	return false;
}
struct node {
	int left, right;
	int max, index, update;  
} seg[200000];
void build(int i, int l, int r) {
	seg[i].left = l;
	seg[i].right = r;
	seg[i].index = l;
	if (l == r) {
		seg[i].max = seg[i].update = 0;
		return;
	} else {
		int mid = (l + r) >> 1;
		build(i << 1, l, mid);
		build(i << 1 | 1, mid + 1, r);
	}
}
void cover(int o, int rhs) {
	seg[o].update += rhs;
}
void pushup(int o) {
	int lc = o << 1, rc = o << 1 | 1;
	if (seg[lc].max + seg[lc].update >= seg[rc].max + seg[rc].update) {
		seg[o].max = seg[lc].max + seg[lc].update;
		seg[o].index = seg[lc].index;
	} else {
		seg[o].max = seg[rc].max + seg[rc].update;
		seg[o].index = seg[rc].index;
	}
}
void update(int o, int l, int r, int rhs) {
	//if (o == 1) printf("update [%d, %d]\n", l, r);
	if (l > seg[o].right || r < seg[o].left) {
		return;
	}
	if (l <= seg[o].left && r >= seg[o].right) {
		seg[o].update += rhs;
		return;
	}
	update(o << 1, l, r, rhs);
	update(o << 1 | 1, l, r, rhs);
	pushup(o);
}
int main() {
	//freopen("../../data/magic/magic.in", "r", stdin);
	//freopen("../../data/magic/magic.out", "w", stdout);
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d%d", &L[i], &R[i]);
	}
	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &Q[i].A, &Q[i].B);
		Q[i].i = i;
	}
	sort(Q, Q + m, cmp);
	build(1, 1, 50000);
	//printf("m=%d,n=%d\n", m, n);
	for (int i = 0; i < m; i++) {
		if (i == 0) {
			for (int j = Q[i].A - 1; j < Q[i].B; j++) {
				update(1, L[j], R[j], 1);
			}
		} else {
			int ln = Q[i].A - 1, rn = Q[i].B - 1, lo = Q[i - 1].A - 1, ro = Q[i - 1].B - 1;
			while (ln > lo) {update(1, L[lo], R[lo], -1); lo++;}
			while (ln < lo) {lo--;update(1, L[lo], R[lo], 1);}
			while (rn > ro) {ro++;update(1, L[ro], R[ro], 1);}
			while (rn < ro) {update(1, L[ro], R[ro], -1);ro--;}
		}
		Q[i].ans = seg[1].max + seg[1].update;
		Q[i].index = seg[1].index;
	}
	//for (int i = 0; i < 1; i++) printf("Q[%d]=[%d,%d,%d] [%d, %d]\n", i, Q[i].A, Q[i].B, Q[i].i, Q[i].ans, Q[i].index);
	int ans[m];
	int index[m];
	for (int i = 0; i < m; i++) {
		ans[Q[i].i] = Q[i].ans;
		index[Q[i].i] = Q[i].index;
	}
	for (int i = 0; i < m; i++) {
		printf("%d %d\n", index[i], ans[i]);
	}
} 
