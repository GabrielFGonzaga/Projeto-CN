#include <iostream>
#include <string>

using namespace std;

const int MAX_DIGITOS = 30;

int char_para_digito(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return 0;
}

char digito_para_char(int d) {
    if (d < 10) {
        return '0' + d;
    }
    return 'A' + (d - 10);
}

string converter_base(string numero, int base_origem, int base_destino) {
    if (base_origem == base_destino) {
        return numero;
    }
    if (numero == "0") {
        return "0";
    }

    int tam = numero.size();
    if (tam > MAX_DIGITOS) {
        return "Erro: Numero excede o limite de 30 digitos";
    }

    int digitos[MAX_DIGITOS];
    for (int i = 0; i < tam; i++) {
        digitos[i] = char_para_digito(numero[i]);
    }

    string resultado = "";
    while (tam > 0) {
        int resto = 0;
        int novo_tam = 0;
        int novos_digitos[MAX_DIGITOS];

        for (int i = 0; i < tam; i++) {
            int valor = resto * base_origem + digitos[i];
            resto = valor % base_destino;
            int quociente = valor / base_destino;
            if (novo_tam > 0 || quociente > 0) {
                novos_digitos[novo_tam] = quociente;
                novo_tam++;
            }
        }
        resultado = digito_para_char(resto) + resultado;

        for (int i = 0; i < novo_tam; i++) {
            digitos[i] = novos_digitos[i];
        }
        tam = novo_tam;
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

string adicao_em_bases(string n1, int base1, string n2, int base2, int base_destino) {
    string n1_dest = converter_base(n1, base1, base_destino);
    string n2_dest = converter_base(n2, base2, base_destino);

    string resultado = "";
    int carry = 0;
    int i = n1_dest.size() - 1;
    int j = n2_dest.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int soma = carry;
        if (i >= 0) soma += char_para_digito(n1_dest[i]);
        if (j >= 0) soma += char_para_digito(n2_dest[j]);

        int digito = soma % base_destino;
        carry = soma / base_destino;

        resultado = digito_para_char(digito) + resultado;

        i--; j--;
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

string multiplicacao_em_bases(string n1, int base1, string n2, int base2, int base_destino) {
    string n1_dest = converter_base(n1, base1, base_destino);
    string n2_dest = converter_base(n2, base2, base_destino);

    string resultado = "0";
    int tam_n2 = n2_dest.size();

    for (int j = tam_n2 - 1; j >= 0; j--) {
        string parcial = "";
        int carry = 0;
        int d2 = char_para_digito(n2_dest[j]);

        for (int i = n1_dest.size() - 1; i >= 0; i--) {
            int d1 = char_para_digito(n1_dest[i]);
            int produto = d1 * d2 + carry;
            int digito = produto % base_destino;
            carry = produto / base_destino;
            parcial = digito_para_char(digito) + parcial;
        }
        if (carry > 0) {
            parcial = digito_para_char(carry) + parcial;
        }

        for (int k = 0; k < tam_n2 - 1 - j; k++) {
            parcial += "0";
        }

        resultado = adicao_em_bases(resultado, base_destino, parcial, base_destino, base_destino);
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

int main() {
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    cout << "| Calculadora de bases                                                                                    |" << endl;
    cout << "|                                                                                                         |" << endl;
    cout << "| Se deseja converter a base de um numero digite 1 caso deseje realizar uma operacao digite 2:            |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;

    cout << "| Opcao: ";
    string opcao;
    getline(cin, opcao);
    cout << "|" << endl;

    if (opcao == "1") {
        string numero;
        int base_origem, base_destino;
        cout << "| Digite o numero para ser convertido: ";
        getline(cin, numero);
        if (numero.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base de origem: ";
        cin >> base_origem;
        cout << "| Base de destino: ";
        cin >> base_destino;

        if (base_origem < 2 || base_destino < 2 || base_origem > 36 || base_destino > 36) {
            cout << "| Bases devem estar entre 2 e 36" << endl;
            return 1;
        }

        string resultado = converter_base(numero, base_origem, base_destino);
        cout << "| Resultado: " << resultado << " na base " << base_destino << endl;
    } else if (opcao == "2") {
        string numero1, numero2;
        int base1, base2, base_destino;
        cout << "| Primeiro numero: ";
        getline(cin, numero1);
        if (numero1.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base do primeiro numero: ";
        cin >> base1;
        cin.ignore();
        cout << "| Segundo numero: ";
        getline(cin, numero2);
        if (numero2.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base do segundo numero: ";
        cin >> base2;
        cout << "| Base de destino para o resultado: ";
        cin >> base_destino;

        if (base1 < 2 || base2 < 2 || base_destino < 2 ||
            base1 > 36 || base2 > 36 || base_destino > 36) {
            cout << "| Bases devem estar entre 2 e 36" << endl;
            return 1;
        }

        cout << "|" << endl;
        string operacao;
        while(operacao != "soma" && operacao != "multiplicacao"){
            cout << "| Escolha a operacao (soma, multiplicacao): ";
            cin >> operacao;
        }

        string resultado;
        if (operacao == "soma") {
            resultado = adicao_em_bases(numero1, base1, numero2, base2, base_destino);
            cout << "| A soma da " << resultado<< " na base " << base_destino << endl;
        }
        else if (operacao == "multiplicacao") {
            resultado = multiplicacao_em_bases(numero1, base1, numero2, base2, base_destino);
            cout << "| A multiplicacao da " << resultado<< " na base " << base_destino << endl;
        }

    } else {
        cout << "| Escolha 1 ou 2" << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------" << endl;

    return 0;
}
