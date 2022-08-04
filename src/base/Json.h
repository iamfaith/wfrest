#ifndef WFREST_JSON_H_
#define WFREST_JSON_H_

#include "workflow/json_parser.h"
#include <string>

namespace wfrest
{

// class for convert json_value_t to std::string
class Json {
public:
    static std::string stringfy(json_value_t *val, bool format = false);

private:
    static void value_convert(const json_value_t *val, bool format, int depth, std::string* out_str);

    static void string_convert(const char *raw_str, std::string* out_str);

    static void number_convert(double number, std::string* out_str);

    static void array_convert(const json_array_t *arr, bool format, int depth, std::string* out_str);

    static void array_convert_not_format(const json_array_t *arr, int depth, std::string* out_str);

    static void object_convert(const json_object_t *obj, bool format, int depth, std::string* out_str);

    static void object_convert_not_format(const json_object_t *obj, int depth, std::string* out_str);
};
    
} // namespace wfrest

#endif // WFREST_JSON_H_
