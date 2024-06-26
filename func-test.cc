#include "base/mem_pool.h"
#include "base/string_ref.h"
#include "udf/openmldb_udf.h"

#include <iostream>
#include <cassert>

extern "C" void json_array_sort(::openmldb::base::UDFContext *ctx,
                            ::openmldb::base::StringRef *json_array,
                            ::openmldb::base::StringRef *order,
                            ::openmldb::base::StringRef *column, int32_t n,
                            bool desc, ::openmldb::base::StringRef *out);
extern "C" void list_at(::openmldb::base::UDFContext *ctx,
             ::openmldb::base::StringRef *list, int idx,
             ::openmldb::base::StringRef *del, ::openmldb::base::StringRef *out);

int main(int argc, char *argv[]) {
  openmldb::base::StringRef json =
      R"([{"a": "1", "b": "2"}, {"a": "3", "b": "9"}])";
  openmldb::base::StringRef key = "a";
  openmldb::base::StringRef value = "b";
  openmldb::base::StringRef out;

  openmldb::base::UDFContext ctx;
  ctx.pool = new openmldb::base::ByteMemoryPool();
  json_array_sort(&ctx, &json, &key, &value, 10, false, &out);

  assert(out.ToString() == "2,9");

  json_array_sort(&ctx, &json, &key, &value, 10, true, &out);
  assert(out.ToString() == "9,2");

  json_array_sort(&ctx, &json, &key, &value, 1, true, &out);
  assert(out.ToString() == "9");

  json_array_sort(&ctx, &json, &key, &value, 2, false, &out);
  assert(out.ToString() == "2,9");


  // list_at
  openmldb::base::StringRef list = "a,b,c";
  openmldb::base::StringRef del = ",";
  list_at(&ctx, &list, 0, &del, &out);
  assert(out.ToString() == "a");

  list_at(&ctx, &list, 1, &del, &out);
  assert(out.ToString() == "b");

  list_at(&ctx, &list, 2, &del, &out);
  assert(out.ToString() == "c");

  list_at(&ctx, &list, 4, &del, &out);
  assert(out.ToString() == "");

  delete ctx.pool;
}
