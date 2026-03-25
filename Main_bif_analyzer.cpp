#include "analyzer.hpp"

int main(){
    std::string filename;
    std::cout<<"==>"<<"inserire il file da analizzare"<<std::endl<<"==>";
    std::cin>>filename;
    std::vector<variable> O_var = marginalizer(node_sort(read_var(("file_bif/"+filename+".bif"))));    
    return 0;
}
//aggiungere runtime
