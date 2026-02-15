#ifndef PARSE_H
#define PARSE_H
#include <string>
#include <vector>
#include <map>

struct Argument{
    std::string name;
    std::string help;
};

class ValueMap {
    private:
    std::map<std::string, int>& int_options;
    std::map<std::string, bool>& bool_options;
    std::map<std::string, std::string>& arg_map;
    public:
    ValueMap(std::map<std::string, int>& int_options, std::map<std::string, bool>& bool_options, std::map<std::string, std::string>& arg_map) : int_options(int_options) , bool_options(bool_options), arg_map(arg_map){}
    int getInt(std::string key);
    bool getBool(std::string key);
    std::string getArg(std::string key);
};

class Parser{

    private:
    std::string header;
    std::string footer = "";
    std::map<std::string, int> int_options;
    std::map<std::string, bool> bool_options;
    std::map<std::string, std::string> help_map;
    std::map<std::string, std::string> arg_help_map;
    std::map<std::string, std::string> lf_map;
    std::map<std::string, std::string> arg_map;
    std::vector<std::string> option_names;
    std::vector<std::string> arg_names;
    void (*exec)(ValueMap vmap);
    
	public:
    Parser(std::string header) : header(header){}
    void addArgument(std::string name, std::string help);
    void addOption(std::string sf, std::string lf, int defaultValue, std::string help);
    void addOption(std::string sf, std::string lf, bool defaultValue, std::string help);
    void setFunction(void (*exec)(ValueMap vmap));
    void setFooter(std::string footer);
    void run(int argc, char** argv);
};

#endif
