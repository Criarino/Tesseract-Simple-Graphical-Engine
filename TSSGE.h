#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
//#include <iostream>

using namespace std;

struct TS_Rect //representa um retangulo desenhado pela engine
{
	SDL_Rect r;
	SDL_Color c;
	bool fil;
};

struct TS_Texto //representa um texto simples na tela (não caixa de texto)
{
	string msg;
	int f;
	int x, y;
	SDL_Color c;
};

class TS_Objeto //representa objetos na tela
{
	private:
	SDL_Rect dest; //onde na tela vai ficar
	SDL_Rect src; //escala da imagem original
	SDL_Texture* tex; //textura
	int id=0;

	public:
	TS_Objeto(){tex=NULL;}
	~TS_Objeto(){SDL_DestroyTexture(tex);}
	//getters e setters
	SDL_Rect getDest() {return dest;}
	SDL_Rect getSource() {return src;}
	SDL_Texture* getTex() {return tex;}
	int getSW() {return src.w;}//pegar informações da imagem original
	int getSH() {return src.h;}
	int getDX() {return dest.x;}//para saber posição do objeto (getDestX)
	int getDY() {return dest.y;}
	int getDW() {return dest.w;}
	int getDH()	{return dest.h;}
	int getID() {return id;}

	void setDest(int x, int y, int w, int h);
	void setDest(int x, int y);//para mover a tela
	void setSource(int x, int y, int w, int h);//de onde vai começar a carregar a imagem, qual a porção da imagem que vai carregar. Pode cortar partes da imagem
	void setImage(string filename, SDL_Renderer* ren);//carrega uma textura
	void setID(int ID){id=ID;}
};

class TS_audio //para audio
{
	private:
	SDL_AudioSpec wavSpec;//apenas arquivos wave
	Uint32 wavLength; //TEM que ser esse o tipo
	Uint8* wavBuffer;
	SDL_AudioDeviceID deviceID;
	
	public:
	~TS_audio();
	void Adicionar(string filename);
	void Play();
};

class TS_Botao : public TS_Objeto//um objeto com função extra de botão
{
	private:
	bool click=false;
	public:
	int tamx, tamy;
	void Setclick(bool set){click=set;}
	bool Getclick(){return click;}
	bool Sobre();//verifica se o mouse está sobre o botão
	void setDest(int x, int y, int w, int h);
};

class TS_CaixaTxt : public TS_Botao//um botão com função extra de caixa de texto
{
	private:
	string txt;//guarda o texto
	bool ativ;//se a caixa está ativa ou não
	int ajx, ajy;//ajuste da posição do texto dentro da caixa
	public:
	TS_CaixaTxt();
	int r, g, b, f; //cor, handle da fonte
	//getters e setters
	string Gettxt(){return txt;}
	bool Getativ(){return ativ;}
	int GetAjx(){return ajx;}
	int GetAjy(){return ajy;}
	void Setativ(bool set){ativ=set;}
	void SetAj(int x, int y){ajx=x;ajy=y;}//seta as posições de ajuste do texto
	
	void Texto(string a);//para atualizar o texto da caixa
	void Limpar(){txt="";}//para limpar o texto da caixa
	void Ativar();//para ativar/desativar a caixa de texto
};

class TS //engine em si
{
	private:
	int altura;//tamanho da janela
	int largura;
	SDL_Renderer* rende;//renderizador
	SDL_Window* janela;//janela
	vector<TTF_Font*>FontHandles;//fontes abertas
	SDL_Event eventos;//fila de eventos
	double frameCount=0, timerFPS, lastFrame=0;//para calcular e limitar fps
	
	public:
	string buftxt;
	~TS();
	TS();
	SDL_Window* getJanela(){return janela;}
	SDL_Event* getEvent(){return &eventos;}
	SDL_Renderer* getRende(){return rende;}
	void CriarJanela(int alt, int lar, string nome);
	vector<int> HaInput();//verificar se há input e qual é esse input. Retorna 0 se não tiver, -1 se o usuário clicou no X da janela, retorna um vector de duas posições nos outros casos: o primeiro elemento do vector é o tipo de evento e o segundo o código do evento (ascii caso seja do teclado)
	void SetBackground(int r, int g, int b, int a);//seta um fundo
	void SetBackground(int r, int g, int b, int a, TS_Objeto* o);//setar um fundo com imagem
	int AbrirFonte(string arqv, int tam);//abre uma fonte de texto. Retorna o código para essa fonte (diretório do arquivo ttf, tamanho)
	void Limite(int fps);//limita o número de vezes que uma seção de código vai rodar por segundo
	void Commit();//mostra o renderizador (frame) na tela
	void CriarObj(TS_Objeto* aux, string img, int x, int y, int l, int a);//cria um objeto (referência de um objeto, caminho de diretório para a textura, posção xy de onde vai começar a carregar essa textura, largura e altura dessa textura)
	void CriarBtn(TS_Botao* aux, string img, int x, int y, int l, int a);//criar um botão a partir de uma imagem
	void CriarBtn(TS_Botao* aux, int l, int a);//cria um botão invisível, NÃO SE DEVE CHAMAR A FUNÇÃO "DESENHAR" NESSE CASO
	void CriarCaixaTxt(TS_CaixaTxt* aux, string img, int f, int r, int g, int b, int x, int y, int l, int a);//cria uma caixa de texto com imagem (fonte, cor do texto, ponto do qual começará a carregar a imagem original, tamanho)
	void CriarCaixaTxt(TS_CaixaTxt* aux, int f, int r, int g, int b, int l, int a);//para criar uma caixa de texto invisível(mesmo que o anterior, mas sem ponto para carregar imagem)
	TS_Rect CriarRect(bool fil, int x, int y, int alt, int lar, int r, int g, int b, int a);//criar um TS_Rect
	TS_Texto CriarTexto(string msg, int f, int x, int y, int r, int g, int b);//criar um TS_Texto
	void Desenhar(TS_Objeto* o);//quando o objeto já tem posição
	void Desenhar(TS_Objeto* o, int x, int y);//quando quer atribuir posição ao objeto
	void Desenhar(string msg, int f, int x, int y, int r, int g, int b);//para texto (mensagem, codigo da fonte, posição xy, cor rgb)
	void Desenhar(bool fil, int alt, int lar, int x, int y, int r, int g, int b, int a);//para retangulos(se é para preencher, altura x largura, posição xy, cor)
	void Desenhar(int x, int y, int fx, int fy, int r, int g, int b);//Para linhas(ponto xy inicial, ponto xy final,...)
	void Desenhar(int x, int y, int r, int g, int b);//Para pontos
	void Desenhar(TS_CaixaTxt* a, int mx, int my);//para desenhar o texto de uma caixa (caixa, ajuste x y do texto)
};

class TS_Tela//classe abstrata para telas
{
	protected:
	vector<shared_ptr<TS_Objeto>>conteudo;//onde ficam todos os TS_Objetos a serem desenhados na tela
	vector<shared_ptr<TS_Botao>>botoes;//onde ficam todos os TS_Botao
	vector<shared_ptr<TS_Botao>>botoesInv;//botoes que não devem ser desenhados
	vector<shared_ptr<TS_CaixaTxt>>caixastxt;//caixas de texto
	vector<shared_ptr<TS_CaixaTxt>>caixastxtInv;//caixas de texto que não devem ser desenhadas
	vector<TS_Rect>retangulos;//onde ficam todos os retangulos a serem desenhados
	vector<TS_Texto>textoSimples;//onde fica todo o texto simples a ser desenhado na tela
	
	public:
	void Desenhar (TS* Engine);//implementada em TSSGE.cpp. Desenha tudo que está nos vectors na tela
	virtual void MenuPrinc()=0;//o que será o menu principal da sua tela
};
