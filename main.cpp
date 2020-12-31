#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>



using namespace std;

vector<int> splitaux(string s, string delimiter)
{
    vector<int> lista;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        lista.push_back(stoi(token));
        s.erase(0, pos + delimiter.length());
    }
    lista.push_back(stoi(s));
    return lista;
}

map<int, vector<int>> split(string s, string delimiter)
{
    map<int, vector<int>> yates;
    vector<int> aux;
    size_t pos = 0;
    string token;
    int i = 1;
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        aux = splitaux(token, ",");
        yates[i] = aux;
        s.erase(0, pos + delimiter.length());
        i++;
    }
    aux = splitaux(s, ",");
    yates[i] = aux;

    return yates;
}

int main()
{
    string line;
    string yatesaux;
    string anfitrionesaux;
    ifstream archivo("./Configuraciones/PPP.txt");
    ifstream archivo2("./Configuraciones/config1.txt");
    map<char, int> instancia;
    map<int, vector<int>> yates;
    vector<int> yateskeys;
    vector<int> anfitriones;
    vector<int> huespedes;

    if (archivo.is_open())
    {
        int i = 0;
        while (getline(archivo, line, '\n'))
        {
            if (i == 0)
            {
                instancia['Y'] = stoi(line);
            }
            else if (i == 1)
            {
                instancia['T'] = stoi(line);
            }
            else
            {
                yatesaux = line;
            }
            i++;
        }
        archivo.close();
    }
    else
        std::cout << "Unable to open file\n";

    yates = split(yatesaux, ";");

    for( int i = 1; i <= instancia['Y']; i++) {
        yateskeys.push_back(i);
    }

    if (archivo2.is_open())
    {
        int i = 0;
        while (getline(archivo2, line, '\n'))
        {
            anfitrionesaux = line;
        }
        archivo.close();
    }
    else
        std::cout << "Unable to open file\n";

    anfitriones = splitaux(anfitrionesaux, " ");

    std::set_difference(yateskeys.begin(), yateskeys.end(), anfitriones.begin(), anfitriones.end(),
        std::inserter(huespedes, huespedes.begin()));

    return 0;
}