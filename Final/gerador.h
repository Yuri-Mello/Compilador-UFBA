#include "semantico.h"

using namespace std;

int pos = 0; // posicao no vector de tokens
int funcs = 0;

string condicionais(vector<token> tokens) {
	string codigo = "";

  	if (tokens[pos].tipo == "FRENTE") {
		pos += 2;
		codigo += "\ncall examinar_objeto\n";
		codigo += "in al, 10\n";
		codigo += "cmp al, 255\n";
		codigo += "jne ";
	}
	else if (tokens[pos].tipo == "DIREITA") {
		pos += 2;
		codigo += "\ncall examinar_objeto\n";
		codigo += "in al, 10\n";
		codigo += "cmp al, 15\n";
		codigo += "jne ";
	}
	else if (tokens[pos].tipo == "ESQUERDA") {
		pos += 2;
		codigo += "\ncall examinar_objeto\n";
		codigo += "in al, 10\n";
		codigo += "cmp al, 240\n";
		codigo += "jne ";
	}
	else if (tokens[pos].tipo == "LAMPADA") {
		pos++;
		codigo += "\ncall examinar_objeto\n";
		codigo += "in al, 10\n";
		codigo += "cmp al, ";
		if (tokens[pos].tipo == "ACESA") {
			pos += 2;
			if (tokens[pos].tipo == "DIREITA") {
				codigo += "11\n";
				codigo += "jne ";
			}
			else if (tokens[pos].tipo == "ESQUERDA") {
				codigo += "9\n";
				codigo += "jne ";
			}
			else if (tokens[pos].tipo == "FRENTE") {
				codigo += "7\n";
				codigo += "jne ";
			}
 		}
		else if (tokens[pos].tipo == "APAGADA") {
			pos += 2;
			if (tokens[pos].tipo == "DIREITA") {
				codigo += "12\n";
				codigo += "jne ";
			}
			else if (tokens[pos].tipo == "ESQUERDA") {
				codigo += "10\n";
				codigo += "jne ";
			}
			else if (tokens[pos].tipo == "FRENTE") {
				codigo += "8\n";
				codigo += "jne ";
			}
		}
	}
	else if (tokens[pos].tipo == "ROBO") {
		pos++;
		codigo += "in al, 11\n";
		codigo += "test al, 00000010b\n";
		if (tokens[pos].tipo == "PRONTO") {
			codigo += "jnz ";
		}
		else if (tokens[pos].tipo == "OCUPADO") {
			codigo += "jz ";
		}
	}
	
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
		codigo += "call aguarde_ate_pronto\n";
		codigo += "call mover_frente\n";
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
		codigo += "push bx\n";
		while (tokens[pos].tipo != "FIMREPITA") {
			codigo += comando(tokens);
			pos++;
		}
		codigo += "\npop bx\n";
		codigo += "sub bl, 1\n";
		codigo += "jmp " + comeco + "\n";
		codigo += fim + ":\n";
	}
	else if (tokens[pos].tipo == "ENQUANTO") {
		pos++;
		string comeco = "_enquanto" + to_string(funcs);
		string fim = "_enquanto_fim" + to_string(funcs);
		funcs++;
		codigo += "\n" + comeco + ":\n";
		codigo += condicionais(tokens) + fim + "\n";
		while (tokens[pos].tipo != "FIMPARA") {
			pos++;
			codigo += comando(tokens);
		}
		codigo += "jmp " + comeco + "\n";
		codigo += fim + ":\n";
	}
	else if (tokens[pos].tipo == "SE") {
		pos++;
		string second = "_se_second" + to_string(funcs);
		funcs++;
		codigo += "\n" + condicionais(tokens) + second + "\n";
		while (tokens[pos].tipo != "FIMSE") {
			pos++;
			codigo += comando(tokens);
		}
		codigo += second + ":\n";
		pos++;
		if (tokens[pos].tipo == "SENAO") {
			while (tokens[pos].tipo != "FIMSENAO") {
				pos++;
				codigo += comando(tokens);
			}
		}
		else pos--;
	}
	else if (tokens[pos].tipo == "AGUARDE") {
		pos += 2;
		string comeco = "_aguarde" + to_string(funcs);
		string fim = "_aguarde_fim" + to_string(funcs);
		funcs++;
		codigo += "\n" + comeco + ":\n";
		codigo += condicionais(tokens) + comeco + "\n";
		codigo += fim + ":\n";
	}
	else if (tokens[pos].tipo == "ID") {
		codigo += "\ncall __" + tokens[pos].texto + "\n";
	}
	else if (tokens[pos].tipo == "PARE") {
		codigo += "\ncall fazer_nada\n";
		pos++;
	}
	else if (tokens[pos].tipo == "FINALIZE") {
		codigo += "ret\n";
	}

	return codigo;
}
string gerarCodigo(vector<token> tokens) {
	pos = 0;
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

	codigo += "\nfazer_nada proc\n";
	codigo += "mov al, 0\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "fazer_nada endp\n";

	codigo += "\nmover_frente proc\n";
	codigo += "mov al, 1\n";
	codigo += "out 9, al\n";
	codigo += "ret\n";
	codigo += "mover_frente endp\n";

	codigo += "\nvirar_esquerda proc\n";
	codigo += "mov al, 2\n";
	codigo += "out 9, al\n";
	codigo += "call aguarde_ate_pronto\n";
	codigo += "ret\n";
	codigo += "virar_esquerda endp\n";

	codigo += "\nvirar_direita proc\n";
	codigo += "mov al, 3\n";
	codigo += "out 9, al\n";
	codigo += "call aguarde_ate_pronto\n";
	codigo += "ret\n";
	codigo += "virar_direita endp\n";

	codigo += "\nexaminar_objeto proc\n";
	codigo += "mov al, 4\n";
	codigo += "out 9, al\n";
	codigo += "call examinar_byte_0\n";
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

	while(tokens[pos].tipo != "EXECUCAOINICIO") {
		if (tokens[pos].tipo == "DEFINAINSTRUCAO") {
			pos++;
			string funcao = "__" + tokens[pos].texto;
			codigo += "\n" + funcao + " proc\n";
			pos++;
			while (tokens[pos].tipo != "DEFINAINSTRUCAO" && tokens[pos].tipo != "EXECUCAOINICIO") {
				codigo += comando(tokens);
				pos++;
			}
			pos--;
			codigo += "ret\n";
			codigo += funcao + " endp\n";
		}
		pos++;
	}

	codigo += "\ncod_main:\n";

	while(tokens[pos].tipo != "FIMEXECUCAO") {
		codigo += comando(tokens);
		pos++;
	}

	codigo += "\nret\n";

	return codigo;
}
