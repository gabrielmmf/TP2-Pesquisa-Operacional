#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

vector<vector<int>> g;
vector<vector<int>> edgs;
vector<vector<int>> edg_num;
vector<int> c;

vector<int> match;
vector<int> y;

int main(int argc, char **argv)
{
    ifstream in(argv[1]); // Input
	
	int n, m;
	in >> n >> m;

	c.resize(m);
	edgs.resize(m);
	g.resize(2*n);
	edg_num.resize(2*n,vector<int>(2*n,-1));
	y.resize(2*n);

	for (int i = 0; i < 2*n; i++)
		for (int j = 0; j < m; j++) {
			int v;
			in >> v;
			if(v == 1) edgs[j].push_back(i);
		}
	
	for(int i = 0; i < m; i++){
		g[edgs[i][0]].emplace_back(edgs[i][1]);
		g[edgs[i][1]].emplace_back(edgs[i][0]);
		edg_num[edgs[i][0]][edgs[i][1]] = i;
	}

	for(int i = 0; i < m; i++)
		in >> c[i];

    in.close();
	ifstream out(argv[2]); // output

	int otim;
	out >> otim;
	if (otim == -1) {
		vector<bool> vizs(n,false);
		vector<bool> confirms(n,true);
		int tota = 0, totb = 0;
		for (int i = 0; i < n; i++) {
			int val;
			out >> val;
			tota += val;
			if (val)
				for(int j : g[i]) {
					vizs[j-n] = true;
					confirms[j-n] = false;
				}
		}
		for (int i = 0; i < n; i++) {
			int val;
			out >> val;
			totb += val;
			if (val) {
				assert(vizs[i]);// realmente sao os vizinhos
				confirms[i] = true;
			}
		}
		for(bool b: confirms)
			assert(b); // realmente sao todos os vizinhos
		assert(tota > totb); // realmente S > N(S)
	} else {
		int empsum = 0;
		vector<int> count(2*n,0);
		for (int i = 0; i < m; i++) {
			int v;
			out >> v;
			if(v == 1) {
				empsum += c[i];
				count[edgs[i][0]]++;
				count[edgs[i][1]]++;
				match.push_back(i);
			}
		}
		// realmente eh um emparelhamento perfeito
		for(int i = 0; i < 2*n; i++)
			assert(count[i] == 1);
		// emparelhamento realmente vale o afirmado
		assert(empsum == otim);
		int ysum = 0;
		for (int i = 0; i < 2*n; i++) {
			double db;
			out >> db;
			int val = db*2;
			y[i] = val;
			ysum += val;
		}
		// otimo da dual eh multiplo de 0.5
		assert(ysum%2 == 0);
		// otimo da dual eh o otimo declarado
		assert(ysum/2 == otim);
		for (int i = 0; i < m; i++) {
			// restricao da dual
			assert(y[edgs[i][0]] + y[edgs[i][1]] <= c[i]*2);
		}
	}

    out.close();
}
