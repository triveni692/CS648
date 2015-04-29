#include <bits/stdc++.h>

using namespace std;

class SimpleHash
{
public:
	SimpleHash(){}
	SimpleHash(int a[], int nn){
		n = nn;
		table.resize(n);
		for (int i = 0; i < n; ++i){
			table[a[i] % n].push_back(a[i]);
		}
	}
	~SimpleHash(){
		for (int i = 0; i < n; ++i){
			table[i].clear();
		}
		table.clear();
	}
	void insert(int x){
		table[x % n].push_back(x);
	}
	bool find(int x){
		return count(table[x%n].begin(), table[x%n].end(), x);
	}
private:
	int n;
	vector<vector<int> > table;
};

int main()
{
	int n, A[10000];
	cin >> n;
	for (int i = 0; i < n; ++i){
		cin >> A[i];
	}
	int q;
	SimpleHash sh(A, n);
	cin >> q;
	while(q--){
		int x;
		cin >> x;
		cout << sh.find(x) << endl;
	}
	return 0;
}