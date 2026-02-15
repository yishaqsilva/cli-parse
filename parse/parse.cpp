#include "parse.h"
#include <cstring>
#include <iostream>

void Parser::addArgument(std::string name, std::string help){
    arg_map.insert({name, ""});
    arg_names.push_back(name);
    arg_help_map.insert({name, help});
}

void Parser::addOption(std::string sf, std::string lf, int defaultValue, std::string help){
    int_options.insert({sf, defaultValue});
    help_map.insert({sf, help});
    lf_map.insert({sf, lf});
    option_names.push_back(sf);
    option_names.push_back(lf);
}

void Parser::addOption(std::string sf, std::string lf, bool defaultValue, std::string help){
    bool_options.insert({sf, defaultValue});
    help_map.insert({sf, help});
    lf_map.insert({sf, lf});
    option_names.push_back(sf);
    option_names.push_back(lf);
}

void Parser::setFunction(void (*exec)(ValueMap values)){
    this->exec = exec;
}

void Parser::run(int argc, char** argv){
    
    int valid_args = 0;
    //check for description
    if (argc == 1){
        std::cout << header << std::endl;
        std::cout << "Run with --help for usage information" << std::endl;
        return;
    }

    //check for --help
    for (int i = 1; i < argc; i++){
        
        if (std::strcmp(argv[i], "--help") == 0){

            std::cout << header << "\n\n";
            
            for (int j = 0; j < arg_names.size(); j++){
                std::cout << arg_names[j] << "\t\t" << arg_help_map.at(arg_names[j]) << std::endl;

            }

            std::cout << std::endl;

            for (auto& [sf, help] : help_map){

                std::cout << sf << "\t" << lf_map.at(sf) << "\t\t" << help << std::endl;
            }

            std::cout << footer << std::endl;
            return;
        }
    
        //check for invalid options
        if ((char)argv[i][0] == '-'){
            int fails = 0;
            for (auto& option_name : option_names){
                if (std::strcmp(option_name.c_str(), argv[i]) != 0){
                    if (++fails == option_names.size()){
                        std::cout << "Invalid option " << argv[i] << std::endl;
                        return;
                    }
                }else{
                    break;
                }
            }

        //modify values of options
        for (auto& [sf, value] : int_options){

            if(std::strcmp(sf.c_str(), argv[i]) == 0 || std::strcmp(lf_map.at(sf).c_str(), argv[i]) == 0){ //could make this sexier

                try{
                    value = std::stoi(argv[i+1]);
                    i++;
                }
                catch(const std::exception& e){
                    std::cout << argv[i+1] << " is not a valid integer" << std::endl;
                    return;
                }
            }
        }

        for (auto& [sf, value] : bool_options){

            if (std::strcmp(sf.c_str(), argv[i]) == 0 || std::strcmp(lf_map.at(sf).c_str(), argv[i]) == 0){ 

                value = true;

                }
            }
        }
        else { //store potential argument
            if (valid_args < arg_names.size()){
                arg_map[arg_names[valid_args++]] = argv[i];
            }else{
                std::cout << "Too many arguments passed" << std::endl;
                return;
            }
        }
    }   
        
        if (valid_args != arg_names.size()){
            std::cout << "Invalid number of arguments\n";
            return;
        }

        exec(ValueMap{int_options, bool_options, arg_map});
}

void Parser::setFooter(std::string footer){
    
    this->footer = footer;
}

int ValueMap::getInt(std::string key){

    return int_options.at(key);

}

bool ValueMap::getBool(std::string key){

    return bool_options.at(key);
}

std::string ValueMap::getArg(std::string key){

    return arg_map.at(key);

}
