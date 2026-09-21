// Copyright 2025 ROS Industrial Consortium Asia Pacific
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <pybind11/stl.h>

#include "rmf2_scheduler_py/storage/schedule_stream.hpp"
#include "rmf2_scheduler/storage/schedule_stream.hpp"

namespace rmf2_scheduler_py
{

namespace storage
{

// Every ScheduleStream pure virtual follows the same shape: look up a
// same-named Python override, call it with the forwarded args, and expect
// a (bool result, str error) tuple back -- setting `error` and returning
// false gracefully (rather than throwing) whenever that contract isn't
// met, exactly like PyTaskExecutor::start() does for TaskExecutor.
#define RS_SCHEDULE_STREAM_PY_OVERRIDE(fn, ...)                                        \
  pybind11::gil_scoped_acquire gil;  /* Acquire the GIL while in this scope. */        \
  pybind11::function override = pybind11::get_override(this, #fn);                    \
  if (!override) {                                                                     \
    error = "ScheduleStream " #fn " failed: cannot find defined Python function";      \
    return false;                                                                      \
  }                                                                                     \
  auto obj = override(__VA_ARGS__);                                                    \
  if (!py::isinstance<py::tuple>(obj)) {                                               \
    error = "ScheduleStream " #fn " failed: Invalid Python return type.";              \
    return false;                                                                      \
  }                                                                                     \
  py::tuple tuple_obj = obj;                                                           \
  if (py::len(tuple_obj) != 2) {                                                       \
    error = "ScheduleStream " #fn " failed: Invalid number of returns";                \
    return false;                                                                      \
  }                                                                                     \
  bool result = tuple_obj[0].cast<bool>();                                             \
  if (!result) {                                                                       \
    error = tuple_obj[1].cast<std::string>();                                          \
    return false;                                                                      \
  }                                                                                     \
  return true

/// Trampoline that lets a Python subclass of ScheduleStream (e.g. a
/// SQLAlchemy-backed implementation) be dispatched into by the native
/// Scheduler. Without this, ScheduleStream can only be produced via the
/// create_default/create_simple factories -- it has no pybind11 py::init(),
/// so Python cannot construct or subclass it at all.
class PyScheduleStream : public rmf2_scheduler::storage::ScheduleStream
{
public:
  using rmf2_scheduler::storage::ScheduleStream::ScheduleStream;

  /// Trampoline
  bool read_schedule(
    rmf2_scheduler::cache::ScheduleCache::Ptr cache,
    const rmf2_scheduler::data::TimeWindow & time_window,
    std::string & error
  ) override
  {
    RS_SCHEDULE_STREAM_PY_OVERRIDE(read_schedule, cache, time_window);
  }

  bool write_schedule(
    rmf2_scheduler::cache::ScheduleCache::ConstPtr cache,
    const rmf2_scheduler::data::TimeWindow & time_window,
    std::string & error
  ) override
  {
    RS_SCHEDULE_STREAM_PY_OVERRIDE(write_schedule, cache, time_window);
  }

  bool write_schedule(
    rmf2_scheduler::cache::ScheduleCache::ConstPtr cache,
    const std::vector<rmf2_scheduler::data::ScheduleChangeRecord> & records,
    std::string & error
  ) override
  {
    RS_SCHEDULE_STREAM_PY_OVERRIDE(write_schedule, cache, records);
  }

  bool refresh_tasks(
    rmf2_scheduler::cache::ScheduleCache::Ptr cache,
    const std::vector<std::string> & ids,
    std::string & error
  ) override
  {
    RS_SCHEDULE_STREAM_PY_OVERRIDE(refresh_tasks, cache, ids);
  }
};

#undef RS_SCHEDULE_STREAM_PY_OVERRIDE

void init_schedule_stream_py(py::module & m)
{
  using namespace rmf2_scheduler;  // NOLINT(build/namespaces)
  using namespace rmf2_scheduler::storage;  // NOLINT(build/namespaces)

  py::module m_storage = m.def_submodule("storage");

  py::class_<
    ScheduleStream,
    rmf2_scheduler_py::storage::PyScheduleStream,
    ScheduleStream::Ptr
  >(
    m_storage,
    "ScheduleStream",
    R"(
    Stream for the schedule

    Subclassable from Python: override read_schedule/write_schedule/
    refresh_tasks, each returning a (bool result, str error) tuple, to plug
    in a custom backend (e.g. SQLAlchemy). Instances produced by the
    create_default/create_simple factories use the same calling convention.
    )"
  )
  .def(py::init<>())
  .def(
    "read_schedule",
    [](
      ScheduleStream & self,
      cache::ScheduleCache::Ptr schedule_cache,
      const data::TimeWindow & time_window
    ) {
      std::string error;
      bool result = self.read_schedule(schedule_cache, time_window, error);
      return py::make_tuple(result, error);
    }
  )
  .def(
    "write_schedule",
    [](
      ScheduleStream & self,
      cache::ScheduleCache::ConstPtr schedule_cache,
      const data::TimeWindow & time_window
    ) {
      std::string error;
      bool result = self.write_schedule(schedule_cache, time_window, error);
      return py::make_tuple(result, error);
    }
  )
  .def(
    "write_schedule",
    [](
      ScheduleStream & self,
      cache::ScheduleCache::ConstPtr schedule_cache,
      const std::vector<data::ScheduleChangeRecord> & records
    ) {
      std::string error;
      bool result = self.write_schedule(schedule_cache, records, error);
      return py::make_tuple(result, error);
    }
  )
  .def(
    "refresh_tasks",
    [](
      ScheduleStream & self,
      cache::ScheduleCache::Ptr schedule_cache,
      const std::vector<std::string> & ids
    ) {
      std::string error;
      bool result = self.refresh_tasks(schedule_cache, ids, error);
      return py::make_tuple(result, error);
    }
  )
  .def_static(
    "create_default",
    &ScheduleStream::create_default
  )
  .def_static(
    "create_simple",
    &ScheduleStream::create_simple
  )
  ;
}

}  // namespace storage

}  // namespace rmf2_scheduler_py
