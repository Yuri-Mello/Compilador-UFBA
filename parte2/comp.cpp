// #include <iostream>
// #include <fstream>
// #include <stdio.h>
// #include <string>
// #include <vector>
// #include <map>
// #include <cctype>
#include "sint.h"

using namespace std;

int main() {
	FILE *program;
	char buffer[512000]; // tamanho máximo do programa = 512kb
	char filename[512]; // tamanho máximo do filename = 512b

	cout << "Digite o nome do arquivo a ser lido: ";
	cin >> filename;

	fstream teste;
	teste.open(filename);
	if (teste.fail()) {
		cout << "Erro: arquivo não encontrado" << endl;
		return -1;
	}

	program = fopen(filename,"r");
	if (fopen == NULL) {
		cout << "Erro: não foi possível ler o arquivo." << endl;
		return -1;
	}
	int tam = fread(buffer,sizeof(char),511999,program);
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

	if (!tokens.empty()) {
		for (int i = 0; i < tokens.size(); i++) {
			cout << tokens[i].texto << " " << tokens[i].tipo << endl;
		}
	}
	else if (erro == false) {
		cout << "Erro: arquivo não contém código." << endl;
	}

    return 0;
}