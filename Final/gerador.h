#include "semantico.h"

using namespace std;

int pos = 0;
int funcs = 0;

string condicionais(vector<token> tokens) {
	string codigo = "";
	
	return codigo;
}

string comando(vector<token> tokens) {
	string codigo = "";
	
	if (tokens[pos].tipo == "MOVA") {
		pos++;
		string num = tokens[pos].texto;
		string comeco = "_mover" + to_string(funcs);
		string fim = "_mover_fim" + to_string(funcs);
		funcs++;
		codigo += "\nmov bl, " + num + "\n";
		codigo += comeco + ":\n";
		codigo += "cmp bl, 0\n";
		codigo += "je " + fim + "\n";
		codigo += "push bl\n";
		codigo += "call aguarde_ate_pronto\n";
		codigo += "call mover_frente\n";
		codigo += "pop bl\n";
		codigo += "sub bl, 1\n";
		codigo += "jmp " + comeco + "\n";
		codigo += fim + ":\n";

		if (tokens[pos + 1].tipo == "PASSOS" || tokens[pos + 1].tipo == "PASSO")
			pos++;
	}
	else if (tokens[pos].tipo == "VIRE") {
		pos += 2;
		if (tokens[pos].tipo == "ESQUERDA")
			codigo += "\ncall virar_esquerda\n";
		else if (tokens[pos].tipo == "DIREITA")
			codigo += "\ncall virar_direita\n";
	}
	else if (tokens[pos].tipo == "ACENDA") {
		codigo += "\ncall acender_lampada\n";
		pos++;
	}
	else if (tokens[pos].tipo == "APAGUE") {
		codigo += "\ncall apagar_lampada\n";
		pos++;
	}
	else if (tokens[pos].tipo == "REPITA") {
		pos++;
		string num = tokens[pos].texto;
		string comeco = "_repita" + to_string(funcs);
		string fim = "_repita_fim" + to_string(funcs);
		funcs++;
		codigo += "\nmov bl, " + num + "\n";
		codigo += comeco + ":\n";
		codigo += "cmp bl, 0\n";
		codigo += "je " + fim + "\n";
		codigo += "push bl\n";
		while (tokens[pos].tipo != "FIMREPITA") {
			codigo += comando(tokens);
			pos++;
		}
		codigo += "\npop bl\n";
		codigo += "sub bl, 1\n";
		codigo += "jmp " + comeco + "\n";
		codigo += fim + ":\n";
	}
	else if (tokens[pos].tipo == "FINALIZE") {
		codigo += "ret\n";
	}

	return codigo;
}
string gerarCodigo(vector<token> tokens) {
	string codigo = "\n#start=robot.exe#\n"; // começa programa do robo
	codigo += "\nname \"robot\"\n"; // nome
	codigo += "\n#make_bin#\n#cs = 500#\n#ds = 500#\n#ss = 500#\n#sp = ffff#\n#ip = 0#\n"; // bin

	codigo += "\njmp cod_main\n";

	codigo += "\nexaminar_byte_0 proc\n";
	codigo += "busy0:\n";
	codigo += "in al, 11\n";
	codigo += "test al, 00000001b\n";
	codigo += "jz busy0\n";
	codigo += "ret\n";
	codigo += "examinar_byte_0 endp\n";

	codigo += "\naguarde_ate_pronto proc\n";
	codigo += "busy1:\n";
	codigo += "in al, 11\n";
	codigo += "test al, 00000010b\n";
	codigo += "jnz busy1\n";
	codigo += "ret\n";
	codigo += "aguarde_ate_pronto endp\n";

	codigo += "\nmover_frente proc\n";
	codigo += "mov al, 1\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "mover_frente endp\n";

	codigo += "\nvirar_esquerda proc\n";
	codigo += "mov al, 2\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "virar_esquerda endp\n";

	codigo += "\nvirar_direita proc\n";
	codigo += "mov al, 3\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "virar_direita endp\n";

	codigo += "\nexaminar_objeto proc\n";
	codigo += "mov al, 4\n";
	codigo += "out 9, al\n";
	codigo += "call examinar_byte_0 \n";
	codigo += "ret\n";
	codigo += "examinar_objeto endp\n";

	codigo += "\nacender_lampada proc\n";
	codigo += "mov al, 5\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "acender_lampada endp\n";

	codigo += "\napagar_lampada proc\n";
	codigo += "mov al, 6\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "apagar_lampada endp\n";

	codigo += "\ncod_main:\n";

	pos = 0;
	while(tokens[pos].tipo != "EXECUCAOINICIO") {
		pos++;
	}
	while(tokens[pos].tipo != "FIMEXECUCAO") {
		codigo += comando(tokens);
		pos++;
	}

	codigo += "\nret\n";

	return codigo;
}