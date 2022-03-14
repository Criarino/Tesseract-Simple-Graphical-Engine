#include "TSSGE.h"

void TS_Objeto::setDest(int x, int y, int w, int h)
{
	dest.x=x;
	dest.y=y;
	dest.w=w;
	dest.h=h;
}

void TS_Objeto::setDest(int x, int y)
{
	dest.x=x;
	dest.y=y;
}

void TS_Objeto::setSource(int x, int y, int w, int h)
{
	src.x=x;
	src.y=y;
	src.w=w;
	src.h=h;
}

void TS_Objeto::setImage(string filename, SDL_Renderer* rende)
{
	if(tex)
		SDL_DestroyTexture(tex);
	tex=IMG_LoadTexture(rende, filename.c_str());
	if(!tex)
		throw runtime_error(SDL_GetError());
}

bool TS_Botao::Sobre()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if(((getDX()<=x)&&(x<=(getDX()+tamx))) && ((getDY()<=y)&&(y<=(getDY()+tamy))))
		return true;
	else
		return false;
}

void TS_Botao::setDest(int x, int y, int w, int h)
{
	TS_Objeto::setDest(x, y, w, h);
	tamx=w;
	tamy=h;
}

TS_CaixaTxt::TS_CaixaTxt()
{
	txt="";
	ativ=false;
}

void TS_CaixaTxt::Texto(string a)
{
	if ((a.size()>1) && (txt.size()>0))
		txt.pop_back();
	else if(a.size()==1)		
			txt+=a;
}

void TS_CaixaTxt::Ativar()
{
	if (ativ)
	{
		SDL_StopTextInput();
		ativ=false;
	}
	else
	{
		SDL_StartTextInput();
		ativ=true;
	}
}
