#pragma once

#include <string>
#include <vector>
#include <map>
#include <any>
#include <iostream>
#include <sstream>
#include <assert.h>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace quasar 
{

    constexpr const char* PROMPT_BEGIN = "Output variables are ";
    constexpr const char* PROMPT_END = "Return all output variables and their values in a valid json object like this: {\"name\":<value>, \"name2\":<value>}";

    constexpr const char* GPT_URL = "https://api.openai.com/v1/completions";

    template<typename S, typename T, typename = void>
    struct is_streamable: std::false_type {};

    template<typename S, typename T>
    struct is_streamable<S, T, 
        std::void_t<decltype(std::declval<S&>()<<std::declval<T>())> >
    : std::true_type {};

    class Quasar {
        static CURL* curl_handle;
        CURLcode curl_result;

        static std::string api_key;
        static std::string org_key;

        const char* prompt;
        const char* model;
        float temperature;
        int max_tokens;

        std::vector<std::pair<const char*, std::string>> inputs;
        std::vector<std::pair<const char*, std::any>> outputs;

        std::string last_response;
        size_t resp_builder(void *ptr, size_t size, size_t nmemb, void *);

    public:
        Quasar(const char* prompt);

        static int  init(std::string key, std::string org="");
        static void cleanup();

        Quasar& with_model(const char* model_id, 
                float temp=1.f, int tokens=16);

        template<typename T>
        Quasar& bind_input(const char* name, T* input) {
            static_assert(is_streamable<std::stringstream, T>::value == true);
            std::stringstream ss;
            (ss << *input);
            inputs.push_back(std::make_pair(name, ss.str()));
            return *this;
        }


        template<typename T>
        Quasar& bind_output(const char* name, T* output) {
            std::any ref = output;
            outputs.push_back(std::make_pair(name, ref));
            return *this;
        }

        Quasar& execute();
        int success();
    };
}


/*
 *
 *  quasar::Quasar("Multiply a and b together, and return into c.")
 *          .with_model(quasar::davinci)
 *          .bind_input<int>(&a, "a")
 *          .bind_input<int>(&b, "b")
 *          .bind_output<int>(&c, "c")
 *          .execute();
 *
 */

/*
 * {'a': '5', 'b':'10'}
 * Multiply a and b together into c.
 * Output variables are c.
 * Return all output variables and their values in a json object like this: {'name': '<value>'}
 * 
 */