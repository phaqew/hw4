#include <cmath>
#include "bitmap.cpp"
#define sig -4.5 //-2*sigma^2
void show(int);
void gsf(Bitmap *p, int z){
	float **gs = new float*[z];
	float gss = 0;
	for(int i = 0; i < z; i++){
		gs[i] = new float[z];
		for(int j = 0; j < z; j++){
			int d = (i - z / 2) * (i - z / 2) + (j - z / 2) * (j - z / 2);
			gs[i][j] = std::exp((float)d / sig);
			gss += gs[i][j];
		}
	}
	for(int i = 0; i < z; i++) for(int j = 0; j < z; j++) gs[i][j] /= gss;
	
	BMP256 g(p->height, p->width);
	for(int y = 0; y < g.height - z - 1; y++){
		for(int x = 0; x < g.width - z - 1; x++){
			float s = 0;
			for(int i = 0; i < z; i++) for(int j = 0; j < z; j++)
					s += (float)(p->px[y + i][x + j]) * gs[i][j];
			g.px[y + z / 2][x + z / 2] = (int)s;
		}
	}
	
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 'g';
			g.name[i++] = '0' + z; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_gk.bmp"
		}
	g.writeFile(g.name);
}

int main(){
	/*
	show(3);
	show(5);
	show(7);
	*/
	PGM5 p1("test1.pgm");
	TIF p2("test2.tif");
	gsf(&p1, 3);
	gsf(&p2, 3);
	gsf(&p1, 5);
	gsf(&p2, 5);
	gsf(&p1, 7);
	gsf(&p2, 7);
	return 0;
}

void show(int z){
	char n[] = "gaussian0.csv";
	n[8] += z;
	std::ofstream out(n);
	float **gs = new float*[z];
	float gss = 0;
	for(int i = 0; i < z; i++){
		gs[i] = new float[z];
		for(int j = 0; j < z; j++){
			int d = (i - z / 2) * (i - z / 2) + (j - z / 2) * (j - z / 2);
			gs[i][j] = std::exp((float)d / sig);
			gss += gs[i][j];
		}
	}
	for(int i = 0; i < z; i++) for(int j = 0; j < z; j++) gs[i][j] /= gss;
	for(int i = 0; i < z; i++) {
		for(int j = 0; j < z; j++) out << gs[i][j] << ",";
		out << "\n";
	}
}
