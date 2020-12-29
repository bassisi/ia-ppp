import random
import math
import numpy as np

def simulated_annealing(initial_state):
    """Peforms simulated annealing to find a solution"""
    initial_temp = 90
    final_temp = .1
    alpha = 0.01
    
    current_temp = initial_temp

    # Start by initializing the current state with the initial state
    current_state = initial_state
    solution = current_state

    while current_temp > final_temp:
        neighbor = random.choice(get_neighbors())

        # Check if neighbor is best so far
        cost_diff = get_cost(current_state) - get_cost(neighbor)

        # if the new solution is better, accept it
        if cost_diff > 0:
            solution = neighbor
        # if the new solution is not better, accept it with a probability of e^(-cost/temp)
        else:
            if random.uniform(0, 1) < math.exp(cost_diff / current_temp):
                solution = neighbor
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

def meet_cost(solucion):
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
    
    for guest in guests:
        guestset = list(set(guests[guest]))
        costomeet += len(guests[guest]) - len(guestset)
    
    return costomeet/2
    

def get_cost(state):
    """Calculates cost of the argument state for your solution."""
    raise NotImplementedError
    
def get_neighbors(state):
    """Returns neighbors of the argument state for your solution."""
    raise NotImplementedError

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

print(solucion_i)

print(solucion_i[:,0])

print(meet_cost(solucion_i))


