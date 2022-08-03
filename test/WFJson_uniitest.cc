#include "wfrest/WFJson.h"
#include <gtest/gtest.h>

using namespace wfrest;

TEST(WFJsonTest, stringfy)
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
    // std::cout << WFJson::stringfy(val) << std::endl;
    EXPECT_EQ(WFJson::stringfy(val), json1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}