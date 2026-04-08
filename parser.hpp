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
#include <unordered_set>
//per ora source ed header sono nello stesso file
//dichiaro la classe variable per benino così dopo posso giocarci sopra
class variable{
    public:
    std::string name;
    std::vector<std::string> values;
    std::vector<std::string> parents;
    std::vector<std::vector<double>> cpt;
    std::vector<double> probability;///guarda se funziona
    variable(std::string& name,std::vector<std::string>& values,std::vector<std::string>& parents,std::vector<std::vector<double>> cpt):name(name),values(values),parents(parents),cpt(cpt){}
    variable(std::string& name):name(name){}
};


//ultima cosa aggiunta
std::vector<variable> read_var(std::string filename);



#endif