#include <aoi/injector.hh>
#include <aoi/work_group.hh>
#include <aoi/task.hh>

#include <iostream>
#include <memory>
#include <stdexcept>

using nlohmann::json;

constexpr unsigned int hash(const char* str, int h = 0) { return !str[h] ? 5381 : (hash(str, h+1)*33) ^ str[h]; }

aoi::injector::injector(std::shared_ptr<boost::asio::io_context> executor) : _executor{executor} { }

std::unique_ptr<aoi::work_group> aoi::injector::parse(const aoi::injector::input_type& input) {  
  auto work_group = std::make_unique<aoi::work_group>(_executor);
  if(input.find("tasks") != input.end()){
    auto tasks = input["tasks"];
    for(auto& [index,task] : tasks.items()){
      work_group->add_task(this->parse_task(task["detail"]["command"], task["type"].get<std::string>(), task["name"].get<std::string>()));
    }
  }

  return std::move(work_group);
}

std::shared_ptr<aoi::basic_task> aoi::injector::parse_task(const aoi::injector::input_type& input, const std::string task_type, const std::string name) {
  std::shared_ptr<aoi::basic_task> task;
  switch(hash(task_type.c_str())){
  case hash("term"):
    {	
      task = std::make_shared<aoi::term_task>(name, input.get<std::vector<std::string>>());
    }
    break;
  default:
    throw std::runtime_error("unknow task type");
    break;
  }
  return task;
}

