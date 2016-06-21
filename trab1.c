// COMPILAR: gcc trab.c -o trab -lglut -lGLU -lGL -lm -Wall
// RODAR: ./trab

//Biblioteca necessárias
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

//Defines utilizados para marcar as posições dos textos
#define TEXT_START 0
#define TEXT_GAMEOVER 1
#define TEXT_RESTART 2
#define TEXT_LIVES_NO_FPS 3
#define TEXT_LIVES_FPS 4

//Parâmetros de câmera: determinados através de testes
float eyeX = 190.0f;
float eyeY = -160.0f;
float eyeZ = -500.0f;
float centerX = 190.0f;
float centerY = 0.0f;
float centerZ = 0.0f;
float upX = 0.0f;
float upY = 1.0f;
float upZ = 0.0f;

//Indica se o painel do motorista aparece ou não
int painelAtivado = 1;

//Utilizado para mover a câmera (SOMENTE QUANDO EM PRIMEIRA PESSOA)
float lx = 0.0f;

//Indica se a câmera deve ou não ir para FPS (1 EM FPS, 0 EM NÃO)
int cameraFPS = 0;

//Indica se a câmera está ou não em FPS
int activateFPSGameplay = 0;

//Textos utilizados durante o programa, assim como suas respectivas posições
char textoInicio[100] = "Pressione S ou s para iniciar o jogo!";
float textoInicioPos[3] = {-200, 400, 1800};

char textoFinal1[100] = "Game Over!";
float textoFinal1Pos[3] = {100, 600, 1800};

char textoFinal2[100] = "Pressione R ou r para reiniciar o jogo!";
float textoFinal2Pos[3] = {-200, 600, 1600};

char textoVidas [100] = "Voce tem 3 vidas !!";
float textoVidasPosNoFPS[3] = {-400, -200, 2010};
float textoVidasPosFPS[3] = {-400, -450, 2010};

//Struct que armazena as posições em X e Y das faixas (utilizado para tornar contínuo)
typedef struct faixa_s {
	float x;
	float z;
} faixa_t;

//Vetor de grama (Utilizado para que a grama seja contínua, vários poligonos vão sendo gerados e destruídos)
float gramaPistaPos[3];

//Tamanho da faixa em X e Z (Y é o mesmo para todas as faixas)
float faixaSizeX = 20.0;
float faixaSizeZ = 100.0;

//Cores da faixa (BRANCO)
float faixaR = 1.0;
float faixaG = 1.0;
float faixaB = 1.0;

//Ao sofrer uma colisão, o carro deve girar. Essa variável é utilizado no "rotate"
float spin = 360.0f;
//Variável que indica se o carro deve rodar ou não
int spinActive = 0;

//Quantidade de faixas (muitas para preencher o horizonte)
int qtdeFaixas = 1000;

//Struct que armazena as posições e X e Z de cada faixa
faixa_t faixas[1000];

// Coordenadas do carro
float carroX = 165.0f;
float carroY = 0.0f;
float carroZ = -270.0f;

// Tamanho do carro
float carroSizeX = 50.0;
float carroSizeY = -50.0;
float carroSizeZ = 100.0;

// Cores do carro (BRANCO) - Textura será colocada por cima.
float carroR = 0.0;
float carroG = 0.0;
float carroB = 1.0;

// Coordenadas da barreira
float barreiraX;
float barreiraY;
float barreiraZ;

// Tamanho da barreira
float barreiraSizeX = 100.0;
float barreiraSizeY = -100.0;
float barreiraSizeZ = 10.0;

// Cores da barreira
float barreiraR = 1.0;
float barreiraG = 0.0;
float barreiraB = 0.0;

// Coordenadas do boost
float boostX;
float boostY;
float boostZ;

// Tamanho do objeto
float boostSizeX = 20.0;
float boostSizeY = -20.0;
float boostSizeZ = 20.0;

// Cores da barreira
float boostR = 1.0;
float boostG = 0.0;
float boostB = 1.0;

// Coordenadas da pista
float pistaX = -10.0;
float pistaZ = -270.0;

// Tamanho da pista
float pistaSizeX = 400.0;
float pistaSizeZ = 5000.0;

//PLano utilizado para representar o fim do horizonte e o céu
float planoZ = 4800.0;

// Cor da pista
float pistaR = 0.1;
float pistaG = 0.1;
float pistaB = 0.1;

// Direcao do movimento do carro
float movx = 1.0;
float movy = 1.0;

// Velocidade Inicial do carro
float velocidade = 10;

// Variavel aleatoria para ver se é boost ou barreira
int isBoost = 2;

//Número de vidas do jogo
int lives = 3;

// Indica o ínicio do jogo
int start = 0;

// Utilizados para carregar as texturas
int w, h;

// Variáveis utilizadas para definir a perpectiva
GLfloat angle, fAspect;

// VAriáveis para carregar as texturas
GLuint texPista; 
GLuint texCarroEsq;
GLuint texCarroDir;
GLuint texCarroFrente;
GLuint texCarroFundo;
GLuint texCarroTopo;
GLuint texGrama;
GLuint texBarreira;
GLuint texBoost;
GLuint texFundo;
GLuint texPainel;

// Propriedades de iluminacao da pista
GLfloat faixasDifusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat faixasEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat faixasBrilho[] = { 100.0 };

// Propriedades de iluminacao da pista
GLfloat carroDifusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat carroEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat carroBrilho[] = { 60.0 };

// Propriedades de iluminacao da pista
GLfloat barreiraDifusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat barreiraEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat barreiraBrilho[] = { 50.0 };

// Propriedades de iluminacao da pista
GLfloat boostDifusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat boostEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat boostBrilho[] = { 50.0 };

// Propriedades de iluminacao da pista
GLfloat pistaDifusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat pistaEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat pistaBrilho[] = { 50.0 };

//Função que carrega uma textura necessária (DEVE ESTAR EM FORMATO .bmp E TAMANHO MÚLTIPLO DE POTÊNCIA DE 2)
char *load_bmp (char * name){ 

	FILE *f = fopen(name,"rb");
	if(f == NULL) exit(0);
	int of;
	fseek(f, 10, SEEK_SET);
	fread(&of, sizeof(int), 1, f);
	fseek(f, 4, SEEK_CUR);
	fread(&w, sizeof(int), 1, f);
	fread(&h, sizeof(int), 1, f); 

	fseek(f, of, SEEK_SET); 

	int by = ((w * 3 + 3)/4) * 4 - (w * 3 % 4);
	char *tmp_m = malloc(sizeof(char) * by * h);
	char *m = malloc(sizeof(char) * w * h * 3);
	fread(tmp_m, sizeof(char) * by * h, 1, f);
	int x,y,i;
	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++){
			for(i = 0; i < 3; i++){
				m[3*(w*y + x) + i] = tmp_m[3*(w*y + x) + (2 - i)];
			}
		}
	}
	free(tmp_m);
	return m;
} 

//Carrega uma textura
GLuint loadTex (char *c){
		char *wa = load_bmp(c);
		GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, wa);
		free(wa);
		return texid;
} 

//Define alguns parâmetros para a colocação da textura e habilitá-a
void colocarTextura (GLuint textura){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textura);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// Função que muda a câmera de terceira pessoa para primeira pessoa (valores determinados por teste)
void changeCameraFPS() {

	if (eyeY != -20.0f)		//Muda a posição da câmera em Y, somando 10 até que se atinga a posição desejada (-20)
	{
		eyeY += 10.0f;		//Vai somando 10 até atingir o desejado
	}
	if (eyeZ != -160.0f)	//Muda a posição da câmera em z, somando 10 até que se atinga a posição desejada (-160)
	{
		eyeZ += 10.0f;		//Vai somando 10 até atingir o desejado
	}

}

// Função que muda a câmera de primeira pessoa para terceira pessoa (valores determinados por teste)
void changeCameraFPSBack() {

	if (eyeY != -160.0f)	//Muda a posição da câmera em Y, subtraindo 10 até que se atinga a posição desejada (-160)
	{
		eyeY -= 10.0f;		//Vai subtraindo 10 até atingir o desejado
	}
	if (eyeZ != -500.0f)	//Muda a posição da câmera em z, subtraindo 10 até que se atinga a posição desejada (-500)
	{
		eyeZ -= 10.0f;		//Vai subtraindo 10 até atingir o desejado
	}
}

// Define o tipo de visualização (qual tipo de câmera usar - primeira pessoa ou terceira pessoa)
// A mudança das câmera acontece aqui
void parametrosVisualizacao () {

	// Especifica sistema de coordenadas de projeção e o inicializa
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle,fAspect,0.1,5000);

	// Especifica sistema de coordenadas do modelo e o inicializa
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Processo de transição da câmera (de terceira para primeira pessoa)
	if (cameraFPS == 1 && eyeZ != -160.0f)
	{
		changeCameraFPS();					
		activateFPSGameplay = 1;
	}
	// Processo de transição da câmera (de primeira para terceira pessoa)
	if (cameraFPS == 0 && eyeZ != -500.0f)
	{
		changeCameraFPSBack();
		activateFPSGameplay = 0;
	}

	//Dependendo do tipo de câmera, as variáveis de gluLookAt devem mudar
	//Caso em terceira pessoas (câmera deve permanecer fixa!)
	if (activateFPSGameplay == 0)
	{
		gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	}
	//Caso primeira pessoa (câmera deve se mover ao longo de X)
	else if (activateFPSGameplay == 1)
	{
		gluLookAt(eyeX+lx, eyeY, eyeZ, centerX+lx, centerY, centerZ, upX, upY, upZ);
	}
}

//Desenha o carro e o painel (o painel só aparece em primeira pessoa)
void desenhaCarro () {

	//Define propriedades de iluminação do material	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, carroDifusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, carroEspecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, carroBrilho);

	//Inicia o processo de desenho do carro
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);			//Sempre desenhando a partir da origem !!

	//Cor branca para aplicação da textura
 	glColor3f(1.0f, 1.0f, 1.0f);
 	
 	//Caso se esteja em primeira pessoa, e o usuário tenha aprtado 'p' (mostra o painel), o painel deve ser dedenhado
 	if (activateFPSGameplay==1 && painelAtivado==1) {
	 	
 		//Carrega a textura do painel
	 	colocarTextura(texPainel);

	 	//Utilizado caso seja necessário redimensionar a janela
	 	float tradeoff=0.0f;

	 	//Verifica o tamanho da janela
	 	if (glutGet(GLUT_WINDOW_WIDTH)>1000)
	 		tradeoff=-5.0f;
	 	else
	 		tradeoff=+5.0f;

	 	//Desenha o painel
	 	glBegin(GL_QUADS);
	 	glNormal3f(0,1,0);
	 	glTexCoord2f(1.0f, 0.0f);	glVertex3f(carroX+tradeoff, carroY + 2.0f, carroZ + carroSizeZ+50.0f);
	 	glTexCoord2f(1.0f, 1.0f);	glVertex3f(carroX + carroSizeX-tradeoff, carroY + 2.0f, carroZ + carroSizeZ+50.0f);
	 	glTexCoord2f(0.0f, 1.0f);	glVertex3f(carroX + carroSizeX-tradeoff, carroY + carroSizeY + 40.0f, carroZ + carroSizeZ+50.0f);
	 	glTexCoord2f(0.0f, 0.0f);	glVertex3f(carroX+tradeoff, carroY + carroSizeY + 40.0f, carroZ + carroSizeZ+50.0f);
		
	 	glDisable(GL_TEXTURE_2D);
		glEnd();

		glPopMatrix();
	}

	//Caso o carro não tenha colidido com uma parede, e portanto, não deve rodar !!
	if (spinActive == 0)
	{
		//Desenha o carro (5 faces)
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
	
		// Face dianteira
		colocarTextura(texCarroFrente);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(carroX, carroY, carroZ + carroSizeZ);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(carroX, carroY + carroSizeY, carroZ + carroSizeZ);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();

		// Face lateral direita
		colocarTextura(texCarroDir);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);  		glVertex3f(carroX, carroY, carroZ);
		glTexCoord2f(1.0f, 1.0f);  		glVertex3f(carroX, carroY + carroSizeY, carroZ);
		glTexCoord2f(0.0f, 1.0f);  		glVertex3f(carroX, carroY + carroSizeY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);  		glVertex3f(carroX, carroY, carroZ + carroSizeZ);

		glDisable(GL_TEXTURE_2D);
		glEnd();

		// Face lateral esquerda
		colocarTextura(texCarroEsq);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(carroX + carroSizeX, carroY, carroZ);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();
		
		// Face traseira
		colocarTextura(texCarroFundo);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(carroX, carroY, carroZ);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(carroX, carroY + carroSizeY, carroZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(carroX + carroSizeX, carroY, carroZ);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();
		

		// Face superior
		colocarTextura(texCarroTopo);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(carroX, carroY + carroSizeY, carroZ);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(carroX, carroY + carroSizeY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(carroX + carroSizeX, carroY + carroSizeY, carroZ);

		glDisable(GL_TEXTURE_2D);
		glEnd();	
		
		glPopMatrix();
	}
	
	// Caso o carro tenha colidio com uma parede, e portanto, deve girar. Note que isto não ocorre
	// quando a câmera está em primeira pessoa.
	else if (spinActive == 1 && activateFPSGameplay == 0)
	{

		// Para desenhar, mover a "origem" para o centro do carro, de modo que a rotação possa ser feita
		// em torno da "suposta" origem (rodar o carro no próprio eixo Y)
		glPushMatrix();
		glTranslatef(carroX+25.0f, carroY, carroZ+50.0f);
		glRotatef(spin, 0.0f, 1.0f, 0.0f);					//Rotação em torno de Y

		// Carro é rodado de 10 em 10 graus
		spin = spin - 10.0f;

		// Face dianteira
		colocarTextura(texCarroFrente);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(-25.0f, 0, -50.0f + carroSizeZ);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f + carroSizeZ);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();

		// Face lateral direita
		colocarTextura(texCarroDir);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(-25.0f, 0, -50.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-25.0f, 0, -50.0f + carroSizeZ);

		glDisable(GL_TEXTURE_2D);
		glEnd();

		// Face lateral esquerda
		colocarTextura(texCarroEsq);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(-25.0f + carroSizeX, 0, -50.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();
		
		// Face traseira
		colocarTextura(texCarroFundo);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-25.0f, 0, -50.0f);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(-25.0f + carroSizeX, 0, -50.0f);
		
		glDisable(GL_TEXTURE_2D);
		glEnd();
		

		// Face superior
		colocarTextura(texCarroTopo);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f(-25.0f, 0 + carroSizeY, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f + carroSizeZ);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-25.0f + carroSizeX, 0 + carroSizeY, -50.0f);

		glDisable(GL_TEXTURE_2D);
		glEnd();
		
		glPopMatrix();

		//Caso a rotaçao tenha sido completada (spin==0)
		if(spin==0)
		{	
			spinActive=0;
			spin = 360.0f;
		}

	}
		glDisable(GL_TEXTURE_2D);
}

//Desenha as barreiras (6 faces)
void desenhaBarreira () {

	//Cor branca para aplicação da textura
	glColor3f(1.0f, 1.0f, 1.0f);

	//Habilita colocação da textura da barreira
	colocarTextura(texBarreira);

	glPushMatrix();
	glTranslatef(0, 0, 0);

	//Define características de iluminaçã do material
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, barreiraDifusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, barreiraEspecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, barreiraBrilho);

	// Face inferior
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX, barreiraY, barreiraZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX, barreiraY, barreiraZ + barreiraSizeZ);
	glEnd();

	// Face lateral direita
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX, barreiraY, barreiraZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX, barreiraY, barreiraZ + barreiraSizeZ);
	glEnd();

	// Face lateral esquerda
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ);
	glEnd();
	
	// Face traseira
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX, barreiraY, barreiraZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ);
	glEnd();
	
	// Face dianteira
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX, barreiraY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glEnd();

	// Face superior
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ);
	glTexCoord2f(0.4f, 0.0f);		glVertex3f(barreiraX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.4f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ + barreiraSizeZ);
	glTexCoord2f(0.0f, 0.4f);		glVertex3f(barreiraX + barreiraSizeX, barreiraY + barreiraSizeY, barreiraZ);
	glEnd();
	
	//Processo de desenho finalizado
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//DEsenha o boost (6 faces)
void desenhaBoost () {

	//Cor branca para aplica textura
	glColor3f(1.0f, 1.0f, 1.0f);

	//Carrega textura de boost
	colocarTextura(texBoost);

	//Origem sempre na origem!
	glPushMatrix();
	glTranslatef(0, 0, 0);

	//Define características de iluminação do material
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, boostDifusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boostEspecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, boostBrilho);

	// Face inferior
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX, boostY, boostZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ + boostSizeZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX, boostY, boostZ + boostSizeZ);
	glEnd();

	// Face lateral direita
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX, boostY, boostZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ + boostSizeZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX, boostY, boostZ + boostSizeZ);
	glEnd();

	// Face lateral esquerda
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ + boostSizeZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ + boostSizeZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ);
	glEnd();
	
	// Face traseira
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX, boostY, boostZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ);
	glEnd();
	
	// Face dianteira
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX, boostY, boostZ + boostSizeZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX + boostSizeX, boostY, boostZ + boostSizeZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ + boostSizeZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ + boostSizeZ);
	glEnd();

	// Face superior
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ);
	glTexCoord2f(1.0f, 0.0f);		glVertex3f(boostX, boostY + boostSizeY, boostZ + boostSizeZ);
	glTexCoord2f(1.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ + boostSizeZ);
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(boostX + boostSizeX, boostY + boostSizeY, boostZ);
	glEnd();
	
	//Desabilita aplicação de textura
	glDisable(GL_TEXTURE_2D);	
	
	glPopMatrix();
}

//Desenha todas as faixas
void desenhaFaixas () {

	int i;

	//Não existe textura na faixa, sua cor é definida como branca
	glColor3f(faixaR, faixaG, faixaB);

	//Para todas as faixas, deve-se definir os parÂmetros de iluminação e suas posições
	for (i = 0; i < qtdeFaixas; ++i) {
		
		glPushMatrix();

		//Define parÂmetros de iluminação
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, faixasDifusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, faixasEspecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, faixasBrilho);

		//Desenha todas as faixas (Posições em z variam para que as faixas se movam)
		glTranslatef(0, 0, faixas[i].z);
		glBegin(GL_QUADS);
		glVertex3f (faixas[i].x, 0, faixas[i].z);
		glVertex3f (faixas[i].x + faixaSizeX, 0, faixas[i].z);
		glVertex3f (faixas[i].x + faixaSizeX, 0, faixas[i].z + faixaSizeZ);
		glVertex3f (faixas[i].x, 0, faixas[i].z + faixaSizeZ);
		glEnd();
		glPopMatrix();
	}
}

//Desenha a pista (asfalto)
void desenhaPista () {

	//Cor branca para aplicação da textura
	glColor3f(1.0f, 1.0f, 1.0f);
	
	//Carrega textura de asfalto
	colocarTextura(texPista);

	int i=0;

	//Desenha a pista. Note que existe mais de uma pista (3 na verdade), paraque a pista se mova junto com o carro
	for (i=0; i<3; i++)
	{

		glPushMatrix();

		//Define propriedades de iluminação
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pistaDifusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pistaEspecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pistaBrilho);

		//Muda eixo para facilitar desenho
		glTranslatef(0, 0, pistaZ);
		glBegin(GL_QUADS);

		//Define normal de reflexão
		glNormal3f(0,1,1);

		//Desenha a pista e aplica as texturas (usa-se pistaSizeZ*1.5 para evitar pequenas descontinuidades e possíveis falhas de arredondamento)
		glTexCoord2f(20.0f, 0.0f);			glVertex3f(pistaX, 0, gramaPistaPos[i]);
		glTexCoord2f(20.0f, 20.0f);			glVertex3f(pistaX + pistaSizeX, 0, gramaPistaPos[i]);
		glTexCoord2f(0.0f, 20.0f);			glVertex3f(pistaX + pistaSizeX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);
		glTexCoord2f(0.0f, 0.0f);			glVertex3f(pistaX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);

		glEnd();

		glPopMatrix();
	}
	
	//Desabilita aplicação de textura
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	//Desenha plano de fundo (céu). É estático, pois os elementos que se movem, e não a câmera.
	colocarTextura(texFundo);

	glBegin(GL_QUADS);

	//Plano deve ser bem grande e estar no fim da pista (no horizonte)
	glNormal3f(0,0,-1);
	glTexCoord2f(1.0, 0.0);			glVertex3f(pistaX-10*pistaSizeX, 0.0f, pistaZ + planoZ);
	glTexCoord2f(1.0, 1.0);			glVertex3f(pistaX-10*pistaSizeX, -1400.0f, pistaZ + planoZ);
	glTexCoord2f(0.0, 1.0);			glVertex3f(pistaX+10*pistaSizeX, -1400.0f, pistaZ + planoZ);
	glTexCoord2f(0.0, 0.0);			glVertex3f(pistaX+10*pistaSizeX, 0.0f, pistaZ + planoZ);

	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

//Desenha a grama (dos dois lados)
void desenhaGramas () {

	//Cor branca para aplicação de textura
	glColor3f(1.0f, 1.0f, 1.0f);
	//Carrega textura para colocar na grama
	colocarTextura(texGrama);

	int i=0;

	//Novamente, existem mais de um polígono para representar a grama. Isso é necessário para que a grama se mova junto com o carro.
	for (i = 0; i < 3; i++) {

		glPushMatrix();

		//Define propriedades de iluminação da grama
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pistaDifusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pistaEspecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pistaBrilho);

		//Para facilitar o desenho da grama
		glTranslatef(0, 0, pistaZ);
		glBegin(GL_QUADS);

		//Define normal de iluminação
		glNormal3f(0,1,0);

		//Desenha a grama a esquerda (pistaSizeZ*1.5 é usado para evitar pequenas descontinuidades e possíveis falhas de arredondamento)
		glTexCoord2f(20.0f, 0.0f);			glVertex3f(pistaX - 10 * pistaSizeX, 0, gramaPistaPos[i]);
		glTexCoord2f(20.0f, 20.0f);			glVertex3f(pistaX, 0, gramaPistaPos[i]);
		glTexCoord2f(0.0f, 20.0f);			glVertex3f(pistaX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);
		glTexCoord2f(0.0f, 0.0f);			glVertex3f(pistaX - 10 * pistaSizeX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);

		glEnd();

		glPopMatrix();

		glPushMatrix();
		
		glBegin(GL_QUADS);

		glNormal3f(0,1,1);

		//Desenha a grama a esquerda (pistaSizeZ*1.5 é usado para evitar pequenas descontinuidades e possíveis falhas de arredondamento)
		glTexCoord2f(20.0f, 0.0f);			glVertex3f(pistaX + pistaSizeX, 0, gramaPistaPos[i]);
		glTexCoord2f(20.0f, 20.0f);			glVertex3f(pistaX + 10 * pistaSizeX, 0, gramaPistaPos[i]);
		glTexCoord2f(0.0f, 20.0f);			glVertex3f(pistaX + 10 * pistaSizeX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);
		glTexCoord2f(0.0f, 0.0f);			glVertex3f(pistaX + pistaSizeX, 0, gramaPistaPos[i] + pistaSizeZ*1.5);

		glEnd();

		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
}

//Função que gera uma nova barreira ou boos (é aleatório)
void novoBoostBarreira () {

	// Seleciona entre boost e barreira
	isBoost = rand() % 2;

	// Inicia novo boost
	if (isBoost == 1) {
		boostX = -10 + (rand() % (int)(405 - boostSizeX));
		boostY = 0;
		boostZ = 5000.0;

	}
	// Inicia nova barreira
	else if (isBoost == 0) {
		barreiraX = -10 + (rand() % (int)(405 - barreiraSizeX));
		barreiraY = 0;
		barreiraZ = 5000.0;
	}
}

// Funcao que faz o carro parar apos quebrar
void gameOver () {

	//Câmera deve voltar para terceira pessoa
	cameraFPS = 0;

	//Reduz a velocidade do carro, até que ele pare
	if (velocidade > 0.0)
		velocidade -= 0.1;
	else
		velocidade = 0.0;

}

//Função que modifica o texte que indica o número de vidas
void modificaTexto3() {

	textoVidas[9] = lives + '0';
}

//Funçaõ que verifica a colisão
void verificaColisao () {

	int i = 0;

	if (lives == 0) {
		gameOver();
	}
	else {

		// Trata colisao do boost
		if (isBoost == 1) {
			// Com a tela
			if (boostZ + boostSizeZ <= -300){
				novoBoostBarreira();				//Cria novo boost ou barreira
			}
			//Com o carro
			if (boostZ <= carroZ + carroSizeZ) {
				
				//Colisão do meio (carro está dentro dos limites da boost - caso mude o tamanho do boost!!)
				if ((carroX >= boostX) && (carroX + carroSizeX <= boostX + boostSizeX)) {
						velocidade += 2.0;			//Aumenta a velocidade do carro
						novoBoostBarreira();		//Cria um novo boost ou barreira
				}
				
				//Colisão das laterias - esquerda (apenas parte do carro está nos limites da boost)
				else if ((carroX + carroSizeX >= boostX) && (carroX < boostX)) {
						velocidade += 2.0;			//Aumenta a velocidade do carro
						novoBoostBarreira();		//Cria um novo boost ou barreira
				}
				//Colisão das laterias - direita (apenas parte do carro está nos limites da boost)
				else if ((boostX + boostSizeX >= carroX) && (carroX > boostX)) {
						velocidade += 2.0;			//Aumenta a velocidade do carro
						novoBoostBarreira();		//Cria um novo boost ou barreira
				}

			}
		}

		// Trata colisao da barreira
		else if (isBoost == 0) {
			// Com a tela
			if (barreiraZ + barreiraSizeZ <= -300) {
				novoBoostBarreira();				//Cria novo boost ou barreira
			}
			//Com o carro
			if (barreiraZ <= carroZ + carroSizeZ) {
				
				//Colisão do meio (carro está dentro dos limites da barreira)
				if ((carroX >= barreiraX) && (carroX + carroSizeX <= barreiraX + barreiraSizeX)) {
						lives--;					//Reduz o número de vidas do jogador
						novoBoostBarreira();		//Cria um novo boost ou barreira
						modificaTexto3();			//Modifica o número de vidas no texto
						if (activateFPSGameplay==0)	//Caso a câmera não esteja em primeira pessoa, o carro deve girar devido a colisão
						{	spinActive = 1;	 }
				}
				
				//Colisão das laterias - esquerda (apenas parte do carro está nos limites da barreira)
				else if ((carroX + carroSizeX >= barreiraX) && (carroX < barreiraX)) {
						lives--;					//Reduz o número de vidas do jogador
						novoBoostBarreira();		//Cria um novo boost ou barreira
						modificaTexto3();			//Modifica o número de vidas no texto
						if (activateFPSGameplay==0)	//Caso a câmera não esteja em primeira pessoa, o carro deve girar devido a colisão
						{	spinActive = 1;	 }
				}
				//Colisão das laterias - direita (apenas parte do carro está nos limites da barreira)
				else if ((barreiraX + barreiraSizeX >= carroX) && (carroX > barreiraX)) {
						lives--;					//Reduz o número de vidas do jogador
						novoBoostBarreira();		//Cria um novo boost ou barreira
						modificaTexto3();			//Modifica o número de vidas no texto
						if (activateFPSGameplay==0)	//Caso a câmera não esteja em primeira pessoa, o carro deve girar devido a colisão
						{	spinActive = 1;	 }
				}

			}
		
		}
	}

	// Trata colisao das faixas com o fundo da tela, fazendo com que novas aparecam conforme necessidade
	for (i = 0; i < qtdeFaixas; ++i) {
		if (faixas[i].z + faixaSizeZ <= -200)
			faixas[i].z = 200 * (qtdeFaixas - 1) - 100;
	}

	// Trata a colisão da grama e da pista com o fundo da tela, fazendo que novas aparecam conforme necessidade
	for (i = 0; i < 3; i++) {
		if (gramaPistaPos[i] + pistaSizeZ <= -200)
			gramaPistaPos[i] = pistaZ + (pistaSizeZ*2);
	}
}

//Desenha os textos necessários na tela
void desenhaTexto(int text, char *string) {

		glPushMatrix();
        
        // Posição no universo onde o texto será colocado          
        glColor3f(0.9, 0.9, 0.9);

        //Switch define a posição do texto
        switch (text)
        {
        	case 0:		//Texto de início
        	glRasterPos3f(textoInicioPos[0], textoInicioPos[1], textoInicioPos[2]);
        	break; 
        	case 1:		//Texto de final 1
        	glRasterPos3f(textoFinal1Pos[0], textoFinal1Pos[1], textoFinal1Pos[2]);
        	break; 
        	case 2:		//Texto de final 2
        	glRasterPos3f(textoFinal2Pos[0], textoFinal2Pos[1], textoFinal2Pos[2]);
        	break; 
        	case 3:		//Texto que indica número de vidas (câmera em terceira pessoa)
        	glRasterPos3f(textoVidasPosNoFPS[0], textoVidasPosNoFPS[1], textoVidasPosNoFPS[2]);
        	break;
        	case 4:		//Texto que indica número de vidas (câmera em primeira pessoa)
        	glRasterPos3f(textoVidasPosFPS[0], textoVidasPosFPS[1], textoVidasPosFPS[2]);
        	break;
		}

		//Função que imprimirá o texto
		while(*string)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*string++); 
	
		glPopMatrix();
} 

// **************************************************************************
// **************************************************************************

//Inicia o processo de desenho de todos os objetos
void desenho () {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//Limpa tela e Z-Buffer

	desenhaPista();			//Desenha a pista
	desenhaGramas();		//Desenha a grama
	desenhaFaixas();		//Desenha as faixas

	if (isBoost == 2)			//Necessário gerar uma nova barreira ou boost (início do jogo)
		novoBoostBarreira();

	if (isBoost == 1)			//Desenha Boost ou barreira dependendo do resultado anterior
		desenhaBoost();
	else if (isBoost == 0)
		desenhaBarreira();

	desenhaCarro();				//Desenha o carro

	//Desenha os textos na tela
	//Começo do jogo
	if (start == 0) {
		desenhaTexto(TEXT_START ,textoInicio);
	}
	//Fim do jogo
	if (lives == 0) {
		desenhaTexto(TEXT_GAMEOVER, textoFinal1);
		desenhaTexto(TEXT_RESTART, textoFinal2);
	}
	//Durante o jogo (câmera primeira pessoa)
	if (activateFPSGameplay==1) {
		desenhaTexto(TEXT_LIVES_FPS, textoVidas);
	}
	//Durante o jogo (câmera terceira pessoa)
	else if (activateFPSGameplay==0) {
		desenhaTexto(TEXT_LIVES_NO_FPS, textoVidas);
	}

	//Verifica a colisão de todos os elementos na tela
	verificaColisao();
	glutSwapBuffers();
}

//Movimenta o carro para os lados, quando pressionada as teclas
void movimentaCarro (int tecla, int x, int y) {

	//Caso o usuário não tenha mais vidas, o carro não se move
	if (lives == 0)
		return;

	//Caso o usuário aperte a seta "direita", mover o carro para a direita
	if (tecla == GLUT_KEY_RIGHT)
		if (carroX + carroSizeX <= 395) {		//Verifica se carro não passa dos limites
			carroX += 15.0;						//Muda posição do carro
			lx += 15.0f;						//Muda posição da câmera (quando em primeira pessoa)
		}


	//Caso o usuário aperte a seta "esquerda", mover o carro para a esquerda
	if (tecla == GLUT_KEY_LEFT) {
		if (carroX >= -25) {					//Verifica se carro não passa dos limites
			carroX -= 15.0;						//Muda posição do carro
			lx -= 15.0f;						//Muda posição da câmera (quando em primeira pessoa
		}
	}
}

//REdimensiona a janela, quando necessário
void alterarJanela (GLsizei w, GLsizei h) {
	// Evita a divisao por zero
	if(w == 0) w = 1;
	if(h == 0) h = 1;

	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w/(GLfloat)h;

	//Redefine a visualização
	parametrosVisualizacao();
	glutPostRedisplay();
}

//Move o carro e todos os elementos que precisam ser movidos
void moveCarro (int valor) {
	
	//Caso o jogador tenha iniciado o jogo
	if (start == 1) {
		int i = 0;
	
		//Caso um boost deva ser gerado, fazer o objeto se mover
		if (isBoost == 1) {
			boostZ -= velocidade*2;
		}
		//Caso uma barreira seja gerada, fazer o objeto se mover
		else if (isBoost == 0) {
			barreiraZ -= velocidade*2;
		}
	
		//Fazer todas as faixas se moverem
		for (i = 0; i < qtdeFaixas; ++i)
			faixas[i].z -= velocidade;

		//Fazer a grama e a pista se moverem
		for (i = 0; i < 3; ++i)
			gramaPistaPos[i] -= velocidade;
	}

	//Define os paraêmtros de visualização
	parametrosVisualizacao();
	glutPostRedisplay();				//Reexibe a imagem
	glutTimerFunc(1, moveCarro, 1);		//Cria loop para fazer tudo se mover
}

// Funcao para reiniciar o jogo com as configuracoes iniciais
void reinicia () {

	int i;

	//Caso o usuário tenha perdido todas as vidas, o jogo pode ser reiniciado, e todos os
	//parâmetros devem ser redefinidos
	if (lives == 0) {
		lives = 3;
		velocidade = 10.0f;
		novoBoostBarreira();
		modificaTexto3();

		//Redefinir faixas
		for (i = 0; i < qtdeFaixas; ++i) {
			faixas[i].x = 180.0;
			faixas[i].z = 200.0 * i;
		}

		//Redefinir pista e grama
		for (i = 0; i < 3; i++) {
			gramaPistaPos[i] = pistaZ + (pistaSizeZ * i);
		}
	}
}

//Função que recebe teclas que o usuário poderia pressionar
void keyboard (unsigned char tecla, int x, int y) {

	if (tecla == 'r' || tecla == 'R')		//Tecla R ou r reinicia o jogo
		reinicia();
	if (tecla == 's' || tecla == 'S')		//Tecla que inicia o jogo
		start = 1;
	
	if (tecla == 32) {						//Tecla "espaço" (ASCII 32) - muda a câmera do jogo (Primeira-Terceira oi vice-versa)
		if (cameraFPS == 0)
		{	cameraFPS = 1;	}
		else if (cameraFPS == 1)
		{	cameraFPS = 0;	}
	}

	if (tecla=='p')							//Ativa ou desetiva o painel do motorista
	{	painelAtivado = !painelAtivado; 	}

}

//Iniciliza todos os parâmetros do programa
void inicializar() {
	
	glClearColor (0.0, 0.39, 0.0, 1.0);		//Vai limpar a tela com a cor da grama
	angle = 5000;							//Utilizado na perspectiva

	//Carrega todos os arquivos de textura necessários
	texPista = loadTex("Asphalt3.bmp");
	texGrama = loadTex("grass.bmp");
	texBarreira = loadTex("brick.bmp");
	texBoost = loadTex("fire.bmp");
	texFundo = loadTex("clouds.bmp");
	texCarroEsq = loadTex("esquerda.bmp");
	texCarroDir = loadTex("direita.bmp");
	texCarroFundo = loadTex("fundo.bmp");
	texCarroFrente = loadTex("frente.bmp");
	texCarroTopo = loadTex("topo.bmp");
	texPainel = loadTex("painel.bmp");

	//Define parâmetros de iluminação
	GLfloat luzAmbiente[4]={0.8,0.8,0.8,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	    
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0}; 
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
	GLint especMaterial = 60;

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
}

//Função principal
int main(int argc, char *argv[]) {

	int i;
	// Semente para a geracao de numeros aleatorios
	time_t t;
	srand((unsigned) time(&t));

	//Define a posição inicial das faixas, de acordo com as posições e tamanhos
	for (i = 0; i < qtdeFaixas; ++i) {
		faixas[i].x = 180.0;
		faixas[i].z = 200.0 * i;
	}

	//Define a posiçao inicial da grama e da pista
	for (i = 0; i < 3; i++) {
		gramaPistaPos[i] = pistaZ + (pistaSizeZ * i);
	}

	//Inicial Glut
	glutInit(&argc, argv);

	//Define o modo de visualização
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);

	//Cria uma janela 600x600
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(1, 1);
	glutCreateWindow("Trabalho CG");

	//Inicializa variáveis necessárias
	inicializar();

	//Define função de desenho
	glutDisplayFunc(desenho);

	//Redimensiona a janela quando necessário
	glutReshapeFunc(alterarJanela);

	//Função que inicia o loop, e consequentemente, move os objetos necessários
	glutTimerFunc(1, moveCarro, 1);

	//Define funções de teclas que o usuário podem pressionar
	glutSpecialFunc(movimentaCarro);	//Teclas fora da ASCII (setas, por ex)
	glutKeyboardFunc(keyboard);			//Teclas ASCII

	//REaliza o loop, redesenhando tudo
	glutMainLoop();

	return 0;
}