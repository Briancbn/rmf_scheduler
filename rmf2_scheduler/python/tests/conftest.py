# Copyright 2025 ROS Industrial Consortium Asia Pacific
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from unittest.mock import create_autospec

import pytest
from rmf2_scheduler import ProcessExecutor, TaskExecutor
from rmf2_scheduler.storage import ScheduleStream

# Each Mock* class below is a trampoline subclass whose overrides are all
# delegated to an autospec'd Mock, so a test configures behavior the same
# way it would for any other mock (return_value/side_effect) and can assert
# on calls (call_count/call_args) the same way too.
#
# Call the method through the pybind-registered base class, e.g.
# ScheduleStream.read_schedule(mock_schedule_stream, ...), not directly on
# the instance. A direct instance call resolves to the mock attribute via
# plain Python attribute lookup and never reaches the C++ trampoline;
# routing it through the base class forces the native virtual dispatch that
# actually looks up and calls the Python override.


class MockScheduleStream(ScheduleStream):
    def __init__(self):
        super().__init__()
        self.mock = create_autospec(spec=ScheduleStream, instance=True)

    def __getattribute__(self, name):
        _mock = object.__getattribute__(self, "mock")
        if name == "mock":
            return _mock
        return getattr(_mock, name)


class MockProcessExecutor(ProcessExecutor):
    def __init__(self):
        super().__init__()
        self.mock = create_autospec(spec=ProcessExecutor, instance=True)

    def __getattribute__(self, name):
        _mock = object.__getattribute__(self, "mock")
        if name == "mock":
            return _mock
        return getattr(_mock, name)


class MockTaskExecutor(TaskExecutor):
    def __init__(self):
        super().__init__()
        self.mock = create_autospec(spec=TaskExecutor, instance=True)

    def __getattribute__(self, name):
        _mock = object.__getattribute__(self, "mock")
        if name == "mock":
            return _mock
        return getattr(_mock, name)


@pytest.fixture
def mock_schedule_stream():
    return MockScheduleStream()


@pytest.fixture
def mock_process_executor():
    return MockProcessExecutor()


@pytest.fixture
def mock_task_executor():
    return MockTaskExecutor()
