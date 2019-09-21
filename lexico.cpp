/*
PALAVRAS RESERVADAS:
ACENDA LAMPADA, APAGUE LAMPADA, AGUARDE ATE,
COMO,
DEFINAINSTRUCAO, DIREITA, DIREITA ROBO BLOQUEADA,
ENQUANTO, ENTAO, ESQUERDA, ESQUERDA ROBO BLOQUEADA, EXECUCAOINICIO,
FACA, FIM, FIMEXECUCAO, FIMPARA, FIMPROGRAMA, FIMREPITA, FIMSE, FIMSENAO, FINALIZE, FRENTE ROBO BLOQUEADA,
INICIO,
LAMPADA ACESA A DIREITA, LAMPADA ACESA A ESQUERDA, LAMPADA ACESA A FRENTE,
LAMPADA APAGADA A DIREITA, LAMPADA APAGADA A ESQUERDA, LAMPADA APAGADA A FRENTE, 
MOVA,
PARE, PASSOS, PROGRAMAINICIO,
REPITA, ROBO MOVIMENTADO, ROBO OCUPADO, ROBO PARADO, ROBO PRONTO,
SE, SENAO,
VEZES, VIRE PARA.
*/
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef struct token_t {
	string tipo;
	string texto;
}token;

token novoToken(string tipo, string texto) {
	token novo;
	novo.tipo = tipo;
	novo.texto = texto;
	return novo;
}

void erroLexico(int linha, int coluna) {
	cout << "Erro: erro léxico na linha " << linha << ", coluna " << coluna << ".\n";
}

vector<token> analisadorLexico(char buffer[], map<string,string> reservadas) {
	vector<token> tokens;
	int linha = 1, coluna = 1, estado = 0, i = 0;
	string texto = "";
	while(true) {
		switch(estado) {
			// caso de erro
			case -1:
				erroLexico(linha,coluna);
				return {};
			// caso base
			case 0: 
				if (buffer[i] == '\t' || buffer[i] == ' ') {
					coluna++;
					i++;
				}
				else if (buffer[i] == '\n') {
					coluna = 1;
					linha++;
					i++;
				}
				else if (buffer[i] >= 48 && buffer[i] <= 57) {
					texto = "";
					estado = 1;
				}
				else if ((buffer[i] >= 65 && buffer[i] <= 90) || 
						 (buffer[i] >= 97 && buffer[i] <= 122)) {
					estado = 2;
				}
				else if (buffer[i] == '\0')
					return tokens;

				break;
			// caso NUMERO
			case 1:
				if (buffer[i] >= 48 && buffer[i] <= 57) {
					texto += buffer[i];
					coluna++;
					i++;
				}
				else if (buffer[i] == '\t' || buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0') {
					tokens.push_back(novoToken("numero",texto));

					estado = 0;
				}
				else {
					estado = -1;
				}

				break;
			// caso de sucesso
			case 8:
				return tokens;
		}
	}


}


int main() {
	char buffer[8] = {'1','2','3',' ','3','\n','2','3'};

	// mapa das palavras reservadas
	// (incompleto pq sou preguiçoso)
	map<string,string> reservadas;
	reservadas["ACENDA LAMPADA"] = "ACENDA LAPADA"; reservadas["APAGUE LAMPADA"] = "APAGUE LAMPADA";
	reservadas["AGUARDE ATE"] = "AGUARDE ATE"; reservadas["COMO"] = "COMO";
	reservadas["DEFINAINSTRUCAO"] = "DEFINAINSTRUCAO"; reservadas["DIREITA"] = "DIREITA";
	reservadas["DIREITA ROBO BLOQUEADA"] = "DIREITA ROBO BLOQUEADA"; 

	vector<token> teste = analisadorLexico(buffer,reservadas);

	if (!teste.empty())
		for (int i = 0; i < teste.size(); i++)
		cout << teste[i].texto << " " << teste[i].tipo << endl;
	else
		cout << "empty" << endl;

    return 0;
}
