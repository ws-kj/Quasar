#include <quasar.h>
#include <fstream>
#include <sstream>
using namespace quasar;

int main(int argc, char **argv) {

    std::ifstream fs("../src/key.json");
    std::string key;

    if(fs.good()) { getline(fs, key); } else {
        std::cerr << "could not load key\n";
        return 1;
    }

    if(Quasar::init(key)) return 1;
/* 
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
*/
    std::string bad = "What day of the wek is it?";
    std::string good;

    Quasar("Fix the spelling mistake in bad and output it into good.")
        .with_model(Model::GPT_Davinci)
        .bind_input<std::string>("bad", &bad) 
        .execute()
        .extract<std::string>("good", &good);

    std::cout << good;

    Quasar::cleanup();
    return 0;
}
