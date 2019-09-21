/*
PALAVRAS RESERVADAS:
PROGRAMAINICIO, EXECUCAOINICIO, FIMEXECUCAO, FIMPROGRAMA, DEFINAINSTRUCAO, COMO, INICIO, FIM,
REPITA, VEZES, FIMREPITA, ENQUANTO, FACA, FIMPARA, SE, ENTAO, FIMSE, SENAO, FIMSENAO, MOVA, PASSOS,
VIRE PARA, PARE, FINALIZE, APAGUE LAMPADA, ACENDA LAMPADA, AGUARDE ATE, ROBO PRONTO, ROBO OCUPADO,
ROBO PARADO, ROBO MOVIMENTADO, FRENTE ROBO BLOQUEADA, DIREITA ROBO BLOQUEADA, ESQUERDA ROBO BLOQUEADA,
LAMPADA ACESA A FRENTE, LAMPADA APAGADA A FRENTE, LAMPADA ACESSA A ESQUERDA, LAMPADA APAGADA A ESQUERDA,
LAMPADA ACESA A DIREITA, LAMPADA APAGADA A DIREITA, ESQUERDA, DIREITA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

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
	cout << "Erro: erro léxico na linha " << linha << ", coluna " << coluna << ". \n";
}

vector<token> analisadorLexico(char buffer[]) {
	vector<token> tokens;
	int linha = 1, coluna = 1, estado = 0, i = 0, aux = 0;
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
					aux = 0;
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
					aux++;
					coluna++;
					i++;
				}
				else if (buffer[i] == '\t' || buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0') {
					string texto = "";
					for (int j = 0; j < aux; j++) {
						texto += buffer[i + j - aux];
					}
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
	char buffer[3] = {'1','2','3'};
	vector<token> teste = analisadorLexico(buffer);

	cout << teste[0].texto << " " << teste[0].tipo << endl;
    return 0;
}