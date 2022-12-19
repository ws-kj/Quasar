#include "quasar.h"

using json = nlohmann::json;
using namespace quasar;

CURL* Quasar::curl_handle = nullptr;
std::string Quasar::api_key = "";
std::string Quasar::org_key = "";

int Quasar::init(std::string key, std::string org) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if(!curl_handle) return 1;
    
    api_key = key;
    org_key = org;

    return 0;
}

void Quasar::cleanup() {
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

Quasar::Quasar(const char* prompt) {
    this->prompt = prompt;
}

Quasar& Quasar::with_model(Model model_id, float temp, size_t tokens) {
    switch(model_id) {
    case Model::GPT_Davinci:
        this->model = "text-davinci-003";
        break;
    case Model::GPT_Curie:
        this->model = "text-curie-001";
        break;
    case Model::GPT_Babbage:
        this->model = "text-babbage-001";
        break;
    case Model::GPT_Ada:
        this->model = "text-ada-001";
        break;
    default:
        this->model = "text-davinci-003";
    }
    this->temperature = temp;
    this->max_tokens = tokens;

    return *this;
}

Quasar& Quasar::with_model(const char* model_id, float temp, size_t tokens) {
    this->model = model_id;
    this->temperature = temp;
    this->max_tokens = tokens;

    return *this;
}

Quasar& Quasar::execute() {
    std::stringstream rprompt;
    struct curl_slist *headers=NULL;

    json invars;
    for(const auto& in : this->inputs)
        invars[in.first] = in.second;
    rprompt << invars.dump() << std::endl;

    rprompt << this->prompt << std::endl;
    rprompt << ".\n" << PROMPT_END << std::endl;

    json rbody = {
        {"model", this->model},
        {"prompt", rprompt.str()},
        {"max_tokens", this->max_tokens},
        {"temperature", this->temperature}
    };

    curl_easy_setopt(this->curl_handle, CURLOPT_URL, GPT_URL);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, 
            ("Authorization: Bearer " + this->api_key).c_str());
    if(!this->org_key.empty())
        headers = curl_slist_append(headers,
            ("OpenAI-Organization: " + this->org_key).c_str());
    curl_easy_setopt(this->curl_handle, CURLOPT_HTTPHEADER, headers);

    auto s_rbody = rbody.dump();
    const char* postfields = s_rbody.c_str();
    curl_easy_setopt(this->curl_handle, CURLOPT_POSTFIELDS, postfields);

    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEFUNCTION, &Quasar::rb_wrapper);

    this->curl_result = curl_easy_perform(this->curl_handle); 

    if(this->curl_result != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(this->curl_result));
        return *this;
    }

    json response = json::parse(this->last_response);
    std::string gpt_out = response["choices"][0]["text"];

    if(!json::accept(gpt_out)) {
        throw std::runtime_error("generated json invalid");
        return *this;
    }

    json final_out = json::parse(gpt_out);
    this->generated = final_out;

    return *this;
}

size_t Quasar::resp_builder(void *ptr, size_t size, size_t nmemb) {
    this->last_response += (const char*)ptr;
    return size * nmemb;
}

size_t Quasar::rb_wrapper(void *ptr, size_t size, size_t nmemb, void *q) {
    return static_cast<Quasar*>(q)->resp_builder(ptr, size, nmemb);
}
