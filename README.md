# Quasar
GPT3-enabled surrogate functions in modern C++

## Installation
- Dependencies: `libcurl`, `nlohmann_json 3.2.0`
- Install process: `cmake .; make; make install`
- Library will appear as `libquasar.so` or `libquasar.dylib` in the path specified by `CMAKE_INSTALL_PREFIX` when built.

## Example usage
```C++
using namespace quasar; 

...

// Load OpenAI API key from file.
std::ifstream fs("<api_key_file>.json");
std::string key;
if(fs.good())
    getline(fs, key);

// Must initialize before using
if(Quasar::init(key)) return 1;

std::string bad = "What day of the wek is it?";
std::string good;

Quasar("Fix the spelling mistake in bad and output it into good.")
    .with_model(Model::GPT_Davinci) // Optional params: gpt temperature, max tokens
    .bind_input<std::string>("bad", &bad) // Input can be any streamable type
    .execute()
    .extract<std::string>("good", &good); // Extraction throws exception on failure

std::cout << good; // "What day of the week is it?"

Quasar::cleanup();
```

## Notes
* GPT3 is not necessarily deterministic. Visit the OpenAI docs to learn more about temperature and tokens.
* Quasar inputs and outputs should ideally be wrapped in `try`/`catch` blocks.


