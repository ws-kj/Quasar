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
    int a = 5;
    int b = 10;
    int c;

    Quasar surrogate = Quasar("multiply a and b together into c")
            .with_model("text-davinci-003") 
            .bind_input<int>("a", &a) 
            .bind_input<int>("b", &b) 
            .bind_output<int>("c", &c)
            .execute();

    return 0;
}
