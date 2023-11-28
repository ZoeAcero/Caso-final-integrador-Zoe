#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "json11.hpp"
#include "jsonlib.hpp"

struct Entorno {

};

class Variant {
public:
    typedef Variant(*proc_type)(const std::vector<Variant>&);
    typedef std::vector<Variant>::const_iterator iter;
    typedef std::map<std::string, Variant> map;

    variant_type type;
    std::string val;
    std::vector<Variant> list;
    proc_type proc;
    Entorno* env;

    Variant(variant_type type = Symbol) : type(type), env(nullptr), proc(nullptr) {}
    Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) {}
    Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) {}

    std::string to_string();
    std::string to_json_string();
    static Variant from_json_string(const std::string& json);
    static Variant parse_json(jsonlib::Json job);
    Variant apply(const std::vector<Variant>& args);
    void setEnvironment(Entorno* environment);
    }

        std::string Variant::to_string() {

    switch (type) {
        case Symbol:
            return val;
        case Number:
            return std::to_string(std::stod(val));
        case List:
            // Implementa la conversión de la lista a cadena
            break;
        case Proc:
            // Implementa la conversión del procedimiento a cadena
            break;
            // Agrega casos para otros tipos si es necesario
    }
    return "Unknown Type";
}

std::string Variant::to_json_string() {
    jsonlib::Json jsonObj;
    switch (type) {
        case Symbol:
            jsonObj = val;
            break;
        case Number:
            jsonObj = std::stod(val);
            break;
        case List:

            break;
        case Proc:

            break;

    }
    return jsonObj.dump();
}
Variant Variant::from_json_string(const std::string& sjson) {

    jsonlib::Json json = jsonlib::Json::parse(sjson);
    return parse_json(json);
}

Variant Variant::parse_json(jsonlib::Json job) {
    Variant result;
    if (job.is_string()) {
        result.type = Symbol;
        result.val = job.string_value();
    } else if (job.is_number()) {
        result.type = Number;
        result.val = std::to_string(job.number_value());
    } else if (job.is_array()) {
        result.type = List;
        for (const auto& item : job.array_items()) {
            result.list.push_back(parse_json(item));
        }
    } else if (job.is_object()) {

    }
    return result;
}
Variant Variant::apply(const std::vector<Variant>& args) {
    if (type == Proc && proc != nullptr) {
        return proc(args);
    } else {

        std::cerr << "Error: No se puede aplicar el procedimiento." << std::endl;
        return Variant();
    }
}
void Variant::setEnvironment(Entorno* environment) {
    // Establecer el entorno para el objeto Variant
    env = environment;
}

int main() {
    // Ejemplo de uso
    Variant symbolVariant(Symbol, "example");
    Variant numberVariant(Number, "42.5");
    Variant listVariant(List);
    listVariant.list.push_back(symbolVariant);
    listVariant.list.push_back(numberVariant);

    std::cout << "Symbol: " << symbolVariant.to_string() << std::endl;
    std::cout << "Number: " << numberVariant.to_string() << std::endl;
    std::cout << "List: " << listVariant.to_string() << std::endl;

    std::string jsonStr = "{\"type\":\"Symbol\",\"val\":\"example\"}";
    Variant jsonVariant = Variant::from_json_string(jsonStr);
    std::cout << "From JSON: " << jsonVariant.to_string() << std::endl;

    return 0;
}