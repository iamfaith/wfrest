#ifndef WFREST_WFJSON_H_
#define WFREST_WFJSON_H_

#include "workflow/json_parser.h"
#include <string>

namespace wfrest
{

// class for convert json_value_t to std::string
class WFJson {
public:
    static std::string stringfy(json_value_t *val, bool format = false);

private:
    static void value_convert(const json_value_t *val, bool format, int depth, std::string* out_str);

    static void string_convert(const char *raw_str, std::string* out_str);

    static void number_convert(double number, std::string* out_str);

    static void array_convert(const json_array_t *arr, bool format, int depth, std::string* out_str);

    static void object_convert(const json_object_t *obj, bool format, int depth, std::string* out_str);
};
    
} // namespace wfrest

#endif // WFREST_WFJSON_H_
