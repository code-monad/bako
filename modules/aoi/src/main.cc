#include <aoi/aoi.hh>

#include <boost/uuid/nil_generator.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
  auto logger = aoi::logger();

  auto context = std::make_shared<boost::asio::io_context>(2);

  auto group = aoi::work_group{context};


  auto task = std::make_shared<aoi::term_task>(fmt::format("test_task_echo" ), std::initializer_list<std::string>{"echo", "hello world"});
  logger.get()->info("task:{} logging to {}", task->name(), task->log_path());
  task->params()["test"] = "test_param";
  auto task_ls = std::make_shared<aoi::term_task>(fmt::format("test_task_ls" ), std::initializer_list<std::string>{"ls", "-al"});
  task_ls->set_next(task);
  group.add_task(task_ls);
  group.add_task(task);
  group.execute();

  auto injector = aoi::injector{context};
  using nlohmann::json;

  auto new_work = injector.parse(R"({"tasks":[{"type": "term", "name": "test-injector", "detail": {"command": ["echo", "injector"]}}]})"_json);
  new_work->execute();
  context->run();
  return 0;
}
