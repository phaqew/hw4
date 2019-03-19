#include "bitmap.cpp"
#include "segTreeC.cpp"
void mdf(Bitmap *p, int z){
	BMP256 g(p->height, p->width);
	segTreeC t(256);
	int m = z * z / 2 + 1;
	for(int y = z / 2; y < g.height - z / 2 - 1; y++){
		t.clr();
		for(int i = y - z / 2; i <= y + z / 2; i++)
			for(int j = 0; j < z; j++) t.inc(p->px[i][j]);
		g.px[y][z / 2] = t.mid(m);	
		for(int x = 1 + z / 2; x < g.width - z / 2 - 1; x++){
			for(int i = y - z / 2; i <= y + z / 2; i++){
				t.inc(p->px[i][x + z / 2]);
				t.dec(p->px[i][x - z / 2 - 1]);
			}
			g.px[y][x] = t.mid(m);
		}
	}
	for(int i = 0; ; i++)
		if((g.name[i] = p->name[i]) == '.'){
			g.name[i++] = '_'; g.name[i++] = 'm';
			g.name[i++] = '0' + z; g.name[i++] = '.';
			g.name[i++] = 'b'; g.name[i++] = 'm';
			g.name[i++] = 'p'; g.name[i++] = '\0';
			break; //attach "_mk.bmp"
		}
	g.writeFile(g.name);
}

int main(){
	PGM5 p1("test1.pgm");
	TIF p2("test2.tif");
	mdf(&p1, 3);
	mdf(&p2, 3);
	mdf(&p1, 5);
	mdf(&p2, 5);
	mdf(&p1, 7);
	mdf(&p2, 7);
	/*
	BMP256 t(512, 512);
	for(int i = 0; i < 512; i++)
		for(int j = 0; j < 512; j++)
			t.px[i][j] = p2.px[i][j];
	t.writeFile("test2.bmp");
	*/
	return 0;
}
