#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include <fstream>
#include<vector>
#include <istream>
#include <string>
#include <unordered_map>
#include<algorithm>
#include<cmath>
//per ora source ed header sono nello stesso file
//dichiaro la classe variable per benino così dopo posso giocarci sopra
class variable{
    public:
    std::string name;
    std::vector<std::string> values;
    std::vector<std::string> parents;
    std::vector<std::vector<double>> cpt;
    std::vector<double> probabilty;///guarda se funziona
    //chiedere il significato della riga sotto
    variable(std::string& name,std::vector<std::string>& values,std::vector<std::string>& parents,std::vector<std::vector<double>> cpt):name(name),values(values),parents(parents),cpt(cpt){}
    variable(std::string& name):name(name){}
};


//ultima cosa aggiunta
std::vector<variable> read_var(std::string filename);




//sembra che non mi serva
//dichiaro la probability
class probability{
    public:
    std::string target;
    std::vector<std::string> parents;
    std::vector<std::vector<double>> cpt;
    probability(std::string& target,std::vector<std::string>& parents,std::vector<std::vector<double>> cpt):target(target),parents(parents),cpt(cpt){}
};
std::vector<variable> read_var(std::string filename);

#endif