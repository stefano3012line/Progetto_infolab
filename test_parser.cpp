#include "parser.hpp"

std::vector<variable> read_var(std::string filename){//caricare un vettore di variabili a partire dal bif
    std::ifstream file(filename);
    std::vector<variable> var;
    std::string line,value_string,p_arguments,var_name,variable_token;
    std::vector<std::string> values;
    std::vector<std::string> parents;
    double cpt_value;
    std::vector<double> cpt_vector;
    std::vector<std::vector<double>> cpt;
    int n_values;
    int i=0,parents_number=0;
/////////BLOCCO NAME
    while(std::getline(file, line)){
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());//cancella gli spazi
        variable_token = "variable";
        if (line.find(variable_token) != std::string::npos) {
        var_name= line.substr(line.find(variable_token)+variable_token.size(),line.find("{")-(line.find(variable_token)+variable_token.size()));//seleziona tutti i valori tra variable e la graffa esclusi 
        var.push_back(variable(var_name));
        variable_position[var_name]=i;//creo un unordered map con chiave il nome della variabile e contenuto la posizione nel vettore dei nodi
        i++;
        }
    
    
    
    /////BLOCCO VALUES
    
        if (line.find("typediscrete") != std::string::npos) {//ottiene i possibili valori assunti dalla variable
            
        n_values=stoi(line.substr(line.find("[")+1,line.find("]")-(line.find("[")+1)));//ottiene il numero di valori assunti dalla variabile
            
        value_string=line.substr(line.find("{")+1,line.find("}")-(line.find("{")+1));

        for (int a = 0; a < n_values-1; a++)
        {
            values.push_back(value_string.substr(0,value_string.find(","))); 
            value_string.erase(0,value_string.find(",")+1);                                                                                     
        }
        values.push_back(value_string);
        var[variable_position[var_name]].values = values;  //TEMP---usa l'ultimo var_name trovato
        values.clear();//svuoto il vettore per usarlo di nuovo      
            
        }
    
    ///////BLOCCO PARENTS 
        if (line.find("probability(") != std::string::npos) {//ottiene i parenti di una certa variabile 
            p_arguments = line.substr(line.find("(")+1,line.find(")")-(line.find("(")+1));

            if (p_arguments.find("|") == std::string::npos)
            {
                var_name = p_arguments;
            }
            else{
                var_name=p_arguments.substr(0,p_arguments.find('|'));
                p_arguments.erase(0,p_arguments.find('|')+1);
                if (p_arguments.find(",") == std::string::npos)
                {
                    parents.push_back(p_arguments);
                }
                else{
                    while (p_arguments.find(',') != std::string::npos){
                        parents.push_back(p_arguments.substr(0,p_arguments.find(",")));
                        p_arguments.erase(0,p_arguments.find(",")+1) ; 
                    }
                    parents.push_back(p_arguments);
                }                
            }
        
            parents_number=parents.size();
            var[variable_position[var_name]].parents = parents;
            parents.clear();
            ///////////////////////////////////////blocco CPT
            if (parents_number == 0 ){
                std::getline(file, line);
                variable_token="table";
                line.erase(0,line.find(variable_token)+variable_token.size());//cancella da 0 a table inclusi
                while (line.find(',') != std::string::npos)
                {
                    cpt_value=stod(line.substr(0,line.find(",")));
                    cpt_vector.push_back(cpt_value);
                    line.erase(0,line.find(",")+1);     
                }
                cpt_vector.push_back(stod(line.substr(0,line.find(";"))));
                cpt.push_back(cpt_vector);
                cpt_vector.clear();
                
            }
            else{
                for (size_t k = 0; k < pow(2,parents_number); k++){
                    std::getline(file, line);
                    line.erase(0,line.find(")")+1);
                    while (line.find(',') != std::string::npos)
                    {
                        cpt_value=stod(line.substr(0,line.find(",")));
                        cpt_vector.push_back(cpt_value);
                        line.erase(0,line.find(",")+1);     
                    }
                cpt_vector.push_back(stod(line.substr(0,line.find(";"))));
                cpt.push_back(cpt_vector);
                cpt_vector.clear();

                        
                }
                
            }
            var[variable_position[var_name]].cpt = cpt;
            cpt.clear();
            
            
        }
    }
    return var;
}

int main(){
    std::vector<variable> gradient = read_var("andes.bif");
    for (size_t i = 0; i < gradient.size(); i++)//scorre tra gli elementi di var
    {
        std::cout<<gradient[i].name<<":";
        for (size_t j = 0; j < gradient[i].cpt.size(); j++)//scorre tra i vettori della cpt
        {
            for (size_t k = 0; k < gradient[i].cpt[j].size(); k++)//scorre tra gli elementi del vettore della cpt
            {
                std::cout<<gradient[i].cpt[j][k]<<",";               
            }
            std::cout<<std::endl;
            
        }
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        
    }
    
    
    return 0;
}