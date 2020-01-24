#include <aoi/work_group.hh>
#include <aoi/task.hh>

aoi::work_group::work_group(std::shared_ptr<boost::asio::io_context> executor) : _executor{executor}, _tasks{} {}

void aoi::work_group::add_task(std::shared_ptr<basic_task> task) {
  if(_tasks.empty())
    _begin = task;
  _tasks[task->uuid()] = task;
  task->set_executor(_executor);
}


std::shared_ptr<aoi::basic_task> aoi::work_group::begin() noexcept {
    return _begin;
}

void aoi::work_group::execute() {
  if(this->begin())
    this->begin()->process();
}
