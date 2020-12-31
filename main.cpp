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

float diff_cost(vector<vector<int>> solucion){
    float costodiff = 0;
    int nfila = solucion.size();
    int ncolumna = solucion[0].size();

    for (int i = 0; i < nfila; i++){
        for (int j = 0; j < ncolumna; j++){
            for (int k = 0; k < ncolumna; k++){
                if (solucion[i][j] == solucion[i][k] and j != k){
                    costodiff += 1;
                }
            }
        }
    }
    return costodiff/2;
}

float once_cost(vector<vector<int>> solucion, vector<int> huespedes){
    float costomeet = 0;
    map<int, vector<int>> guests;
    int nfila = solucion.size();
    int ncolumna = solucion[0].size();

    for (int j = 0; j < ncolumna; j++){
        map<int, vector<int>> hosts;
        vector<int> columna;
        for (int i = 0; i < solucion.size(); i++){
            columna.push_back(solucion[i][j]);
        }
        int i = 0;
        for (int h = 0; h < columna.size(); h++){
            map<int,vector<int>>::iterator it;
            it = hosts.find(columna[h]);
            if (it != hosts.end()){
                hosts[columna[h]].push_back(huespedes[i]);
            }
            else{
                hosts[columna[h]] = {huespedes[i]};
            }
            i += 1;
        }

        for ( int i = 1; i <= hosts.size(); i++ )
        {
            if (hosts[i].size() != 0){
                int hostaux = i;
                for ( std::vector<int>::size_type j = 0; j < hosts[i].size(); j++ )
                {
                    int guest = hosts[i][j];
                    map<int,vector<int>>::iterator it;
                    it = guests.find(guest);
                    if (it == guests.end()){
                        guests[guest] = {};
                    }
                    for ( int g = 0; g < hosts[hostaux].size(); g++){
                        int guestaux = hosts[hostaux][g];
                        if (guestaux != guest){
                            guests[guest].push_back(guestaux);
                        } 
                    }
                }
            }
        }
    }

    for(int g = 1; g <= guests.size(); g++){
        vector<int> guest = guests[g];
        if (guest.size() != 0){
            vector<int> guestset = guest;
            sort( guestset.begin(), guestset.end() );
            guestset.erase( unique( guestset.begin(), guestset.end() ), guestset.end() );
            costomeet += guest.size() - guestset.size();
        }
    }

    return costomeet/2;
}

float capa_cost(vector<vector<int>> solucion, map<int, vector<int>> yates,vector<int> huespedes){
    float costocapa = 0;
    int nfila = solucion.size();
    int ncolumna = solucion[0].size();

    for (int j = 0; j < ncolumna; j++){
        map<int, vector<int>> hosts;
        vector<int> columna;
        for (int i = 0; i < solucion.size(); i++){
            columna.push_back(solucion[i][j]);
        }
        int i = 0;
        for (int h = 0; h < columna.size(); h++){
            map<int,vector<int>>::iterator it;
            it = hosts.find(columna[h]);
            if (it != hosts.end()){
                hosts[columna[h]].push_back(huespedes[i]);
            }
            else{
                hosts[columna[h]] = {huespedes[i]};
            }
            i += 1;
        }

        for (int host = 1; host <= hosts.size(); host++){
            if (hosts[host].size() != 0){
                int cap_rest = yates[host][0] - yates[host][1];
                int crews_size = 0;
                for(int guest = 0; guest < hosts[host].size(); guest++){
                    crews_size += yates[hosts[host][guest]][1]; 
                }
                float sigma = cap_rest - crews_size;
                if (sigma <= 0){
                    sigma = sigma * (-1);
                }
                else {
                    sigma = 0;
                }
                costocapa += 1 + (sigma-1)/4;
            }
        }
 
    }

    return costocapa;
}

float get_cost(vector<vector<int>> solucion, map<int, vector<int>> yates, vector<int> huespedes){
    float total_cost = 4*diff_cost(solucion) + 2*once_cost(solucion, huespedes) + 4*capa_cost(solucion, yates, huespedes)   ;
    return total_cost;
}

int main()
{
    int randindex;
    srand(time(0));
 
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
    vector<int> solaux;
    vector<vector<int>> solucion_i;

    

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

    for (int i = 0; i < huespedes.size(); i++){
        solaux = {};
        for (int j = 0; j < instancia['T']; j++){
            randindex = rand() % anfitriones.size();
            solaux.push_back(anfitriones[randindex]);
        }
        solucion_i.push_back(solaux);
    }
/*
    for ( std::vector<std::vector<int>>::size_type i = 0; i < solucion_i.size(); i++ )
    {
        for ( std::vector<int>::size_type j = 0; j < solucion_i[i].size(); j++ )
        {
            std::cout << solucion_i[i][j] << ' ';
        }
        std::cout << std::endl;
    }
*/
    solucion_i = {{10, 10, 12,  1,  8,  7},
 { 8,  3, 10,  8,  5,  1},
 { 3, 12,  4,  3, 12,  2},
 { 9,  8, 11,  6, 10, 16},
 { 2,  6, 12, 16,  1,  4},
 { 2,  4,  4,  7,  9,  6},
 { 2,  8,  3, 10, 16, 12},
 {10, 10, 12,  1,  7, 10},
 { 2,  4,  5, 11,  3,  4},
 { 5,  2, 12, 16,  5, 12},
 {11,  7, 10,  8, 11,  4},
 { 1,  7,  7,  3,  5, 11},
 {10, 10, 16,  1,  3, 16},
 { 7,  7, 16,  7, 11, 10},
 { 9, 11, 10,  7,  4, 11},
 { 8,  6,  5,  3, 10, 10},
 {12,  9,  4, 12,  7,  9},
 { 6,  1, 12,  6, 12,  7},
 {11,  7,  9,  6,  7,  4},
 { 2, 10,  3,  1,  6,  4},
 {10, 16, 16,  9, 11,  3},
 { 7,  6, 12,  9,  3,  6},
 { 8,  1, 12, 10, 11, 10},
 { 5,  1, 12,  3,  4,  8},
 { 5,  3,  4,  2, 10,  6},
 { 2,  4,  7, 16,  1, 10},
 { 4, 11,  3, 11, 12,  6},
 {16,  6,  1,  4, 12, 16},
 { 2,  9, 12,  2,  3,  9}};
    cout << to_string(get_cost(solucion_i, yates, huespedes)) << endl;
    return 0;
}