#include <cmath>
#include "bitmap.cpp"

void gsus(Bitmap *p){//unsharp mask based on gaussian blur
	float gsus[3][3] = {{-0.0947416,-0.118318,-0.0947416},
	 					{-0.118318,1.852239,-0.118318},
	 					{-0.0947416,-0.118318,-0.0947416}};
	BMP256 g(p->height, p->width);
	for(int y = 0; y < g.height - 2; y++){
		for(int x = 0; x < g.width - 2; x++){
			float s = 0;
			for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++)
					s += (float)(p->px[y + i][x + j]) * gsus[i][j];
			int p = s;
			if(p < 0) p = 0;
			if(p > 255) p = 255;
			g.px[y + 1][x + 1] = p;
		}
	}
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 'u';
			g.name[i++] = 'm'; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_um.bmp"
		}
	g.writeFile(g.name);
}

void lpls(Bitmap *p){//Laplace edge detection
	float **c = new float*[p->height];
	for(int i = 0; i < p->height; i++) c[i] = new float[p->width];
	float q = 1e9, a = 0;
	for(int y = 1; y < p->height - 1; y++){
		for(int x = 1; x < p->width - 1; x++){
			c[y][x] = (float)p->px[y - 1][x] + (float)p->px[y + 1][x] + (float)p->px[y][x - 1]
			+ (float)p->px[y][x + 1] + (float)p->px[y - 1][x - 1] + (float)p->px[y - 1][x + 1]
			+ (float)p->px[y + 1][x - 1] + (float)p->px[y + 1][x + 1] - 8.0 * (float)p->px[y][x];
			if(c[y][x] > a) a = c[y][x];
			if(c[y][x] < q) q = c[y][x];
		}
	}
	a = 255.0 / (a - q);
	BMP256 g(p->height, p->width);
	for(int y = 1; y < g.height - 1; y++)
		for(int x = 1; x < g.width - 1; x++)
			g.px[y][x] = a * (c[y][x] - q);
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 'l';
			g.name[i++] = 'p'; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_lp.bmp"
		}
	g.writeFile(g.name);
}

void rns(Bitmap *p, bool **f, int l, int y, int x){ //recursive naignbor search
	if(x > 0) if((!f[y][x-1]) && p->px[y][x-1] > l) {
		f[y][x-1] = true;
		rns(p, f, l, y, x - 1);
	}
	if(y > 0) if((!f[y-1][x]) && p->px[y-1][x] > l) {
		f[y-1][x] = true;
		rns(p, f, l, y - 1, x);
	}
	if(x < p->width - 1) if((!f[y][x+1]) && p->px[y][x+1] > l) {
		f[y][x+1] = true;
		rns(p, f, l, y, x + 1);
	}
	if(y < p->height - 1) if((!f[y+1][x]) && p->px[y+1][x] > l) {
		f[y+1][x] = true;
		rns(p, f, l, y + 1, x);
	}
}

void spr(Bitmap *p, int l, int h){//supression for canny algorithm
	bool **f = new bool*[p->height];
	for(int i = 0; i < p->height; i++){
		f[i] = new bool[p->width];
		for(int j = 0; j < p->width; j++) f[i][j] = (p->px[i][j] > h);
	}
	for(int y = 0; y < p->height; y++)
		for(int x = 0; x < p->width; x++)
			if(f[y][x]) rns(p, f, l, y, x);
	for(int y = 0; y < p->height; y++)
		for(int x = 0; x < p->width; x++)
			p->px[y][x] = f[y][x] ? 255 : 0;
}

void sobe(Bitmap *p){//Sobel edge detector
	float **c = new float*[p->height];
	for(int i = 0; i < p->height; i++) c[i] = new float[p->width];
	float q = 1e9, a = 0;
	for(int y = 1; y < p->height - 1; y++){
		for(int x = 1; x < p->width - 1; x++){
			c[y][x] = fabs(p->px[y - 1][x - 1] + 2 * p->px[y - 1][x] + p->px[y - 1][x + 1]
				- p->px[y + 1][x - 1] - 2 * p->px[y + 1][x] - p->px[y + 1][x + 1])
				+ fabs(p->px[y - 1][x - 1] + 2 * p->px[y][x - 1] + p->px[y + 1][x - 1]
				- p->px[y - 1][x + 1] - 2 * p->px[y][x + 1] - p->px[y + 1][x + 1]);
			if(c[y][x] > a) a = c[y][x];
			if(c[y][x] < q) q = c[y][x];
		}
	}
	a = 255.0 / (a - q);
	BMP256 g(p->height, p->width);
	for(int y = 1; y < g.height - 1; y++)
		for(int x = 1; x < g.width - 1; x++)
			g.px[y][x] = a * (c[y][x] - q);
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 's';
			g.name[i++] = 'b'; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_sb.bmp"
		}
	g.writeFile(g.name);
	//canny algorithm
	spr(&g, 30, 100);
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 'c';
			g.name[i++] = 'a'; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_ca.bmp"
		}
	g.writeFile(g.name);
}

int main(){
	PGM5 p1("test3.pgm");
	TIF p2("test4.tif");
	gsus(&p1);
	gsus(&p2);
	lpls(&p1);
	lpls(&p2);
	sobe(&p1);
	sobe(&p2);
	return 0;
}
