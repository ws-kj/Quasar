#include "quasar.h"
#include <fstream>
#include <sstream>
using namespace quasar;

int main(int argc, char **argv) {

    std::ifstream fs("key.json");
    std::string key;
    if(fs.good())
        getline(fs, key);
        
    if(Quasar::init(key)) return 1;
    
    std::string b = "I love sunshine and happiness.";
    std::string a = "I hate puppies and kittens.";
    bool c;

    Quasar("if sentence a sounds more positive than sentence b, output c as true.")
        .with_model(Model::GPT_Davinci)
        .bind_input<std::string>("a", &a)
        .bind_input<std::string>("b", &b)
        .execute()
        .extract<bool>("c", &c);

    std::cout << c;

    Quasar::cleanup();
    return 0;
}
