#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

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
    ifstream archivo("./Configuraciones/PPP.txt");
    map<char, int> instancia;
    map<int, vector<int>> yates;
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
    cout << instancia['Y'] << endl;
    cout << instancia['T'] << endl;
    cout << to_string(yates[1][0]) << endl;

    return 0;
}