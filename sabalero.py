import random
import math
import numpy as np

def simulated_annealing(initial_state, hosts):
    """Peforms simulated annealing to find a solution"""
    initial_temp = 90
    final_temp = 0
    alpha = 0.01
    
    current_temp = initial_temp

    # Start by initializing the current state with the initial state
    current_state = initial_state.copy()
    solution = current_state.copy()

    while current_temp > final_temp and get_cost(solution) != 0:
        neighbor = random.choice(get_neighbors(current_state, hosts)+get_neighbors2(current_state)).copy() 

        print(get_cost(neighbor), current_temp)
        # Check if neighbor is best so far
        cost_diff = get_cost(current_state) - get_cost(neighbor)
        # if the new solution is better, accept it
        if cost_diff > 0:
            current_state = neighbor.copy()
        # if the new solution is not better, accept it with a probability of e^(-cost/temp)
        else:
            if random.uniform(0, 1) < math.exp(cost_diff / current_temp):
                current_state = neighbor.copy()
        if get_cost(solution) > get_cost(current_state):
            solution = current_state.copy()
        # decrement the temperature
        current_temp -= alpha
    return solution

def diff_cost(solucion):
    costodiff = 0
    for fila in solucion:
        for i in range(len(fila)):
            for j in range(len(fila)):
                if fila[i] == fila[j] and i != j:
                    costodiff += 1
    return costodiff/2

def once_cost(solucion):
    costomeet = 0
    guests = dict()
    for j in range(solucion.shape[1]):
        hosts = dict()
        columna = solucion[:,j]
        i = 0
        for host in columna:
            if host not in hosts:
                hosts[int(host)] = [huespedes[i]]
            else:
                hosts[int(host)] += [huespedes[i]]
            i += 1

        for host in hosts:
            hostaux = host
            for guest in hosts[host]:
                if guest not in guests:
                    guests[guest] = []
                for guestaux in hosts[hostaux]:
                    if guestaux != guest:
                        guests[guest] += [guestaux]
    print(hosts)
    for guest in guests:
        guestset = list(set(guests[guest]))
        costomeet += len(guests[guest]) - len(guestset)
    
    return costomeet/2

def capa_cost(solution):
    capa_cost = 0
    for j in range(solution.shape[1]):
        hosts = dict()
        columna = solution[:,j]
        i = 0
        for host in columna:
            if host not in hosts:
                hosts[int(host)] = [huespedes[i]]
            else:
                hosts[int(host)] += [huespedes[i]]
            i += 1

        for host in hosts:
            cap_rest = instancia["D"][host][0]-instancia["D"][host][1]
            crews_size = 0
            for guest in hosts[host]:
                crews_size += instancia["D"][guest][1]
            sigma = cap_rest - crews_size
            if sigma <= 0:
                sigma = abs(sigma)
            else:
                sigma = 0
            capa_cost += 1 + (sigma-1)/4

    return capa_cost

def get_cost(solution):
    total_cost = 4*diff_cost(solution) + 2*once_cost(solution) + 4*capa_cost(solution)   
    return total_cost

def get_neighbors(solution, hosts):
    neighbors = list()
    for i in range(solution.shape[0]):
        for j in range(solution.shape[1]):
            for host in hosts:
                solutionaux = solution.copy()
                if host != solution[i,j]:
                    solutionaux[i,j] = host
                    neighbors.append(solutionaux)
    return neighbors

def get_neighbors2(solution):
    neighbors = list()
    for i in range(solution.shape[1]):
        columna = solution[:,i].copy()
        for j in range(len(columna)):
            solucion_aux = solution.copy()
            for k in range(j,len(columna)):
                if(k != j):
                    aux = columna[j].copy()
                    columna[j] = columna[k].copy()
                    columna[k] = aux.copy()
                    solucion_aux[:,i] = columna.copy()
                    neighbors.append(solucion_aux.copy())
                columna = solution[:,i].copy()

    return neighbors

def best_neighbor(neighbors):
    solution = np.zeros(neighbors[0].shape)
    min_cost = float("inf")

    for neighbor in neighbors:
        cost = get_cost(neighbor)
        if cost < min_cost:
            min_cost = cost
            solution = neighbor.copy()
    return solution

archivo = open("./Configuraciones/PPP.txt")
instancia = dict()
instancia["D"] = dict()
for linea in archivo:
    dato = linea.strip().split(';')
    if "Y" not in instancia:
        instancia["Y"] = int(dato[0])
    elif "T" not in instancia:
        instancia["T"] = int(dato[0])
    else:
        i = 1   
        for yate in dato:
            yate = yate.split(',')
            instancia["D"][i] = [int(yate[0]), int(yate[1])]
            i += 1
archivo.close()

yates = instancia["D"]
archivo2 = open("./Configuraciones/config1.txt")
anfitriones = list()
for linea in archivo2:
    dato = linea.strip().split()
    for anfitrion in dato:
        anfitriones.append(int(anfitrion))

huespedes = [item for item in yates.keys() if item not in anfitriones]
solucion_i = np.zeros((len(huespedes), instancia["T"]))
for i in range(len(huespedes)):
    for j in range(instancia["T"]):
        solucion_i[i][j] = np.random.choice(list(anfitriones))


solucion_i = np.array([[10, 10, 12,  1,  8,  7],
 [ 8,  3, 10,  8,  5,  1],
 [ 3, 12,  4,  3, 12,  2],
 [ 9,  8, 11,  6, 10, 16],
 [ 2,  6, 12, 16,  1,  4],
 [ 2,  4,  4,  7,  9,  6],
 [ 2,  8,  3, 10, 16, 12],
 [10, 10, 12,  1,  7, 10],
 [ 2,  4,  5, 11,  3,  4],
 [ 5,  2, 12, 16,  5, 12],
 [11,  7, 10,  8, 11,  4],
 [ 1,  7,  7,  3,  5, 11],
 [10, 10, 16,  1,  3, 16],
 [ 7,  7, 16,  7, 11, 10],
 [ 9, 11, 10,  7,  4, 11],
 [ 8,  6,  5,  3, 10, 10],
 [12,  9,  4, 12,  7,  9],
 [ 6,  1, 12,  6, 12,  7],
 [11,  7,  9,  6,  7,  4],
 [ 2, 10,  3,  1,  6,  4],
 [10, 16, 16,  9, 11,  3],
 [ 7,  6, 12,  9,  3,  6],
 [ 8,  1, 12, 10, 11, 10],
 [ 5,  1, 12,  3,  4,  8],
 [ 5,  3,  4,  2, 10,  6],
 [ 2,  4,  7, 16,  1, 10],
 [ 4, 11,  3, 11, 12,  6],
 [16,  6,  1,  4, 12, 16],
 [ 2,  9, 12,  2,  3,  9]])
print(get_cost(solucion_i))


