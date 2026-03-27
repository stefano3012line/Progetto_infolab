#include "analyzer.hpp"

std::vector<std::vector<int>> line_to_parents_id(std::vector<std::string> node_parents,std::unordered_map<std::string,int> variable_position,std::vector<variable> var){//funzione per fare il conteggio multibase necessario a collegare la cpt a specifici valori dei genitori
    std::vector<std::vector<int>> line_to_parents_id_map;//connette una riga della cpt alla configurazione dei genitori corrispondente
    std::vector<int> vec(node_parents.size(),0);//vettore contenente le cordinate del genitore per una certa linea della cpt
    bool step_done=false;
    int j,line_number=1;
    for (size_t i = 0; i < node_parents.size(); i++)
    {
        line_number*=var[variable_position[node_parents[i]]].values.size();
    } 
    for (size_t i = 0; i < line_number; i++)//itero sulle linee della cpt
    {   
        line_to_parents_id_map.push_back(vec);
        step_done=false;
        j=node_parents.size()-1;
        if (i<line_number-1){//stavo facendo un conteggio di troppo
        while (step_done==false && j >= 0)
        {
            if (vec[j] < var[variable_position[node_parents[j]]].values.size()-1)//conteggio multibase
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


std::vector<double> conditional_probability(variable node,std::vector<int> parent_configuration,std::unordered_map<std::string,int> variable_position,std::vector<variable> var){//trova il valore della cpt corrispondente ad una specifica configurazione dei genitori
    int line_number=0,parent_value=0;
    for (size_t i = 0; i < node.parents.size(); i++)//trovo la linea della cpt corrispondente alla configurazione dei genitori data in input
    {
        parent_value=parent_configuration[i];
        for (size_t j = i+1; j < node.parents.size(); j++)//produttoria per eseguire il conteggio multibase al contrario e trovare la linea corretta della cpt
        {
            parent_value*=var[variable_position[node.parents[j]]].values.size();
        }
        
        line_number+=parent_value;
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


std::vector<variable> marginalizer(std::vector<variable> var){//completamente sbagliato mannaggia al clero
    std::unordered_map<std::string,int> variable_position ={};//mappa che associa a ogni nome la sua posizione nel vettore dei nodi
    std::vector<int> config(var.size(), 0);//vettore che contiene le configurazioni dei nodi
    double value=0,anchestor_config_probability=1;
    for (size_t i = 0; i < var.size(); i++)//creo un unordered map con chiave il nome della variabile e contenuto la posizione nel vettore dei nodi
    {
        variable_position[var[i].name]=i;
    }
    for (size_t i = 0; i < var.size(); i++){//itero sui nodi del network
        std::vector<std::vector<int>> line_id=line_to_parents_id(var[i].parents,variable_position,var);//possibile bottleneck
        if (var[i].parents.size() == 0){//caso senza parenti
            var[i].probabilty = var[i].cpt[0];
        }
        else{//bisogna rielaborare da qui
            for (size_t t = 0; t < var[i].values.size(); t++)//itero sui valori che può assumere il nodo
            {
                
                double value = 0.0;

                // Loop multibase su tutte le configurazioni
                while (true) {
                    // Verifico se l'evento del nodo in questa configurazione è quello che sto marginalizzando
                    if (config[variable_position[var[i].name]] == t) {
                        double joint = 1.0;
                        for (size_t j = 0; j < var.size(); j++) {
                            std::vector<int> parent_config;
                            for (const auto& pname : var[j].parents)
                                parent_config.push_back(config[variable_position[pname]]);
                            joint *= conditional_probability(var[j], parent_config, variable_position, var)[config[j]];
                        }
                        value += joint;
                    }

                    // Incremento multibase della configurazione
                    int A = var.size() - 1;
                    while (A >= 0) {
                        if (++config[A] < var[A].values.size())
                            break;
                        config[A] = 0;
                        A--;
                    }
                    if (A < 0) break; // tutte le configurazioni esplorate
                }

                var[i].probabilty.push_back(value);

























/*
                int line=0;//linea ed elemento della linea di cpt
                for (size_t line = 0; line < var[i].cpt.size(); line++)//itero sulle linee della cpt
                {
                    anchestor_config_probability = 1;
                    for (size_t parent_number = 0; parent_number < var[i].parents.size(); parent_number++)//itero sui parenti
                    {
                        anchestor_config_probability*=var[
                            variable_position[
                                var[i].parents[parent_number]
                            ]].probabilty[line_id[line][parent_number]];//questa linea serve a risalire alle specifiche probabilità marginalizzate dei genitori
                    }
                    value+=var[i].cpt[line][t]*anchestor_config_probability;
                }
                var[i].probabilty.push_back(value);
                value=0;

*/






































/*

                /////////////////////////////OBROBRIO ORRIBILE CHE DEVO ASSOLUTAMENTE OTTIMIZZARE 
                //generare un vettore di nodi necessari per il calcolo della probabilità marginalizzata
                std::vector<std::string> necessary_nodes,//tiene di conto dei nodi necessari al calcolo della probabilità marginalizzata (nomi dei nodi)
                                        previously_added_necessary_nodes,//tiene conto dei nodi aggiunti all'ultima iterazione del ciclo (nomi dei genitori dei nodi aggiunti nell'ultima iterazione)
                                        just_added_necessary_nodes,//tiene conto dei nodi aggiunti in questa iterazione (nomi dei genitori dei nodi aggiunti in questa iterazione)
                                        to_be_added_necessary_nodes;//tiene conto dei nodi che potrebbero essere aggiunti 
                bool added_node=false;

                for (size_t j = 0; j < var[i].parents.size(); j++)//aggiungo alla lista di nodi necessari i genitori del nodo che sto marginalizzando
                {
                    to_be_added_necessary_nodes.push_back(var[variable_position[var[i].parents[j]]].name);
                }
                for (size_t z = 0; z < to_be_added_necessary_nodes.size(); z++)
                {
                    if (std::count(necessary_nodes.begin(), necessary_nodes.end(), to_be_added_necessary_nodes[z]) == 0)
                    {
                        necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                        added_node=true;
                    }
                }
                previously_added_necessary_nodes=to_be_added_necessary_nodes;
                to_be_added_necessary_nodes.clear();

                 while (added_node)
                {
                    added_node=false;
                    for (size_t k = 0; k < previously_added_necessary_nodes.size(); k++)
                    {
                        for (size_t j = 0; j < var[variable_position[previously_added_necessary_nodes[k]]].parents.size(); j++)//itero sui parenti del nodo
                        {
                            to_be_added_necessary_nodes.push_back(var[variable_position[var[variable_position[previously_added_necessary_nodes[k]]].parents[j]]].name);
                        }
                        for (size_t z = 0; z < to_be_added_necessary_nodes.size(); z++)
                        {
                            if (std::count(necessary_nodes.begin(), necessary_nodes.end(), to_be_added_necessary_nodes[z]) == 0)
                            {
                                necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                                added_node=true;
                                just_added_necessary_nodes.push_back(to_be_added_necessary_nodes[z]);
                            }
                        }
                        to_be_added_necessary_nodes.clear();
                    }
                    previously_added_necessary_nodes=just_added_necessary_nodes;
                    just_added_necessary_nodes.clear();
                }
                //genero un vettore ordinato di nodi necessari a calcolare la probabilità marginalizzata
                std::vector<variable> var_necessary_nodes,O_var_necessary_nodes;//vettore che contiene solo i nodi necessari al calcolo della probabilità marginalizzata
                for (size_t j = 0; j < necessary_nodes.size(); j++)
                {
                    var_necessary_nodes.push_back(var[variable_position[necessary_nodes[j]]]);
                }
                var_necessary_nodes.push_back(var[i]);//aggiungo anche il nodo che sto marginalizzando
                O_var_necessary_nodes=node_sort(var_necessary_nodes);//ordino i nodi necessari in modo che se un nodo è genitore di un altro allora viene prima nell'ordinamento




                int number_of_configurations=1;//calcolo il nemero di configurazioni dei genitori
                for (size_t j = 0; j < O_var_necessary_nodes.size(); j++)
                {
                    if (O_var_necessary_nodes[j].name != var[i].name)
                    {
                        number_of_configurations*=O_var_necessary_nodes[j].values.size();
                    }
                }
                

                //genero una mappa name to id per i nodi necessari
                std::unordered_map<std::string,int> necessary_variable_position ={};
                for (size_t j = 0; j < necessary_nodes.size(); j++)
                {
                    necessary_variable_position[necessary_nodes[j]]=j;
                }

                
  
                    
                std::vector<std::vector<int>> configurations=line_to_parents_id(necessary_nodes,variable_position,var);//genero un vettore contenente tutte le possibili configurazioni dei nodi necessari
                std::vector<int> nodes_parents_configuration;//vettore che contiene la configurazione dei genitori di uno specifico nodo
                for (size_t A=0; A < configurations.size();A++)//ciclo su tutte le configurazioni dei nodi necessari (A mi dice a quale configurazione faccio riferimento)
                {
                    double configuration_probability=1;
                    for (size_t B = 0; B < O_var_necessary_nodes.size(); B++)
                    {

                        for (size_t C = 0; C < O_var_necessary_nodes[B].parents.size(); C++)
                        {
                            nodes_parents_configuration.push_back(configurations[A][necessary_variable_position[O_var_necessary_nodes[B].parents[C]]]);//creo un vettore che contiene la configurazione dei genitori di uno specifico nodo
                        }
                        //aggiungi un if per controllare se il nodo in questione è quello che sto marginalizzando e in quel caso non prendere la configurazione dei genitori ma specificare il valore del nodo che sto marginalizzando in questa iterazione
                        int D;//prendo il valore del nodo in questione nella configurazione A

                        if (O_var_necessary_nodes[B].name == var[i].name)
                        {
                            D = t; // 
                        }
                        else
                        {
                            D = configurations[A][necessary_variable_position[O_var_necessary_nodes[B].name]];
                        }
                        configuration_probability*=conditional_probability(O_var_necessary_nodes[B],nodes_parents_configuration,variable_position,var)[D];
                        nodes_parents_configuration.clear();////TOGLI QUESTA MERDA CHE E' COMPLESSITA N
                    }
                    value+=configuration_probability;
                }//per ora non sto specificando il valore del nodo che sto marginalizzando

                    var[i].probabilty.push_back(value);
                    value=0;

*/










            }
        }
        std::cout<<"==>"<<"nodi caratterizzati:"<<i+1<<" su "<<var.size()<<" ("<<((i+1)*100)/var.size()<<"%)"<<std::endl;
    }
    reader(var,variable_position);   
    return var;
}

//mi sputa fuori il vettore di nodi ma con un nodo marginalizzato



//////testare  il corretto funzionamento del codice

/////ottimizzare il processo si marginalizzazione (posso fare una memoization per le combinazioni in modo tale da calcolarli una volta sola)