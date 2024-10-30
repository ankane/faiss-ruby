#pragma once

#include <ruby.h>
#include <ruby/thread.h>

#include "numo.hpp"

size_t check_shape(const numo::NArray& objects, size_t k);

template<typename F>
void without_gvl(F&& func) {
  auto wrapper = [](void* ptr) -> void* {
    (*static_cast<F*>(ptr))();
    return NULL;
  };
  rb_thread_call_without_gvl(wrapper, &func, RUBY_UBF_PROCESS, NULL);
}
