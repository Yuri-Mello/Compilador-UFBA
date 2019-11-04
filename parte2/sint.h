#include "lexico.h"
#include <utility>
#include <string>
#include <stack>

using namespace std;

void analisadorSintatico(vector<token> tokens) {
	stack<string> pilha;

	// tabela LL(1) (INCOMPLETA)
	map<pair<string,string>, string> tabelaSint;
	tabelaSint[{"PROGRAMA","PROGRAMAINICIO"}] = "PROGRAMAINICIO D EXECUCAOINICIO COMANDO FIMEXECUCAO FIMPROGRAMA";
	tabelaSint[{"D","EXECUCAOINICIO"}] = "";
	tabelaSint[{"D","DEFINAINSTRUCAO"}] = "DECLARACAO D";
	tabelaSint[{"DECLARACAO","DEFINAINSTRUCAO"}] = "DEFINAINSTRUCAO ID COMO COMANDO";
}