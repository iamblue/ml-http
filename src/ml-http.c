#include <string.h>
#include "jerry-api.h"
#include "httpclient.h"
#include "microlattice.h"

DELCARE_HANDLER(__http) {
  if (args_cnt == 2 && jerry_value_is_object(args_p[0])) {

    jerry_value_t prop_method = jerry_create_string ((const jerry_char_t *) "method");
    jerry_value_t method = jerry_get_property(args_p[0], prop_method);

    jerry_value_t prop_url = jerry_create_string ((const jerry_char_t *) "url");
    jerry_value_t url = jerry_get_property(args_p[0], prop_url);

    jerry_value_t prop_header = jerry_create_string ((const jerry_char_t *) "header");
    jerry_value_t header = jerry_get_property(args_p[0], prop_header);

    jerry_size_t url_req_sz = jerry_get_string_size (url);
    jerry_char_t url_buffer[url_req_sz];
    jerry_string_to_char_buffer (url, url_buffer, url_req_sz);
    url_buffer[url_req_sz] = '\0';

    jerry_size_t header_req_sz = jerry_get_string_size (header);
    jerry_char_t header_buffer[header_req_sz];
    jerry_string_to_char_buffer (header, header_buffer, header_req_sz);
    header_buffer[header_req_sz] = '\0';

    httpclient_t client = {0};
    httpclient_data_t client_data = {0};

    char *buf = (char*) malloc (1024);
    client_data.response_buf = buf;
    client_data.response_buf_len = 1024;

    /* header */
    if (!jerry_value_has_error_flag(header)) {
      httpclient_set_custom_header(&client, header_buffer);
    }

    /* hint !!! method_buffer must be here */
    jerry_size_t method_req_sz = jerry_get_string_size (method);
    jerry_char_t method_buffer[method_req_sz];
    jerry_string_to_char_buffer (method, method_buffer, method_req_sz);
    method_buffer[method_req_sz] = '\0';

    if (strncmp (method_buffer, "POST", (size_t)method_req_sz) == 0) {
      // data
      jerry_value_t prop_data = jerry_create_string ((const jerry_char_t *) "data");
      jerry_value_t data = jerry_get_property(args_p[0], prop_data);

      // contentType
      jerry_value_t prop_contentType = jerry_create_string ((const jerry_char_t *) "contentType");
      jerry_value_t contentType = jerry_get_property(args_p[0], prop_contentType);

      /* data */
      jerry_size_t data_req_sz = jerry_get_string_size (data);
      jerry_char_t data_buffer[data_req_sz];
      jerry_string_to_char_buffer (data, data_buffer, data_req_sz);
      data_buffer[data_req_sz] = '\0';

      /* contentType */
      jerry_size_t contentType_req_sz = jerry_get_string_size (contentType);
      jerry_char_t contentType_buffer[contentType_req_sz];
      jerry_string_to_char_buffer (contentType, contentType_buffer, contentType_req_sz);
      contentType_buffer[contentType_req_sz] = '\0';

      client_data.post_content_type = contentType_buffer;
      client_data.post_buf = data_buffer;
      client_data.post_buf_len = data_req_sz;

      httpclient_post(&client, url_buffer, HTTP_PORT, &client_data);

      jerry_release_value(prop_data);
      jerry_release_value(prop_contentType);
      jerry_release_value(data);
      jerry_release_value(contentType);

      free(data_buffer);
      free(contentType_buffer);

    } else if (strncmp (method_buffer, "GET", (size_t)method_req_sz) == 0) {
      httpclient_get(&client, url_buffer, HTTP_PORT, &client_data);
    }

    jerry_value_t params[0];
    params[0] = jerry_create_string(client_data.response_buf);

    jerry_value_t this_val = jerry_create_undefined();
    jerry_value_t ret_val = jerry_call_function (args_p[1], this_val, &params, 1);

    jerry_release_value(params);
    jerry_release_value(this_val);
    jerry_release_value(ret_val);

    httpclient_close(&client, HTTP_PORT);

    jerry_release_value(header);
    jerry_release_value(method);
    jerry_release_value(url);
    jerry_release_value(prop_header);
    jerry_release_value(prop_method);
    jerry_release_value(prop_url);

    free(buf);
    free(method_buffer);
    free(url_buffer);
    free(header_buffer);
    buf = NULL;
  }
  return jerry_create_boolean (true);
}


void ml_http_init(void) {
  REGISTER_HANDLER(__http);
}