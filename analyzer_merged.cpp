#include "analyzer.hpp"

std::vector<std::vector<int>> line_to_parents_id(std::vector<std::string> node_parents, std::unordered_map<std::string,int> variable_position, std::vector<variable> var){
    std::vector<std::vector<int>> line_to_parents_id_map;
    std::vector<int> vec(node_parents.size(), 0);
    bool step_done = false;
    int j, line_number = 1;
    for (size_t i = 0; i < node_parents.size(); i++)
        line_number *= var[variable_position[node_parents[i]]].values.size();

    for (size_t i = 0; i < line_number; i++){
        line_to_parents_id_map.push_back(vec);
        step_done = false;
        j = node_parents.size() - 1;
        if (i < line_number - 1){
            while (step_done == false && j >= 0){
                if (vec[j] < var[variable_position[node_parents[j]]].values.size() - 1){
                    vec[j]++;
                    step_done = true;
                } else {
                    vec[j] = 0;
                    j--;
                }
            }
        }
    }
    return line_to_parents_id_map;
}


std::vector<double> conditional_probability(variable node, std::vector<int> parent_configuration, std::unordered_map<std::string,int> variable_position, std::vector<variable> var){
    int line_number = 0, parent_value = 0;
    for (size_t i = 0; i < node.parents.size(); i++){
        parent_value = parent_configuration[i];
        for (size_t j = i + 1; j < node.parents.size(); j++)
            parent_value *= var[variable_position[node.parents[j]]].values.size();
        line_number += parent_value;
    }
    return node.cpt[line_number];
}








void reader(std::vector<variable> var,std::unordered_map<std::string,int> variable_position){//permette di accedere agevolmente alle caratteristiche di un network e continua a runnare il codice fino a comando di chiusura
    bool running = true;
    int node_position;
    double total_probability=0;
    while (running)
    { 
            std::cout<<"==>";
            std::string input,node_name;
            std::cin>>input;
            if (input=="help")
            {
                std::cout<<"==>";
                std::cout<<"comandi disponibili:help,stop,node(name,values,parents,cpt,probability,conditional),all(name,values,parents,cpt,probability,number)"<<std::endl;
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
                node_position=variable_position[node_name];//posizione del nodo nel vettore dei nodi
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
                    total_probability=0;
                    std::cout<<"==>";
                    for (size_t i = 0; i < var[node_position].probabilty.size(); i++)
                    {
                        std::cout<<"|"<<var[node_position].probabilty[i];
                        total_probability+=var[node_position].probabilty[i];
                    }
                    std::cout<<"| "<<"somma:"<<total_probability<<std::endl;
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
                ////////////////////////////comando per calcolare la probabilità condizionata di un nodo dato una certa configurazione dei suoi parenti
                else if (input=="conditional")
                {
                    bool valid_configuration=false;
                    std::vector<int> parent_configuration;
                    std::string parent_value;
                    for (size_t i = 0; i < var[node_position].parents.size(); i++)
                    {
                        valid_configuration=false;
                        while (!valid_configuration)//chiedo di inserire la configurazione dei genitori finché non è valida
                        {
                            std::cout<<"==>"<<"inserire valore del genitore '"<<var[node_position].parents[i]<<"'"<<std::endl;
                            std::cout<<"==>";
                            std::cin>>parent_value;
                            for (size_t j = 0; j < var[variable_position[var[node_position].parents[i]]].values.size(); j++)//controllo che il valore inserito sia valido altrimenti chiedo di riinserirlo
                            {
                                if (parent_value == var[variable_position[var[node_position].parents[i]]].values[j])//itero sui possibili valori del genitore j
                                {
                                    parent_configuration.push_back(j);
                                    valid_configuration=true;
                                    break;
                                }
                            }
                            if(!valid_configuration)
                            {
                                std::cout<<"==>"<<"valore del genitore '"<<var[node_position].parents[i]<<"' inserito non valido, riprovare"<<std::endl;
                            }
                        }
                    }
                    std::vector<double> result=conditional_probability(var[node_position],parent_configuration,variable_position,var);
                    std::cout<<"==>"<<"la probabilita' condizionata di '"<<var[node_position].name<<"' e':"<<std::endl;
                    for (size_t i = 0; i < result.size(); i++)
                    {
                        std::cout<<"|"<<result[i];
                    }
                    std::cout<<"|"<<std::endl; 
                }
                ///////////////////////////////////////////////////
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
                        total_probability=0;
                        std::cout<<"==>"<< var[i].name + ":";
                        if (var[i].probabilty.size() == 0)
                        {
                            std::cout<<"non ho calcolato la probabilita'";
                        }
                        else{
                            for (size_t j = 0; j < var[i].probabilty.size(); j++)
                            {
                                std::cout<< var[i].probabilty[j]<<",";
                                total_probability+=var[i].probabilty[j];
                            }    
                        }
                        std::cout<<" somma:"<<total_probability<<std::endl;
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





std::vector<variable> marginalizer(std::vector<variable> var){
    std::unordered_map<std::string,int> variable_position = {};
    for (size_t i = 0; i < var.size(); i++)
        variable_position[var[i].name] = i;

    for (size_t i = 0; i < var.size(); i++){

        if (var[i].parents.size() == 0){//nel caso di nodi senza parenti la probabilità è data direttamente dalla cpt
            
            var[i].probabilty = var[i].cpt[0];

        } else {

            //backpropagation per trovare i nodi necessari al calcolo della probabilità marginale del nodo i
            std::vector<std::string> necessary_nodes,
                                    previously_added_necessary_nodes,
                                    just_added_necessary_nodes,
                                    to_be_added_necessary_nodes;
            bool added_node = false;

            for (size_t j = 0; j < var[i].parents.size(); j++)
                to_be_added_necessary_nodes.push_back(var[variable_position[var[i].parents[j]]].name);

            for (size_t z = 0; z < to_be_added_necessary_nodes.size(); z++){
                if (std::count(necessary_nodes.begin(), necessary_nodes.end(), to_be_added_necessary_nodes[z]) == 0){
                    necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                    added_node = true;
                }
            }
            previously_added_necessary_nodes = to_be_added_necessary_nodes;
            to_be_added_necessary_nodes.clear();

            while (added_node){
                added_node = false;
                for (size_t k = 0; k < previously_added_necessary_nodes.size(); k++){
                    for (size_t j = 0; j < var[variable_position[previously_added_necessary_nodes[k]]].parents.size(); j++)
                        to_be_added_necessary_nodes.push_back(
                            var[variable_position[var[variable_position[previously_added_necessary_nodes[k]]].parents[j]]].name
                        );
                    for (size_t z = 0; z < to_be_added_necessary_nodes.size(); z++){
                        if (std::count(necessary_nodes.begin(), necessary_nodes.end(), to_be_added_necessary_nodes[z]) == 0){
                            necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                            added_node = true;
                            just_added_necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                        }
                    }
                    to_be_added_necessary_nodes.clear();
                }
                previously_added_necessary_nodes = just_added_necessary_nodes;
                just_added_necessary_nodes.clear();
            }

            
            
            
            //costruzzione del vettore di nodi ordinato toplogicamente
            std::vector<variable> var_necessary_nodes, O_var_necessary_nodes;
            for (size_t j = 0; j < necessary_nodes.size(); j++)
                var_necessary_nodes.push_back(var[variable_position[necessary_nodes[j]]]);
            var_necessary_nodes.push_back(var[i]); // aggiungo il nodo corrente
            O_var_necessary_nodes = node_sort(var_necessary_nodes);

            //mappatura nome==>indice del nodo nel vettore dei nodi necessari
            std::unordered_map<std::string,int> sub_position;
            for (size_t j = 0; j < O_var_necessary_nodes.size(); j++)
                sub_position[O_var_necessary_nodes[j].name] = j;

            int node_sub_idx = sub_position[var[i].name]; // indice del nodo corrente nel sottografo

            //calcolo della joint_probability sul sottografo 
            std::vector<int> sub_config(O_var_necessary_nodes.size(), 0);

            for (size_t t = 0; t < var[i].values.size(); t++){
                double value = 0;
                std::fill(sub_config.begin(), sub_config.end(), 0); // reset per ogni valore di t

                while (true){
                    // considero solo le configurazioni in cui il nodo corrente vale t
                    if (sub_config[node_sub_idx] == (int)t){
                        double joint = 1.0;
                        for (size_t B = 0; B < O_var_necessary_nodes.size(); B++){
                            std::vector<int> parent_config;
                            for (const auto& pname : O_var_necessary_nodes[B].parents)//itero sui nomi dei genitori del nodo B
                                parent_config.push_back(sub_config[sub_position[pname]]);
                            joint *= conditional_probability(
                                O_var_necessary_nodes[B], parent_config, variable_position, var
                            )[sub_config[B]];
                        }
                        value += joint;
                    }

                    // incremento multibase su sub_config vettore di configurazione del sottografo
                    int A = O_var_necessary_nodes.size() - 1;
                    while (A >= 0){//esploro tutte le configurazioni dei nodi necessari
                        if (++sub_config[A] < (int)O_var_necessary_nodes[A].values.size())
                            break;
                        sub_config[A] = 0;
                        A--;
                    }
                    if (A < 0) break; // tutte le configurazioni esplorate
                }

                var[i].probabilty.push_back(value);
            }
        }

        std::cout << "==>" << "nodi caratterizzati:" << i+1 << " su " << var.size()
                  << " (" << ((i+1)*100)/var.size() << "%)" << std::endl;
    }

    reader(var, variable_position);
    return var;
}