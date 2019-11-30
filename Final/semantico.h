#include "sintatico.h"

using namespace std;

bool erroSem = false;

void erroSemantico(int linha, int coluna) {
	cout << "ERRO: erro semântico na linha " << linha << ", coluna " << coluna << ".\n";
	erroSem = true;
}

vector<string> analisadorSemantico(vector<token> tokens) {

	// REGRA 1 - Duas declarações de instruções não podem ter o mesmo nome
	vector<string> identificadores; // vector dos identificadores
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].tipo == "DEFINAINSTRUCAO") {
			string ident = tokens[i + 1].texto;
			for (int j = 0; j < identificadores.size(); j++) {
				if (identificadores[j] == ident) {
					erroSemantico(tokens[i + 1].linha,tokens[i + 1].coluna);
					return {};
				}
			}

			identificadores.push_back(ident);
		}
	}

	// REGRA 2 - Declarações "Vire Para" imediatamente subsequentes não podem ter sentidos diferentes
	string sentido = ""; // sentido prévio, vazio se não ocorreu declaração "vire para"
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].tipo == "VIRE") {
			i += 2;
			if (sentido == "")
				sentido = tokens[i].tipo;
			else if (sentido != tokens[i].tipo) {
				erroSemantico(tokens[i].linha,tokens[i].coluna);
				return {};
			}
		}
		else
			sentido = ""; // caso declaração não seja do tipo "vire para", resete o sentido
	}

	// REGRA 3 - Instrução "Mova n [passos]" deve ser sucedida por "Aguarde Até Robo Pronto"
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].tipo == "MOVA") {
			i += 2; if (tokens[i].tipo == "PASSOS" || tokens[i].tipo == "PASSO") i++;
			string pronto = tokens[i].tipo + tokens[i + 1].tipo + tokens[i + 2].tipo + tokens[i + 3].tipo;
			if (pronto != "AGUARDEATEROBOPRONTO") {
				erroSemantico(tokens[i].linha,tokens[i].coluna);
				return {};
			}
		}

	}

	return identificadores;
}