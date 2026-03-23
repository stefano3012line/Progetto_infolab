#include "builder.hpp"
std::vector<variable> node_sort(std::vector<variable> var){//algoritmo per il sorting topologico dei nodi
    std::vector<variable> O_var;
    std::vector<std::string> ban_list;
    int indegree;
    while (!var.empty())
    {    
        for (int i=0; i< var.size();i++)
        {
            indegree=0;
                for (int j=0;  j <var[i].parents.size();j++)//controllo l'indegree del vettore non ancora sortato
                {
                    bool banned=false;
                    for (int k=0; k<ban_list.size();k++)//controllo se il nodo è gia stato sortato
                    {
                        if (var[i].parents[j] == ban_list[k])
                        {
                            banned=true;
                            break;
                        }
                    }
                    if (!banned)
                    {
                        indegree++;
                    }      
                }                
                if (indegree==0)
                {
                    O_var.push_back(var[i]);
                    ban_list.push_back(var[i].name);
                    var.erase(var.begin()+i);
                    break;
                }        
        }
    }
    return O_var;
}
