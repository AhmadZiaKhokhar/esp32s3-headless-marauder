#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/OwnedListLifecycle.h"
#pragma once

template <typename ListType>
ListType* resetOwnedList(ListType*& list) {
  if (list != nullptr) {
    list->clear();
    delete list;
  }

  list = new ListType();
  return list;
}
