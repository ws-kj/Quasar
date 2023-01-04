#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdexcept>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace quasar 
{

    constexpr const char* PROMPT_END = "Return all output variables and their values in a valid json object like this: {\"name\":<value>, \"name2\":<value>}. Numerical values should never be in quotations. The json object must be valid.";

    constexpr const char* GPTCOMP_URL = "https://api.openai.com/v1/completions";

    template<typename S, typename T, typename = void>
    struct is_streamable: std::false_type {};

    template<typename S, typename T>
    struct is_streamable<S, T, 
        std::void_t<decltype(std::declval<S&>()<<std::declval<T>())> >
    : std::true_type {};

    enum class Model {
        GPT_Davinci,
        GPT_Curie,
        GPT_Babbage,
        GPT_Ada
    };

    class Quasar {
        static CURL* curl_handle;
        CURLcode curl_result;

        static std::string api_key;
        static std::string org_key;

        const char* prompt;
        const char* model;
        float temperature;
        size_t max_tokens;

        std::vector<std::pair<const char*, std::string>> inputs;

        std::string last_response;

        int openai_req(const char *url, const char *body);
        size_t resp_builder(void *ptr, size_t size, size_t nmemb);
        static size_t rb_wrapper(void *ptr, size_t sz, size_t nmemb, void *q);

        json generated;

    public:
        Quasar(const char* prompt);

        static int init(std::string key, std::string org="");
        static void cleanup();

        Quasar& with_model(Model model_id, float temp=1.f, size_t tokens=128);
        Quasar& with_model(const char* model_id, float temp=1.f, size_t tokens=128);

        template<typename T>
        Quasar& bind_input(const char* name, T* input) {
            static_assert(is_streamable<std::stringstream, T>::value == true);
            std::stringstream ss;
            (ss << *input);
            inputs.push_back(std::make_pair(name, ss.str()));
            return *this;
        }

        template<typename T>
        Quasar& extract(const char* name, T* ref) {
            try {
                T genval = this->generated.at(name).get<T>();
                *ref = genval;
            } catch(std::exception& e) {
                throw e;
            }

            return *this;
        }

        Quasar& execute();
    };
}
