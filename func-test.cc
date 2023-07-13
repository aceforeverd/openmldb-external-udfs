#include "base/mem_pool.h"
#include "base/string_ref.h"
#include "udf/openmldb_udf.h"

#include <iostream>
#include <cassert>

extern void json_array_sort(::openmldb::base::UDFContext *ctx,
                            ::openmldb::base::StringRef *json_array,
                            ::openmldb::base::StringRef *order,
                            ::openmldb::base::StringRef *column, int32_t n,
                            bool desc, ::openmldb::base::StringRef *out,
                            bool *is_null);

int main(int argc, char *argv[]) {
  openmldb::base::StringRef json =
      R"([{"a": "1", "b": "2"}, {"a": "3", "b": "9"}])";
  openmldb::base::StringRef key = "a";
  openmldb::base::StringRef value = "b";
  openmldb::base::StringRef out;
  bool is_null = true;

  openmldb::base::UDFContext ctx;
  ctx.pool = new openmldb::base::ByteMemoryPool();
  json_array_sort(&ctx, &json, &key, &value, 10, false, &out, &is_null);

  assert(is_null == false);
  assert(out.ToString() == "2,9");

  json_array_sort(&ctx, &json, &key, &value, 10, true, &out, &is_null);
  assert(is_null == false);
  assert(out.ToString() == "9,2");

  json_array_sort(&ctx, &json, &key, &value, 1, true, &out, &is_null);
  assert(is_null == false);
  assert(out.ToString() == "9");

  json_array_sort(&ctx, &json, &key, &value, 2, false, &out, &is_null);
  assert(is_null == false);
  assert(out.ToString() == "2,9");

  delete ctx.pool;
}
