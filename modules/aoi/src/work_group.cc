#include <aoi/work_group.hh>
#include <aoi/task.hh>

aoi::work_group::work_group(std::shared_ptr<boost::asio::io_context> executor) : _executor{executor}, _tasks{} {}

void aoi::work_group::add_task(std::shared_ptr<basic_task> task) {
  if(_tasks.empty()) {
    _begin = task;
  } else {
    _last->set_next(task);
  }
  _last = task;
  _tasks[task->uuid()] = task;
  task->set_executor(_executor);
}


std::shared_ptr<aoi::basic_task> aoi::work_group::begin() noexcept {
    return _begin;
}

std::shared_ptr<aoi::basic_task> aoi::work_group::last() noexcept {
    return _last;
}


void aoi::work_group::execute() {

  for(auto task = this->begin(); task != this->end(); task = task->next()){
    task->process();
  }
}
