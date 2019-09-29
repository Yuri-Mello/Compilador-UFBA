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
#include <cctype>

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
				else if (buffer[i] >= '0' && buffer[i] <= '9') {
					texto = "";
					estado = 1;
				}
				else if ((buffer[i] >= 'A' && buffer[i] <= 'Z') || 
						 (buffer[i] >= 'a' && buffer[i] <= 'z')) {
					texto = "";
					estado = 2;
				}
				else if (buffer[i] == '\0') {
					return tokens;
				}
				else {
					estado = -1;
				}

				break;
			// caso NUMERO
			case 1:
				if (buffer[i] >= '0' && buffer[i] <= '9') {
					texto += buffer[i];
					coluna++;
					i++;
				}
				else if (buffer[i] == '\t' || buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0') {
					tokens.push_back(novoToken("NUM",texto));

					estado = 0;
				}
				else {
					estado = -1;
				}

				break;
			case 2:
				if ((buffer[i] >= 'A' && buffer[i] <= 'Z') ||
					(buffer[i] >= 'a' && buffer[i] <= 'z') ||
					(buffer[i] >= '0' && buffer[i] <= '9')) {
                    texto += toupper(buffer[i]);
                    coluna++;
                    i++;
                }            
                else if (buffer[i] == '\t' || buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0') {
                    if(reservadas.count(texto)) tokens.push_back(novoToken(texto,texto));
                    else tokens.push_back(novoToken("ID",texto));
					
                    estado = 0;
                }
                else {
                    estado = -1;
                }

                break;
		}
	}


}


int main() {
	// char buffer[17] = {'t','e','s','t','e',' ','a','c','e','n','d','a',' ','1','1',' ','\0'};
	// string buffer = "teste acenda 11";

	char buffer[512001];
	int tam = fread(buffer,sizeof(char),512000,stdin);
	buffer[tam] = '\0';

	// mapa das palavras reservadas
	map<string,string> reservadas;
		reservadas["ACENDA"]="ACENDA";
		reservadas["LAMPADA"]="LAMPADA";
		reservadas["APAGUE"]="APAGUE";
		reservadas["LAMPADA"]="LAMPADA";
		reservadas["AGUARDE"]="AGUARDE";
		reservadas["ATE"]="ATE";
		reservadas["COMO"]="COMO";
		reservadas["DEFINAINSTRUCAO"]="DEFINAINSTRUCAO";
		reservadas["DIREITA"]="DIREITA";
		reservadas["ROBO"]="ROBO";
		reservadas["BLOQUEADA"]="BLOQUEADA";
		reservadas["ENQUANTO"]="ENQUANTO";
		reservadas["ENTAO"]="ENTAO";
		reservadas["ESQUERDA"]="ESQUERDA";
		reservadas["EXECUCAOINICIO"]="EXECUCAOINICIO";
		reservadas["FACA"]="FACA";
		reservadas["FIM"]="FIM";
		reservadas["FIMEXECUCAO"]="FIMEXECUCAO";
		reservadas["FIMPARA"]="FIMPARA";
		reservadas["FIMPROGRAMA"]="FIMPROGRAMA";
		reservadas["FIMREPITA"]="FIMREPITA";
		reservadas["FIMSE"]="FIMSE";
		reservadas["FIMSENAO"]="FIMSENAO";
		reservadas["FINALIZE"]="FINALIZE";
		reservadas["FRENTE"]="FRENTE";
		reservadas["INICIO"]="INICIO";
		reservadas["ACESA"]="ACESA";
		reservadas["A"]="A";
		reservadas["APAGADA"]="APAGADA";
		reservadas["MOVA"]="MOVA";
		reservadas["PARE"]="PARE";
		reservadas["PASSO"]="PASSO";
		reservadas["PASSOS"]="PASSOS";
		reservadas["PROGRAMAINICIO"]="PROGRAMAINICIO";
		reservadas["REPITA"]="REPITA";
		reservadas["MOVIMENTANDO"]="MOVIMENTANDO";
		reservadas["OCUPADO"]="OCUPADO";
		reservadas["PARADO"]="PARADO";
		reservadas["PRONTO"]="PRONTO";
		reservadas["SE"]="SE";
		reservadas["SENAO"]="SENAO";
		reservadas["VEZ"]="VEZ";
		reservadas["VEZES"]="VEZES";
		reservadas["VIRE"]="VIRE";
		reservadas["PARA"]="PARA";

	vector<token> tokens = analisadorLexico(buffer,reservadas);

	if (!tokens.empty())
		for (int i = 0; i < tokens.size(); i++)
		cout << tokens[i].texto << " " << tokens[i].tipo << endl;
	else
		cout << "empty" << endl;

    return 0;
}
