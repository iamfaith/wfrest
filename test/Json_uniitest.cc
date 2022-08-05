#include "wfrest/WFJson.h"
#include <gtest/gtest.h>

using namespace wfrest;

TEST(JsonTest, stringfy)
{
    std::string json1 = 
R"([
    {
        "name": "Molecule Man",
        "age": 29,
        "secretIdentity": "Dan Jukes",
        "powers": [
            "Radiation resistance",
            "Turning tiny",
            "Radiation blast"
        ]
    },
    {
        "name": "Madame Uppercut",
        "age": 39,
        "secretIdentity": "Jane Wilson",
        "powers": [
            "Million tonne punch",
            "Damage resistance",
            "Superhuman reflexes"
        ]
    }
])";

    json_value_t *val = json_value_parse(json1.c_str());
    EXPECT_TRUE(val);
    // std::cout << Json::stringfy(val) << std::endl;
    EXPECT_EQ(Json::stringfy(val, true), json1);
}

TEST(JsonTest, stringfy_not_format) 
{
    std::string json = R"(
        { "name"   : "John Smith",
        "sku"    : "20223",
        "price"  : 23.95,
        "shipTo" : { "name" : "Jane Smith",
                    "address" : "123 Maple Street",
                    "city" : "Pretendville",
                    "state" : "NY",
                    "zip"   : "12345" },
        "billTo" : { "name" : "John Smith",
                    "address" : "123 Maple Street",
                    "city" : "Pretendville",
                    "state" : "NY",
                    "zip"   : "12345" }
        }
    )";
    json_value_t *val = json_value_parse(json.c_str());
    EXPECT_TRUE(val);
    std::string compressed = R"({"name":"John Smith","sku":"20223","price":23.95,"shipTo":{"name":"Jane Smith","address":"123 Maple Street","city":"Pretendville","state":"NY","zip":"12345"},"billTo":{"name":"John Smith","address":"123 Maple Street","city":"Pretendville","state":"NY","zip":"12345"}})";
    EXPECT_EQ(Json::stringfy(val), compressed);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}