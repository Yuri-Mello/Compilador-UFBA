/*
LINGUAGEM UTILIZADA:

PROGRAMA -> programainicio D execucaoinicio COMANDO fimexecucao fimprograma
D -> DECLARACAO D
| *vazio*
DECLARACAO -> definainstrucao identificador como COMANDO
COMANDO -> BLOCO
| ITERACAO
| LACO
| CONDICIONAL
| INSTRUCAO
BLOCO -> inicio C fim
C -> COMANDO C
| *vazio*
ITERACAO -> repita numero vezes COMANDO fimrepita
LACO -> enquanto CONDICAO faca COMANDO fimpara
CONDICIONAL -> se CONDICAO entao COMANDO fimse ELSE
ELSE -> senao COMANDO fimsenao
| *vazio*
INSTRUCAO -> mova numero PASSOS
PASSOS -> passo
| passos
| *vazio*
INSTRUCAO -> vire para SENTIDO
| identificador
| pare
| finalize
| apague lampada
| acenda lampada
| aguarde ate CONDICAO
CONDICAO -> frente robo bloqueada
| direita robo bloqueada
| esquerda robo bloqueada
| robo STATUS
| lampada ACAP
STATUS -> pronto
| ocupado
| parado
| movimentando
ACAP -> acesa a DIRECAO
| apagada a DIRECAO
DIRECAO -> esquerda
| direita
| frente
SENTIDO -> esquerda
| direita

*/
#include "lexico.h"
#include <algorithm>
#include <utility>
#include <stack>

using namespace std;

bool erroSint = false;

stack<string> addInstToStack(stack<string> pilha, string inst) {
	if (inst != "&") {
		int tam = inst.length();
		string texto = "";

		for(int i = tam - 1; i >= 0; i--) {
			if (inst[i] == ' ') {
				reverse(texto.begin(),texto.end());
				pilha.push(texto);
				texto = "";
			}
			else
				texto += inst[i];
		}
		reverse(texto.begin(),texto.end());
		pilha.push(texto);
	}

	return pilha;
}

void erroSintatico(int linha, int coluna) {
	cout << "ERRO: erro sintático na linha " << linha << ", coluna " << coluna << ".\n";
}

bool analisadorSintatico(vector<token> tokens) {
	if (tokens.empty()) {
		cout << "Erro na função \"analisadorSintatico\" em \"sint.h\": vector de entrada vazio." << endl;
		return false;
	}

	// tabela LL(1)
	map<pair<string,string>, string> tabelaSint;
	
	// -PROGRAMA- (inicial)
	tabelaSint[{"PROGRAMA","PROGRAMAINICIO"}] = "PROGRAMAINICIO D EXECUCAOINICIO COMANDO FIMEXECUCAO FIMPROGRAMA";

	// -D-
	tabelaSint[{"D","EXECUCAOINICIO"}] = "&"; // o caráter '&' é utilizado como substituto da produção vazia (epsilon)
	tabelaSint[{"D","DEFINAINSTRUCAO"}] = "DECLARACAO D";

	// -DECLARACAO-
	tabelaSint[{"DECLARACAO","DEFINAINSTRUCAO"}] = "DEFINAINSTRUCAO ID COMO COMANDO";

	// -COMANDO-
	tabelaSint[{"COMANDO","AGUARDE"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","ACENDA"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","APAGUE"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","FINALIZE"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","PARE"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","ID"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","VIRE"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","MOVA"}] = "INSTRUCAO";
	tabelaSint[{"COMANDO","SE"}] = "CONDICIONAL";
	tabelaSint[{"COMANDO","ENQUANTO"}] = "LACO";
	tabelaSint[{"COMANDO","REPITA"}] = "ITERACAO";
	tabelaSint[{"COMANDO","INICIO"}] = "BLOCO";

	// -C-
	tabelaSint[{"C","AGUARDE"}] = "COMANDO C";
	tabelaSint[{"C","ACENDA"}] = "COMANDO C";
	tabelaSint[{"C","APAGUE"}] = "COMANDO C";
	tabelaSint[{"C","FINALIZE"}] = "COMANDO C";
	tabelaSint[{"C","PARE"}] = "COMANDO C";
	tabelaSint[{"C","ID"}] = "COMANDO C";
	tabelaSint[{"C","VIRE"}] = "COMANDO C";
	tabelaSint[{"C","MOVA"}] = "COMANDO C";
	tabelaSint[{"C","SE"}] = "COMANDO C";
	tabelaSint[{"C","ENQUANTO"}] = "COMANDO C";
	tabelaSint[{"C","REPITA"}] = "COMANDO C";
	tabelaSint[{"C","INICIO"}] = "COMANDO C";
	tabelaSint[{"C","FIM"}] = "&";

	// -BLOCO-
	tabelaSint[{"BLOCO","INICIO"}] = "INICIO C FIM";

	// -ITERACAO-
	tabelaSint[{"ITERACAO","REPITA"}] = "REPITA NUM VEZES COMANDO FIMREPITA" ;  

	// -LACO-
	tabelaSint[{"LACO","ENQUANTO"}] = "ENQUANTO CONDICAO FACA COMANDO FIMPARA";

	// -CONDICAO-
	tabelaSint[{"CONDICAO","SE"}] = "SE CONDICAO ENTAO COMANDO FIMSE ELSE";

	// -ELSE-
	tabelaSint[{"ELSE","AGUARDE"}] = "&";
	tabelaSint[{"ELSE","ACENDA"}] = "&";
	tabelaSint[{"ELSE","APAGUE"}] = "&";
	tabelaSint[{"ELSE","FINALIZE"}] = "&";
	tabelaSint[{"ELSE","PARE"}] = "&";
	tabelaSint[{"ELSE","ID"}] = "&";
	tabelaSint[{"ELSE","VIRE"}] = "&";
	tabelaSint[{"ELSE","MOVA"}] = "&";
	tabelaSint[{"ELSE","FIMSENAO"}] = "&";
	tabelaSint[{"ELSE","SENAO"}] = "SENAO COMANDO FIMSENAO";
	tabelaSint[{"ELSE","FIMSE"}] = "&";
	tabelaSint[{"ELSE","SE"}] = "&";
	tabelaSint[{"ELSE","FIMPARA"}] = "&";
	tabelaSint[{"ELSE","ENQUANTO"}] = "&";
	tabelaSint[{"ELSE","ENQUANTO"}] = "&";
	tabelaSint[{"ELSE","FIMREPITA"}] = "&";
	tabelaSint[{"ELSE","REPITA"}] = "&";
	tabelaSint[{"ELSE","FIM"}] = "&";
	tabelaSint[{"ELSE","INICIO"}] = "&";
	tabelaSint[{"ELSE","DEFINAINSTRUCAO"}] = "&";
	tabelaSint[{"ELSE","FIMEXECUCAO"}] = "&";
	tabelaSint[{"ELSE","EXECUCAOINICIO"}] = "&";

	// -INSTRUCAO-
	tabelaSint[{"INSTRUCAO","AGUARDE"}] = "AGUARDE ATE CONDICAO";
	tabelaSint[{"INSTRUCAO","ACENDA"}] = "ACENDA LAMPADA";
	tabelaSint[{"INSTRUCAO","APAGUE"}] = "APAGUE LAMPADA";
	tabelaSint[{"INSTRUCAO","FINALIZE"}] = "FINALIZE";
	tabelaSint[{"INSTRUCAO","PARE"}] = "PARE";
	tabelaSint[{"INSTRUCAO","ID"}] = "ID";
	tabelaSint[{"INSTRUCAO","VIRE"}] = "VIRE PARA SENTIDO";
	tabelaSint[{"INSTRUCAO","MOVA"}] = "MOVA NUM PASSOS-";

	// -PASSOS-- (adicionei o '-' pois estava em conflito com a palavra reservada "PASSOS")
	tabelaSint[{"PASSOS-","PASSOS"}] = "PASSOS";
	tabelaSint[{"PASSOS-","PASSO"}] = "PASSO";
	tabelaSint[{"PASSOS-","AGUARDE"}] = "&";
	tabelaSint[{"PASSOS-","ACENDA"}] = "&";
	tabelaSint[{"PASSOS-","APAGUE"}] = "&";
	tabelaSint[{"PASSOS-","FINALIZE"}] = "&";
	tabelaSint[{"PASSOS-","PARE"}] = "&";
	tabelaSint[{"PASSOS-","ID"}] = "&";
	tabelaSint[{"PASSOS-","VIRE"}] = "&";
	tabelaSint[{"PASSOS-","MOVA"}] = "&";
	tabelaSint[{"PASSOS-","MOVA"}] = "&";
	tabelaSint[{"PASSOS-","FIMSENAO"}] = "&";
	tabelaSint[{"PASSOS-","FIMSE"}] = "&";
	tabelaSint[{"PASSOS-","SE"}] = "&";
	tabelaSint[{"PASSOS-","FIMPARA"}] = "&";
	tabelaSint[{"PASSOS-","ENQUANTO"}] = "&";
	tabelaSint[{"PASSOS-","ENQUANTO"}] = "&";
	tabelaSint[{"PASSOS-","FIMREPITA"}] = "&";
	tabelaSint[{"PASSOS-","REPITA"}] = "&";
	tabelaSint[{"PASSOS-","FIM"}] = "&";
	tabelaSint[{"PASSOS-","INICIO"}] = "&";
	tabelaSint[{"PASSOS-","DEFINAINSTRUCAO"}] = "&";
	tabelaSint[{"PASSOS-","FIMEXECUCAO"}] = "&";
	tabelaSint[{"PASSOS-","EXECUCAOINICIO"}] = "&";

	// -CONDICAO-
	tabelaSint[{"CONDICAO","DIREITA"}] = "DIREITA ROBO BLOQUEADA";
	tabelaSint[{"CONDICAO","ESQUERDA"}] = "ESQUERDA ROBO BLOQUEADA";
	tabelaSint[{"CONDICAO","FRENTE"}] = "FRENTE ROBO BLOQUEADA";
	tabelaSint[{"CONDICAO","LAMPADA"}] = "LAMPADA ACAP";
	tabelaSint[{"CONDICAO","ROBO"}] = "ROBO STATUS";

	// -STATUS-
	tabelaSint[{"STATUS","MOVIMENTANDO"}] = "MOVIMENTANDO";
	tabelaSint[{"STATUS","PARADO"}] = "PARADO";
	tabelaSint[{"STATUS","OCUPADO"}] = "OCUPADO";
	tabelaSint[{"STATUS","PRONTO"}] = "PRONTO";

	// -ACAP- (ACesa ou APagada)
	tabelaSint[{"ACAP","APAGADA"}] = "APAGADA A DIRECAO";
	tabelaSint[{"ACAP","ACESA"}] = "ACESA A DIRECAO";

	// -DIRECAO- (sentido + "frente")
	tabelaSint[{"DIRECAO","DIREITA"}] = "DIREITA";
	tabelaSint[{"DIRECAO","ESQUERDA"}] = "ESQUERDA";
	tabelaSint[{"DIRECAO","FRENTE"}] = "FRENTE";

	// -SENTIDO-
	tabelaSint[{"SENTIDO","DIREITA"}] = "DIREITA";
	tabelaSint[{"SENTIDO","ESQUERDA"}] = "ESQUERDA";

	// mapa dos simbolos possiveis
	map<string,string> simbolos;
		simbolos["PROGRAMA"] = "PROGRAMA";
		simbolos["D"] = "D";
		simbolos["DECLARACAO"] = "DECLARACAO";
		simbolos["COMANDO"] = "COMANDO";
		simbolos["C"] = "C";
		simbolos["BLOCO"] = "BLOCO";
		simbolos["ITERACAO"] = "ITERACAO";
		simbolos["LACO"] = "LACO";
		simbolos["CONDICIONAL"] = "CONDICIONAL";
		simbolos["ELSE"] = "ELSE";
		simbolos["INSTRUCAO"] = "INSTRUCAO";
		simbolos["PASSOS-"] = "PASSOS-";
		simbolos["CONDICAO"] = "CODICAO";
		simbolos["STATUS"] = "STATUS";
		simbolos["ACAP"] = "ACAP";
		simbolos["DIRECAO"] = "DIRECAO";
		simbolos["SENTIDO"] = "SENTIDO";

	// pilha
	stack<string> pilha;
	pilha.push("$");
	pilha.push("PROGRAMA"); // "PROGRAMA" é o símbolo de partida
	
	tokens.push_back(novoToken("$","$",0,0));

	int pos = 0; // onde está apontando no vector de tokens
 	string topPilha = pilha.top(); // topo da pila
	do {
		topPilha = pilha.top();
		
		if (!simbolos.count(topPilha) || topPilha == "$") { // topo da pilha é um terminal ou $
			if (topPilha == tokens[pos].tipo) {
				pilha.pop();
				pos++;
			}
			else {
				erroSintatico(tokens[pos].linha,tokens[pos].coluna);
				return false;
			}
		}
		else { // topo da pilha é um não-terminal
			string inst = tabelaSint[{topPilha,tokens[pos].tipo}];
			if (inst != "") {
				pilha.pop();
				pilha = addInstToStack(pilha,inst);
			}
			else {
				erroSintatico(tokens[pos].linha,tokens[pos].coluna);
				return false;
			}
		}
	} while (topPilha != "$");

	return true;
}
