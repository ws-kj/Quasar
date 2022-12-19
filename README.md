# Quasar
GPT3-enabled surrogate functions in modern C++
```C++
std::string a = "I hate puppies and kittens."
std::string b = "I love sunshine and happiness."
bool result;

Quasar("If sentence a sounds more positive than sentence b, output result as true.")
	.with_model("text-davinci-003")
	.bind_input<std::string>("a", &a)
	.bind_input<std::string>("b", &b)
	.execute()
	.extract<bool>("result", &result);

std::cout << result; // false
```



