//#include "parser.hpp"
#include "builder.hpp"


#ifndef analyzer_hpp
#define analyzer_hpp

void reader(std::vector<variable> var);


std::vector<variable> marginalizer(std::vector<variable> var);//funzione che calcola la probabilità marginalizzata di ogni nodo a partire da un vettore di nodi ordinato topologicamente
std::vector<std::vector<int>> line_to_parents_id(variable node);//converte le linee della cpt di un nodo negli id dei suoi parenti

//Per ogni variabile X, calcolare la probabilità marginale P(X=x) per ciascun valore x
//Nota: Verificare che la somma delle probabilità marginali di ciascuna variabile sia 1 (entro una tolleranza numerica)
//per ora i nodi sono sortati topologicamente e devo trovare il modo per calcolare le probabilità 
#endif
