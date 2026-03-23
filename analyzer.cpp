#include "analyzer.hpp"

std::vector<std::vector<int>> line_to_parents_id(variable node,std::unordered_map<std::string,int> variable_position,std::vector<variable> var){//funzione per fare il conteggio multibase necessario a collegare la cpt a specifici valori dei genitori
    std::vector<std::vector<int>> line_to_parents_id_map;
    std::vector<int> vec(node.parents.size(),0);//vettore contenente le cordinate del genitore per una certa linea della cpt
    bool step_done=false;
    int j,line_number=1;
    for (size_t i = 0; i < node.parents.size(); i++)
    {
        line_number*=var[variable_position[node.parents[i]]].values.size();
    } 
    for (size_t i = 0; i < line_number; i++)//itero sulle linee della cpt
    {   
        line_to_parents_id_map.push_back(vec);
        step_done=false;
        j=node.parents.size()-1;
        if (i<line_number-1){//stavo facendo un conteggio di troppo
        while (step_done==false)/////////////POSSIBILE CAUSA DEL PROBLEMA j diventa negativooooo
        {
            if (vec[j] < var[variable_position[node.parents[j]]].values.size()-1)//conteggio multibase
            {
                vec[j]++;
                step_done=true;
            }
            else{
                vec[j]=0;
                j--;
            }  
        }
        }
    }
    return line_to_parents_id_map;//l'elemento i esimo del vettore corrisponde alla configurazione dei genitori della linea numero i   
}

void reader(std::vector<variable> var,std::unordered_map<std::string,int> variable_position){//permette di accedere agevolmente alle caratteristiche di un network e continua a runnare il codice fino a comando di chiusura
    bool running = true;
    int node_position;
    while (running)
    {       std::cout<<"==>";
            std::string input,node_name;
            std::cin>>input;
            if (input=="help")
            {
                std::cout<<"==>";
                std::cout<<"comandi disponibili:help,stop,node(name,values,parents,cpt,probability),all(name,values,parents,cpt,probability,number)"<<std::endl;
            }
            else if (input=="stop")
            {
                running= false;
            }
            else if (input=="node")
            {
                std::cout<<"==>";
                std::cout<<"inserire nome del nodo da analizzare"<<std::endl;
                std::cout<<"==>";
                std::cin>>node_name;
                std::cout<<"==>";
                std::cout<<"inserire comando da applicare al nodo"<<std::endl;
                std::cout<<"==>";
                std::cin>>input;
                node_position=variable_position[node_name];
                if (input=="name")
                {
                    std::cout<<"==>"<<node_name<<std::endl;
                }
                else if (input=="values")
                {
                    std::cout<<"==>";
                    for (size_t i = 0; i < var[node_position].values.size(); i++)
                    {
                        std::cout<<"|"<<var[node_position].values[i];
                    }
                    std::cout<<"|"<<std::endl; 
                }
                else if (input=="parents")
                {
                    std::cout<<"==>";
                    if (var[node_position].parents.size()==0)
                    {
                        std::cout<<"il nodo non ha parenti"<<std::endl;
                    }
                    else{
                    for (size_t i = 0; i < var[node_position].parents.size(); i++)
                    {
                        std::cout<<"|"<<var[node_position].parents[i];
                    }
                    std::cout<<"|"<<std::endl; 
                    }   
                }
                else if (input=="probability")
                {
                    std::cout<<"==>";
                    for (size_t i = 0; i < var[node_position].probabilty.size(); i++)
                    {
                        std::cout<<"|"<<var[node_position].probabilty[i];
                    }
                    std::cout<<"|"<<std::endl; 
                }
                else if (input=="cpt")
                {
                        std::cout<<"==>"<<var[node_position].name<<":"<<std::endl;
                        for (size_t j = 0; j < var[node_position].cpt.size(); j++)//scorre tra i vettori della cpt
                        {
                            std::cout<<"==>";
                            for (size_t k = 0; k < var[node_position].cpt[j].size(); k++)//scorre tra gli elementi del vettore della cpt
                            {
                                std::cout<<var[node_position].cpt[j][k]<<",";               
                            }
                            std::cout<<std::endl;
                            
                        }
                }
                else{
                    std::cout<<"==>";
                    std::cout<<"comando '"<<"node."<<input<<"'"<<" sconosciuto, digitare 'help' per una lista dei comandi disponibili"<<std::endl;
                }    
            }
            else if (input=="all")
            {
                std::cout<<"==>";
                std::cout<<"inserire l'informazione di tutti i nodi che si vuole ottenere"<<std::endl;
                std::cout<<"==>";
                std::cin>>input;
                if (input=="name")
                {
                    std::cout<<"==>"<<"i nodi del network sono:";
                    std::cout<<"|";
                    for (size_t i = 0; i < var.size(); i++)
                    {
                        std::cout<<var[i].name<<"|";
                    }
                    std::cout<<std::endl;
                    
                }
                else if (input=="values")
                {
                    for (size_t i = 0; i < var.size(); i++)
                    {   
                        std::cout<<"==>"<< var[i].name + ":";
                        if (var[i].values.size() == 0)
                        {
                            std::cout<<"non ho calcolato la probabilità";
                        }
                        else{
                            for (size_t j = 0; j < var[i].values.size(); j++)
                            {
                                std::cout<< var[i].values[j]<<",";
                            }    
                        }
                        std::cout<<std::endl;
                    }
                }
                else if (input=="parents")
                {
                    for (size_t i = 0; i < var.size(); i++)
                    {   
                        std::cout<<"==>"<< var[i].name + ":";
                        if (var[i].parents.size() == 0)
                        {
                            std::cout<<"non ha parenti";
                        }
                        else{
                            for (size_t j = 0; j < var[i].parents.size(); j++)
                            {
                                std::cout<< var[i].parents[j]<<",";
                            }    
                        }
                        std::cout<<std::endl;
                    }
                }
                else if (input=="cpt")
                {
                    for (size_t i = 0; i < var.size(); i++)//scorre tra gli elementi di var
                    {
                        std::cout<<"==>"<<var[i].name<<":"<<std::endl;
                        for (size_t j = 0; j < var[i].cpt.size(); j++)//scorre tra i vettori della cpt
                        {
                            std::cout<<"==>";
                            for (size_t k = 0; k < var[i].cpt[j].size(); k++)//scorre tra gli elementi del vettore della cpt
                            {
                                std::cout<<var[i].cpt[j][k]<<",";               
                            }
                            std::cout<<std::endl;
                            
                        }
                        std::cout<<std::endl;                        
                    }
                }
                else if(input=="probability")//scrive tutte le pure probability dei nodi
                {
                    for (size_t i = 0; i < var.size(); i++)
                    {   
                        std::cout<<"==>"<< var[i].name + ":";
                        if (var[i].probabilty.size() == 0)
                        {
                            std::cout<<"non ho calcolato la probabilità";
                        }
                        else{
                            for (size_t j = 0; j < var[i].probabilty.size(); j++)
                            {
                                std::cout<< var[i].probabilty[j]<<",";
                            }    
                        }
                        std::cout<<std::endl;
                    }           
                }
                else if (input=="number")
                {
                    std::cout<<"==>";
                    std::cout<<"il network contiene '"<<var.size()<<"' nodi"<<std::endl;                    
                }
                
                else{
                    std::cout<<"==>";
                    std::cout<<"comando '"<<"all."<<input<<"'"<<" sconosciuto, digitare 'help' per una lista dei comandi disponibili"<<std::endl;
                }
            }
            else{
                std::cout<<"==>";
                std::cout<<"comando "<<"'"<<input<<"'"<<" sconosciuto, digitare 'help' per una lista dei comandi disponibili"<<std::endl;
            }
    }
    
}
std::vector<variable> marginalizer(std::vector<variable> var){//funzione che calcola la probabilità marginalizzata di ogni nodo a partire da un vettore di nodi ordinato topologicamente CONTROLLARE SE é GIA' STATO MARGINALIZZATO
    std::unordered_map<std::string,int> variable_position ={};
    double value=0,anchestor_config_probability=1;
    for (size_t i = 0; i < var.size(); i++)//creo un unordered map con chiave il nome della variabile e contenuto la posizione nel vettore dei nodi
    {
        variable_position[var[i].name]=i;
    }
    for (size_t i = 0; i < var.size(); i++){//itero sui nodi del network
        std::vector<std::vector<int>> line_id=line_to_parents_id(var[i],variable_position,var);//possibile bottleneck
        if (var[i].parents.size() == 0){//caso senza parenti
            var[i].probabilty = var[i].cpt[0];
        }
        else{ 
            for (size_t t = 0; t < var[i].values.size(); t++)//itero sui valori che può assumere il nodo
            {
                int line=0;//linea ed elemento della linea di cpt
                for (size_t line = 0; line < var[i].cpt.size(); line++)//itero sulle linee della cpt
                {
                    anchestor_config_probability = 1;
                    for (size_t parent_number = 0; parent_number < var[i].parents.size(); parent_number++)//itero sui parenti
                    {
                        anchestor_config_probability*=var[
                            variable_position[
                                var[i].parents[parent_number]
                            ]].probabilty[line_id[line][parent_number]];
                    }
                    value+=var[i].cpt[line][t]*anchestor_config_probability;
                }
                var[i].probabilty.push_back(value);
                value=0;
            }
        }
    }
    reader(var,variable_position);   
    return var;
}







///////main to test analyzer
int main(){
    std::string filename;
    std::cout<<"==>"<<"inserire il file da analizzare"<<std::endl<<"==>";
    std::cin>>filename;
    std::vector<variable> O_var = marginalizer(node_sort(read_var((filename))));    
    return 0;
}
//aggiungere runtime
//add normalization test