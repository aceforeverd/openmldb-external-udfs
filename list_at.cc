#include "base/mem_pool.h"
#include "base/string_ref.h"
#include "udf/openmldb_udf.h"

#include <string_view>

extern "C"
void list_at(::openmldb::base::UDFContext *ctx,
             ::openmldb::base::StringRef *list, int32_t idx,
             ::openmldb::base::StringRef *del, ::openmldb::base::StringRef *out) {
  out->size_ = 0;
  if (list == nullptr || del == nullptr || out == nullptr) {
    return;
  }


  std::string_view data(list->data_, list->size_);
  std::string_view deli(del->data_, del->size_);

  int i = 0;
  size_t start_pos = 0;
  size_t pos = 0;
  for (; i <= static_cast<uint32_t>(idx); i++) {
    pos = data.find(deli, start_pos);
    if (pos == std::string_view::npos) {
      // not found
      pos = data.size();
      i++;
      break;
    } else {
      // update next start_pos
      if (i < static_cast<uint32_t>(idx)) {
        start_pos = pos + deli.size();
      }
    }
  }

  if (i != static_cast<uint32_t>(idx) + 1) {
    return;
  }

  std::string_view o = data.substr(start_pos, pos - start_pos);
  char* buf = ctx->pool->Alloc(o.size());
  memcpy(buf, o.data(), o.size());
  out->data_ = buf;
  out->size_ = o.size();
}
