#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <time.h>
#include <cmath>


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
                if (solucion[i][j] == solucion[i][k] && j != k){
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
    int ncolumna = solucion[0].size();

    for (int j = 0; j < ncolumna; j++){
        map<int, vector<int>> hosts;
        vector<int> columna;
        for (int unsigned long i = 0; i < solucion.size(); i++){
            columna.push_back(solucion[i][j]);
        }
        int i = 0;
        for (int unsigned long h = 0; h < columna.size(); h++){
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

        for ( int unsigned long i = 1; i <= hosts.size(); i++ )
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
                    for ( int unsigned long g = 0; g < hosts[hostaux].size(); g++){
                        int guestaux = hosts[hostaux][g];
                        if (guestaux != guest){
                            guests[guest].push_back(guestaux);
                        } 
                    }
                }
            }
        }
    }

    for(int unsigned long g = 1; g <= guests.size(); g++){
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
    int ncolumna = solucion[0].size();

    for (int j = 0; j < ncolumna; j++){
        map<int, vector<int>> hosts;
        vector<int> columna;
        for (int unsigned long i = 0; i < solucion.size(); i++){
            columna.push_back(solucion[i][j]);
        }
        int i = 0;
        for (int unsigned long h = 0; h < columna.size(); h++){
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

        for (int unsigned long host = 1; host <= hosts.size(); host++){
            if (hosts[host].size() != 0){
                int cap_rest = yates[host][0] - yates[host][1];
                int crews_size = 0;
                for(int unsigned long guest = 0; guest < hosts[host].size(); guest++){
                    crews_size += yates[hosts[host][guest]][1]; 
                }
                float sigma = cap_rest - crews_size;
                if (sigma < 0){
                    sigma = sigma * (-1);
                    costocapa += 1 + (sigma-1)/4;
                }
            }
        }
 
    }

    return costocapa;
}

float get_cost(vector<vector<int>> solucion, map<int, vector<int>> yates, vector<int> huespedes){
    float total_cost = 4*diff_cost(solucion) + 2*once_cost(solucion, huespedes) + 4*capa_cost(solucion, yates, huespedes);
    return total_cost;
}

vector<vector<vector<int>>> get_neighbors(vector<vector<int>> solucion, vector<int> hosts){
    vector<vector<vector<int>>> neighbors;

    for (int unsigned long i = 0; i < solucion.size(); i++){
        for (int unsigned long j = 0; j < solucion[0].size(); j++){
            for (int unsigned long h = 0; h < hosts.size(); h++){
                int host = hosts[h];
                vector<vector<int>> solucionaux = solucion;
                if (host != solucion[i][j]){
                    solucionaux[i][j] = host;
                    neighbors.push_back(solucionaux);
                }
            }
        }
    }
    return neighbors;
}

vector<vector<vector<int>>> get_neighbors2(vector<vector<int>> solucion){
    vector<vector<vector<int>>> neighbors;
    for (int unsigned long i = 0; i < solucion[0].size(); i++){
        vector<int> columna;
        vector<int> columnai;
        for (int unsigned long c = 0; c < solucion.size(); c++){
            columna.push_back(solucion[c][i]);
        }
        columnai = columna;
        for (int unsigned long j = 0; j < columna.size(); j++){
            vector<vector<int>> solucion_aux = solucion;
            for (int unsigned long k = j; k < columna.size(); k++){
                if(k != j){
                    int aux = columna[j];
                    columna[j] = columna[k];
                    columna[k] = aux;
                    for (int unsigned long c = 0; c < solucion.size(); c++){
                        solucion_aux[c][i] = columna[c];
                    }
                    neighbors.push_back(solucion_aux);
                columna = columnai;
                }
            }
        }
    }
    return neighbors;
}

vector<vector<int>> get_neighbors3(vector<vector<int>> solucion, vector<int> hosts){
    vector<vector<int>> neighbor;

    int randi = rand() % solucion.size();
    int randj = rand() % solucion[0].size();
    int randh = rand() % hosts.size();

    vector<vector<int>> solucionaux = solucion;
    solucionaux[randi][randj] = hosts[randh];
    int randc = rand() % solucion.size();
    while (randc == randi){
        randc = rand() % solucion.size();
    }
    int aux = solucionaux[randi][randj];
    solucionaux[randi][randj] = solucionaux[randc][randj];
    solucionaux[randc][randj] = aux;
    neighbor = solucionaux;

    return neighbor;
}

vector<vector<int>> best_neighbor(vector<vector<vector<int>>> neighbors, vector<vector<int>> state, map<int, vector<int>> yates, vector<int> huespedes){
    vector<vector<int>> solucion = state;
    float min_cost = get_cost(solucion, yates, huespedes);

    for (int unsigned long i = 0; i<neighbors.size(); i++){
        vector<vector<int>> neighbor = neighbors[i];
        float cost = get_cost(neighbor, yates, huespedes);
        if (cost < min_cost){
            min_cost = cost;
            solucion = neighbor;
            i = neighbors.size();
        }
    }
    return solucion;
}

vector<vector<int>> simulated_annealing(vector<vector<int>> initial_state, map<int, vector<int>> yates, vector<int> hosts, vector<int> guests, string config, int times){
    float initial_temp = 50;
    float final_temp = 0;
    float alpha = 0.01;
    int iter_recal = 1;
    int max_iter = 10;

    float current_temp = initial_temp;

    vector<vector<int>> current_state = initial_state;
    vector<vector<int>> solution = current_state;
    while (iter_recal <= max_iter && get_cost(solution, yates, guests) != 0){

        current_temp = initial_temp;
     
        vector<vector<int>> current_state = initial_state;
        
        vector<vector<int>> current_solution = current_state;
        while (current_temp > final_temp && get_cost(solution, yates, guests) != 0){

            vector<vector<int>> neighbor = get_neighbors3(current_state, hosts);

            if (current_temp < 1){
                vector<vector<vector<int>>> neighbors = get_neighbors(current_state, hosts);
                vector<vector<vector<int>>> neighbors2 = get_neighbors2(current_state);
                neighbors.insert( neighbors.end(), neighbors2.begin(), neighbors2.end() );
                neighbor = best_neighbor(neighbors, current_state, yates, guests);
                if (get_cost(neighbor, yates, guests) == get_cost(current_state, yates, guests)){
                    current_temp = 0;
                }
            }   
            cout << iter_recal << " ";
            cout << get_cost(solution, yates, guests) << " ";
            cout << get_cost(neighbor, yates, guests) << " "; 
            cout << current_temp << endl;
            float cost_diff = get_cost(current_state, yates, guests) - get_cost(neighbor, yates, guests);
            if (cost_diff > 0){
                current_state = neighbor;
            }
            else{
                if ((float) rand() / RAND_MAX  < pow(2.7, cost_diff / current_temp)){
                    current_state = neighbor;
                }
            }
            if (get_cost(current_solution, yates, guests) > get_cost(current_state, yates, guests)){
                current_solution = current_state;
                if (get_cost(solution, yates, guests) > get_cost(current_solution, yates, guests)){
                    solution = current_solution;
                }
            }
            current_state = current_solution;
            current_temp -= alpha;
        }
        iter_recal++;
    }

    string periodos = to_string(times);
    string respuesta;
    string instancia;
    fstream resultados;

    if (get_cost(solution, yates, guests) == 0){
        respuesta = "Si";
    }
    else{
        respuesta = "No";
    }

    resultados.open("./Resultados/resultados"+config+".txt", std::ios_base::app);
    resultados << "======================================================" << std::endl;
    resultados << "Periodos: " + periodos << std::endl;
    resultados << "¿Solucion?: " + respuesta +  ", Iteracion: " + to_string(iter_recal-1) + ", Costo: " + to_string(get_cost(solution, yates, guests))<< std::endl;
    resultados << "======================================================" << std::endl;
    resultados << std::endl;
    
    

    for ( std::vector<std::vector<int>>::size_type i = 0; i < solution.size(); i++ )
    {
        for ( std::vector<int>::size_type j = 0; j < solution[i].size(); j++ )
        {
            std::cout << solution[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    for ( std::vector<std::vector<int>>::size_type i = 0; i < solution.size(); i++ )
    {
        for ( std::vector<int>::size_type j = 0; j < solution[i].size(); j++ )
        {
            resultados << solution[i][j] << ' ';
        }
        resultados << std::endl;
    }
    resultados << std::endl;
    resultados << std::endl;
    resultados << std::endl;

    resultados.close();
    return solution;
}

int main(int argc, char *argv[])
{
    int randindex;
    srand(time(0));
    
    string line;
    string yatesaux;
    string anfitrionesaux;
    ifstream archivo("./Configuraciones/PPP.txt");
    ifstream archivo2(argv[2]);
    string configuracion = argv[1]; 
    map<char, int> instancia;
    map<int, vector<int>> yates;
    vector<int> yateskeys;
    vector<int> anfitriones;
    vector<int> huespedes;
    vector<int> solaux;
    vector<vector<int>> solucion_i;
    vector<vector<int>> solucion;
    

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

    for (int unsigned long i = 0; i < huespedes.size(); i++){
        solaux = {};
        for (int j = 0; j < instancia['T']; j++){
            randindex = rand() % anfitriones.size();
            solaux.push_back(anfitriones[randindex]);
        }
        solucion_i.push_back(solaux);
    }
    
    simulated_annealing(solucion_i, yates, anfitriones, huespedes, configuracion, instancia['T']);

    return 0;
}