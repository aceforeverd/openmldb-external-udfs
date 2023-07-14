#include "base/mem_pool.h"
#include "base/string_ref.h"
#include "simdjson.h"
#include "udf/openmldb_udf.h"
#include <map>
#include <sstream>
#include <string_view>

extern "C"
void json_array_sort(::openmldb::base::UDFContext *ctx,
                     ::openmldb::base::StringRef *json_array,
                     ::openmldb::base::StringRef *order,
                     ::openmldb::base::StringRef *column, int32_t n, bool desc,
                     ::openmldb::base::StringRef *out) {
  // empty string on error
  out->size_ = 0;

  simdjson::ondemand::parser parser;
  simdjson::padded_string json = json_array->ToString();
  simdjson::ondemand::document doc;
  auto err = parser.iterate(json).get(doc);
  if (err) {
    return;
  }
  simdjson::ondemand::array arr;
  err = doc.get_array().get(arr);
  if (err) {
    return;
  }

  std::string_view order_ref(order->data_, order->size_);
  std::string_view column_ref(column->data_, column->size_);
  std::map<std::string, std::string> container;

  for (auto ele : arr) {
    simdjson::ondemand::object obj;
    auto error = ele.get_object().get(obj);
    if (error) {
      continue;
    }

    std::string_view order_val;
    err = obj[order_ref].get(order_val);
    if (err) {
      continue;
    }
    std::string_view column_val;
    err = obj[column_ref].get(column_val);
    if (err) {
      continue;
    }

    container.emplace(order_val, column_val);
  }

  std::stringstream ss;
  uint32_t topn = static_cast<uint32_t>(n);
  auto sz = container.size();

  for (uint32_t i = 0; i < topn && i < sz; ++i) {
    if (desc) {
      auto it = std::next(container.crbegin(), i);
      ss << it->second;
      if (std::next(it, 1) != container.crend() && i + 1 < topn) {
        ss << ",";
      }
    } else {
      auto it = std::next(container.cbegin(), i);
      ss << it->second;
      if (std::next(it, 1) != container.cend() && i + 1 < topn) {
        ss << ",";
      }
    }
  }

  auto str = ss.str();
  auto ssz = str.size();
  char *buf = ctx->pool->Alloc(ssz);
  memcpy(buf, str.data(), ssz);
  out->data_ = buf;
  out->size_ = ssz;
}


