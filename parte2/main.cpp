#include "semantico.h"
#include <fstream>
#include <stdio.h>
#include <cctype>

using namespace std;

int main() {
	FILE *program;
	char buffer[512000]; // tamanho máximo do programa = 512kb
	char filename[512]; // tamanho máximo do filename = 512b

	cout << "Digite o nome do arquivo a ser lido: ";
	cin >> filename;

	fstream arquivo;
	arquivo.open(filename);
	if (arquivo.fail()) {
		cout << "ERRO: arquivo não encontrado" << endl;
		return -1;
	}

	program = fopen(filename,"r");
	if (fopen == NULL) {
		cout << "ERRO: não foi possível ler o arquivo." << endl;
		return -1;
	}
	int tam = fread(buffer,sizeof(char),511999,program);
	buffer[tam] = '\0';

	// análise léxica
	cout << "Executando análise léxica... ";
	vector<token> tokens = analisadorLexico(buffer);

	if (!tokens.empty())
		cout << "OK." << endl;
	else {
		if (erroLex == false)
			cout << "ERRO: arquivo não contém código." << endl;
		return -1;
	}

	// análise sintática
	cout << "Executando análise sintática... ";
	erroSint = analisadorSintatico(tokens);
	erroSint = !erroSint; // analisadorSintatico retorna "true" se não teve erros, entao trocamos o valor aqui

	if (!erroSint)
		cout << "OK." << endl;
	else
		return -1;

	// análise semântica
	cout << "Executando análise semântica... ";
	vector<string> identificadores = analisadorSemantico(tokens);

	if (!erroSem)
		cout << "OK." << endl;
	else
		return -1;

    return 0;
}
