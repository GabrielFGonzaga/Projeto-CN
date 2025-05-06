#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int MAX_DIGITOS = 30;
const int PRECISAO_FRACIONARIA = 10;

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

double converter_para_double(const string& numero, int base) {
    size_t ponto = numero.find('.');
    string parteInt;
    string parteFrac;
    if (ponto != string::npos) {
        parteInt = numero.substr(0, ponto);
        parteFrac = numero.substr(ponto + 1);
    } else {
        parteInt = numero;
        parteFrac = "";
    }

    double valor = 0.0;
    for (size_t i = 0; i < parteInt.size(); i++) {
        valor = valor * base + char_para_digito(parteInt[i]);
    }

    double fator = 1.0;
    for (size_t i = 0; i < parteFrac.size(); i++) {
        fator = fator / base;
        valor = valor + char_para_digito(parteFrac[i]) * fator;
    }

    return valor;
}

string converter_de_double(double valor, int base) {
    long long parteInt = static_cast<long long>(valor);
    double parteFrac = valor - parteInt;

    string resultadoInt;
    if (parteInt == 0) {
        resultadoInt = "0";
    } else {
        while (parteInt > 0) {
            int resto = parteInt % base;
            resultadoInt = digito_para_char(resto) + resultadoInt;
            parteInt = parteInt / base;
        }
    }

    if (parteFrac <= 0.0) {
        return resultadoInt;
    }

    string resultado = resultadoInt + ".";
    int count = 0;
    while (count < PRECISAO_FRACIONARIA && parteFrac > 0.0) {
        parteFrac = parteFrac * base;
        int dig = static_cast<int>(parteFrac);
        resultado.push_back(digito_para_char(dig));
        parteFrac = parteFrac - dig;
        count = count + 1;
    }

    return resultado;
}

string converter_base(string numero, int baseOrigem, int baseDestino) {
    bool temPonto = (numero.find('.') != string::npos);
    if (temPonto) {
        double valor = converter_para_double(numero, baseOrigem);
        return converter_de_double(valor, baseDestino);
    }

    if (numero.size() == 1 && numero[0] == '0') {
        return "0";
    }

    int tam = numero.size();
    int digitos[MAX_DIGITOS];
    for (int i = 0; i < tam; i++) {
        digitos[i] = char_para_digito(numero[i]);
    }

    string resultado;
    while (tam > 0) {
        int resto = 0;
        int novoTam = 0;
        int novosDigitos[MAX_DIGITOS];

        for (int i = 0; i < tam; i++) {
            int valorCalc = resto * baseOrigem + digitos[i];
            resto = valorCalc % baseDestino;
            int quociente = valorCalc / baseDestino;
            if (novoTam > 0 || quociente > 0) {
                novosDigitos[novoTam] = quociente;
                novoTam = novoTam + 1;
            }
        }

        resultado = digito_para_char(resto) + resultado;

        for (int i = 0; i < novoTam; i++) {
            digitos[i] = novosDigitos[i];
        }
        tam = novoTam;
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

string adicao_em_bases(string n1, int base1, string n2, int base2, int baseDestino) {
    bool ponto1 = (n1.find('.') != string::npos);
    bool ponto2 = (n2.find('.') != string::npos);

    if (ponto1 || ponto2) {
        double v1 = converter_para_double(n1, base1);
        double v2 = converter_para_double(n2, base2);
        return converter_de_double(v1 + v2, baseDestino);
    }

    string s1 = converter_base(n1, base1, baseDestino);
    string s2 = converter_base(n2, base2, baseDestino);

    int i = s1.size() - 1;
    int j = s2.size() - 1;
    int carry = 0;
    string resultado;

    while (i >= 0 || j >= 0 || carry > 0) {
        int soma = carry;
        if (i >= 0) {
            soma = soma + char_para_digito(s1[i]);
            i = i - 1;
        }
        if (j >= 0) {
            soma = soma + char_para_digito(s2[j]);
            j = j - 1;
        }
        int dig = soma % baseDestino;
        resultado = digito_para_char(dig) + resultado;
        carry = soma / baseDestino;
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

string multiplicacao_em_bases(string n1, int base1, string n2, int base2, int baseDestino) {
    bool ponto1 = (n1.find('.') != string::npos);
    bool ponto2 = (n2.find('.') != string::npos);

    if (ponto1 || ponto2) {
        double v1 = converter_para_double(n1, base1);
        double v2 = converter_para_double(n2, base2);
        return converter_de_double(v1 * v2, baseDestino);
    }

    string s1 = converter_base(n1, base1, baseDestino);
    string s2 = converter_base(n2, base2, baseDestino);

    string resultado = "0";
    int tam2 = s2.size();

    for (int j = tam2 - 1; j >= 0; j--) {
        int d2 = char_para_digito(s2[j]);
        int carry = 0;
        string parcial;

        for (int i = s1.size() - 1; i >= 0; i--) {
            int prod = char_para_digito(s1[i]) * d2 + carry;
            int dig = prod % baseDestino;
            parcial = digito_para_char(dig) + parcial;
            carry = prod / baseDestino;
        }

        if (carry > 0) {
            parcial = digito_para_char(carry) + parcial;
        }
        for (int k = 0; k < tam2 - 1 - j; k++) {
            parcial.push_back('0');
        }

        resultado = adicao_em_bases(resultado, baseDestino, parcial, baseDestino, baseDestino);
    }

    while (resultado.size() > 1 && resultado[0] == '0') {
        resultado.erase(0, 1);
    }
    return resultado;
}

int main() {
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    cout << "| Calculadora de base                                                                                    |" << endl;
    cout << "|                                                                                                         |" << endl;
    cout << "| Se deseja converter a base de um numero digite 1 caso deseje realizar uma operacao digite 2:            |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;

    cout << "| Opcao: ";
    string opcao;
    getline(cin, opcao);
    cout << "|" << endl;

    if (opcao == "1") {
        string numero;
        int baseOrigem;
        int baseDestino;
        cout << "| Digite o numero para ser convertido: ";
        getline(cin, numero);
        if (numero.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base de origem: ";
        cin >> baseOrigem;
        cout << "| Base de destino: ";
        cin >> baseDestino;
        if (baseOrigem < 2) {
            
        } else {
            
        }
        
        if (baseOrigem < 2 || baseDestino < 2 || baseOrigem > 36 || baseDestino > 36) {
            cout << "| Bases devem estar entre 2 e 36" << endl;
            return 1;
        }

        string resultado = converter_base(numero, baseOrigem, baseDestino);
        cout << "| Resultado: " << resultado << " na base " << baseDestino << endl;
    } else if (opcao == "2") {
        string n1;
        string n2;
        int b1;
        int b2;
        int bd;
        cout << "| Primeiro numero: ";
        getline(cin, n1);
        if (n1.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base do primeiro numero: ";
        cin >> b1;
        cin.ignore();
        cout << "| Segundo numero: ";
        getline(cin, n2);
        if (n2.size() > MAX_DIGITOS) {
            cout << "| O numero tem que ter menos de 30 digitos" << endl;
            return 1;
        }
        cout << "| Base do segundo numero: ";
        cin >> b2;
        cout << "| Base de destino para o resultado: ";
        cin >> bd;
        if (b1 < 2 || b2 < 2 || bd < 2 || b1 > 36 || b2 > 36 || bd > 36) {
            cout << "| Bases devem estar entre 2 e 36" << endl;
            return 1;
        }

        cout << "|" << endl;
        string operacao;
        while (operacao != "soma" && operacao != "multiplicacao") {
            cout << "| Escolha a operacao (soma, multiplicacao): ";
            cin >> operacao;
        }

        string resultado;
        if (operacao == "soma") {
            resultado = adicao_em_bases(n1, b1, n2, b2, bd);
            cout << "| A soma da " << resultado << " na base " << bd << endl;
        } else {
            resultado = multiplicacao_em_bases(n1, b1, n2, b2, bd);
            cout << "| A multiplicacao da " << resultado << " na base " << bd << endl;
        }
    } else {
        cout << "| Escolha 1 ou 2" << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    return 0;
}
