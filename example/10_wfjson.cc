#include "workflow/WFFacilities.h"
#include <csignal>
#include "wfrest/HttpServer.h"
#include "wfrest/WFJson.h"

using namespace wfrest;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
    wait_group.done();
}

int main()
{
    signal(SIGINT, sig_handler);

    HttpServer svr;
    
    // curl -v http://ip:port/json1
    svr.GET("/json1", [](const HttpReq *req, HttpResp *resp)
    {
        // for usage you can see https://github.com/Barenboim/json-parser
        json_value_t *json = json_value_create(JSON_VALUE_OBJECT);
        json_object_t *test_obj = json_value_object(json);
        json_object_append(test_obj, "test", JSON_VALUE_NUMBER, 123.0);
        json_object_append(test_obj, "recv", JSON_VALUE_STRING, "json");

        // If you call resp->Json, you don't need to destroey json_value_t yourself
        resp->Json(json);
    });

    if (svr.start(8888) == 0)
    {
        wait_group.wait();
        svr.stop();
    } else
    {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
    return 0;
}
