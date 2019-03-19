class segTreeC { //special segment tree for counting
	private:
		int m;
		int *n;
	public:
		segTreeC(int mm): m(mm) {
			n = new int[m + 1];
			for(int i = 0; i <= m; i++) n[i] = 0;
		}
		~segTreeC() {
			delete[] n;
		}
		void clr(){
			for(int i = 0; i <= m; i++) n[i] = 0;
		}
		void inc(int p) {
			n[++p]++;
			for(int t = 1; p < m; t <<= 1)
				if(p & t) n[p += t]++;
		}
		void dec(int p) {
			n[++p]--;
			for(int t = 1; p < m; t <<= 1)
				if(p & t) n[p += t]--;
		}
		int sum(int p) {
			int s = 0;
			for(int t = 0; p > 0; t++, p >>= 1)
				if(p & 1) s += n[p << t];
			return s;
		}
		int mid(int s){
			int l = 0, r = 256;
			while(l < r - 1){
				if(sum((l + r) / 2) < s) l = (l + r) / 2;
				else r = (l + r) / 2;
			}
			return l;
		}
};
/*//example
int main() {
	segTreeC st(256);
	for(int i = 0; i < 50; i++) st.inc(5 * i + 2);
	for(int i = 1; i <= 256; i++) std::cout << i << "\t" << st.sum(i) << "\n";
	std::cout << st.mid(26);
	return 0;
}
*/
