#include <gtest/gtest.h>
#include "wfrest/HttpServer.h"
#include "wfrest/WFJson.h"
#include "workflow/WFFacilities.h"

using namespace wfrest;
using namespace protocol;

WFHttpTask *create_http_task(const std::string &path)
{
    return WFTaskFactory::create_http_task("http://127.0.0.1:8888/" + path, 4, 2, nullptr);
}

TEST(WFJsonTest, valid_json)
{
    HttpServer svr;
    WFFacilities::WaitGroup wait_group(1);

    svr.GET("/test", [](const HttpReq *req, HttpResp *resp)
    {
        std::string json_str = R"({
                                    "name": "John",
                                    "age": 22,
                                    "gender": "male"
                                })";
        json_value_t *val = json_value_parse(json_str.c_str());
        EXPECT_TRUE(val);
        resp->Json(val);
    });

    EXPECT_TRUE(svr.start("127.0.0.1", 8888) == 0) << "http server start failed";

    WFHttpTask *client_task = create_http_task("test");
    client_task->set_callback([&wait_group](WFHttpTask *task)
    {
        HttpResponse *resp = task->get_resp();

        HttpHeaderMap header(resp);
        std::string content_type = header.get("Content-Type");
        EXPECT_EQ(content_type, "application/json");

        const void *body;
        size_t body_len;
        resp->get_parsed_body(&body, &body_len);
        std::string json_str(static_cast<const char *>(body), body_len);
        std::string expect_str = R"({"name":"John","age":22,"gender":"male"})";
        EXPECT_EQ(json_str, expect_str);
        wait_group.done();
    });

    client_task->start();
    wait_group.wait();
    svr.stop();
}

TEST(WFJsonTest, invalid_json)
{
    HttpServer svr;
    WFFacilities::WaitGroup wait_group(1);

    svr.GET("/test", [](const HttpReq *req, HttpResp *resp)
    {
        std::string invalid_text = R"(
        {
            "strings": ["extra", "comma", ]
        }
        )";
        json_value_t *val = json_value_parse(invalid_text.c_str());
        resp->Json(val);
    });

    EXPECT_TRUE(svr.start("127.0.0.1", 8888) == 0) << "http server start failed";

    WFHttpTask *client_task = create_http_task("test");
    client_task->set_callback([&wait_group](WFHttpTask *task)
    {
        HttpResponse *resp = task->get_resp();

        HttpHeaderMap header(resp);
        std::string content_type = header.get("Content-Type");
        EXPECT_TRUE(content_type == "application/json");

        const void *body;
        size_t body_len;
        resp->get_parsed_body(&body, &body_len);

        std::string json_str(static_cast<const char *>(body), body_len);
        std::string expect_str = R"({"errmsg":"Invalid Json Syntax"})";
        EXPECT_EQ(json_str, expect_str);
        wait_group.done();
    });

    client_task->start();
    wait_group.wait();
    svr.stop();
}

TEST(WFJsonTest, recv_json)
{
    HttpServer svr;
    WFFacilities::WaitGroup wait_group(1);

    svr.POST("/test", [](const HttpReq *req, HttpResp *resp)
    {
        if (req->content_type() != APPLICATION_JSON)
        {
            std::string err_json(R"({errmsg : "Content Type not APPLICATION_JSON"})");
            resp->Json(err_json);
            return;
        }
        json_value_t *js = req->wfjson();
        EXPECT_EQ(req->header("Content-Type"), "application/json");
        json_object_t *obj = json_value_object(js);
        const json_value_t* test = json_object_find("test", obj);
        const json_value_t* recv = json_object_find("recv", obj);
        EXPECT_EQ(json_value_number(test), 123.0);
        EXPECT_TRUE(strcmp(json_value_string(recv), "json") == 0);
    });

    EXPECT_TRUE(svr.start("127.0.0.1", 8888) == 0) << "http server start failed";

    WFHttpTask *client_task = create_http_task("test");

    json_value_t *test_val = json_value_create(JSON_VALUE_OBJECT);
    json_object_t *test_obj = json_value_object(test_val);
    json_object_append(test_obj, "test", JSON_VALUE_NUMBER, 123.0);
    json_object_append(test_obj, "recv", JSON_VALUE_STRING, "json");

    std::string js_str = Json::stringfy(test_val);
    printf("test : %s\n", js_str.c_str());
    std::string *str = new std::string;
    *str = std::move(js_str);
    client_task->get_req()->set_method("POST");
    client_task->get_req()->append_output_body_nocopy(str->data(), str->size());
    client_task->get_req()->add_header_pair("Content-Type", "application/json");
    client_task->user_data = str;
    client_task->set_callback([&wait_group, test_val](WFHttpTask *task)
    {
        json_value_destroy(test_val);
        delete static_cast<std::string *>(task->user_data);
        wait_group.done();
    });
    client_task->start();
    wait_group.wait();
    svr.stop();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}