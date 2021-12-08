#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <regex>
#include <string>
using namespace std;

int getvertice; 

std::string parseInput(std::string stream){
    std::istringstream data(stream);
    std::ws(data);
    char command; 
    data >> command;

    if(data.fail() && !data.eof()){
        std::cout<<"Error:Enter a valid input \n";
        return "Fail";
    }


    if (command == 'V'){
    getvertice = 0;
    int vertice;

    data >> vertice;
    if(data.fail() && !data.eof()){
            std::cout<<"Error:Enter a valid input \n";
            return "Fail";
        }
        if (vertice < 2){
            std::cout<<"Error: Enter a valid input \n";
            return "Fail";
        }else{
        //  graph.set_vertice(vertice); 

        getvertice = vertice;
        //  vectice_size = getvertice + 1;
            // for (auto& e : edges) {
            //     e.clear();
            //         }
        }
        return "Fail";
    }

    if (command == 'E'){
        if(getvertice == 0){
            std::cout<<"Enter a valid input \n";
            return "Fail";
        }
        else {
            std::string edgelist;
            data >> edgelist;
            if(data.fail() && !data.eof()){
                std::cout<<"Error: Enter a valid input \n";
                return "Fail";
            }
            if(edgelist == "{}"){
                std::cout<<"     \n";
                return "Fail";
            }           
            std::regex re(R"(\d+,\d+)");
            std::sregex_iterator currentMatch(edgelist.begin(), edgelist.end(), re);
            std::sregex_iterator lastMatch;
            int node1;
            int node2;
            int checkVertice = getvertice;
            
            vector<int > list;
            //  std::string edgeStr = "";
            bool fail = false;
            while(currentMatch != lastMatch)
            {
                std::smatch match = *currentMatch;
                std::string delimiter = ",";
                std::string num1 = match.str().substr(0, match.str().find(delimiter));
                std::string num2 = match.str().substr(match.str().find(delimiter)+1, match.str().length());
                istringstream (num1)>>node1;
                istringstream (num2)>>node2;        
                if( (node1 < 0 ) || (node2 < 0)){
                std::cout<<"Error : invalid vertex value \n";
                fail = true;
                break;
                }
                if((node1 > checkVertice - 1) || (node2 > checkVertice - 1)){
                        std::cout<<"Error: invalid vertex \n";
                        fail = true;
                        break;
                }else{
                //    edgeStr = edgeStr + std::to_string(node1) + ",";
                //    edgeStr = edgeStr + std::to_string(node2) + ",";

                list.push_back(node1);
                list.push_back(node2);
                currentMatch ++;
                }
            }

            if(fail == true){    
                return "Fail";
            }

            else{
                std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
                int vertex_cover  = 1;
                Minisat::vec<Minisat::Lit> buffer;
                Minisat::vec<Minisat::Lit> buffer_2;
                vector<Minisat::Lit> clause(getvertice);
                vector< vector<Minisat::Lit> > matrix_ci(getvertice , clause);
                vector<int> result;
                bool sat;
                while(vertex_cover <= getvertice){
                    solver.reset (new Minisat::Solver());
                    // Setting up ci
                    for (int i = 0; i < getvertice ; i++){
                        for (int j=0 ; j < vertex_cover ; j++){
                            matrix_ci[i][j] = Minisat::mkLit(solver->newVar());
                        }
                    }
                    // First Set of Clause
                    for (int i=0; i < vertex_cover; i++){
                        for(int n=0; n < getvertice ; n++){
                            buffer.push(matrix_ci[n][i]);
                        }
                        solver->addClause_(buffer);
                        buffer.clear();
                    }

                    // Second Set of Clause
                    for (int m = 0; m < getvertice; m++){
                        for (int p=0;p<vertex_cover;p++){
                            for(int q=p+1;q<vertex_cover;q++){
                                solver->addClause(~matrix_ci[m][p],~matrix_ci[m][q]);
                            }
                        }
                    }
                    // Third Set of Clause 
                    for (int m=0; m < vertex_cover; m++){
                        for(int p=0; p< getvertice; p++){
                            for(int q=p+1;q< getvertice;q++){
                                solver->addClause(~matrix_ci[p][m],~matrix_ci[q][m]);
                            }
                        }
                    }   
                    // Fourth Set of Clause

                    for (std::vector<int>::size_type i=0; i < list.size()-2 ; i = i+2 ){
                        for (int k=0; k< vertex_cover; k++){
                            buffer_2.push(matrix_ci[list[i]][k]);
                            buffer_2.push(matrix_ci[list[i+1]][k]);
                        }
                        solver->addClause_(buffer_2);
                        buffer_2.clear();
                    }
                // The SAT Solve is called.
                sat = solver->solve();

                if(sat){
                    result.clear();
                    for (int i = 0; i < getvertice; i++){
                        for (int j=0; j < vertex_cover; j++){
                            if(toInt(solver->modelValue(matrix_ci[i][j])) == 0 ){
                                result.push_back(i);
                            }
                        }
                    }
                    break;
                }
                else{
                    vertex_cover++;
                }
            }

            for (std::vector<int>::size_type i = 0; i< result.size(); i++){
                cout<<result[i]<<" ";
            }
            cout<<"\n";

        }

         return "Fail";
    }   
      return "Fail";
    }

    return "Fail";
}


int main(int argc, char** argv) {
    // Test code. Replaced with your code
   // Add a try and catch method

  while (true){
     try{
     if (!std::cin.eof()){
         std::string stream;
         std::getline(std::cin , stream);
        std::istringstream data(stream);
        std::ws(data);
        
        // std::cout<<parseInput(stream)<<std::endl;
        while (!data.eof()){
            if(parseInput(stream) == "Fail"){
                break;
            }
         }
       // std::cout<<edges<<std::endl;
        //std::cout<<getvertice<<std::endl;
     } else {
            std::exit(0);
     }
  }catch(...){
      std::cout << "Error: An error occured \n"<<std::endl;
  }
  } 
    return 0;
 }
