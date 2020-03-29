// test program for aoi

#include <aoi/aoi.hh>

#include <boost/uuid/nil_generator.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
  auto logger = aoi::logger();

  auto context = std::make_shared<boost::asio::io_context>(2);

  // test workgroup from api
  auto group = aoi::work_group{context};
  auto task = std::make_shared<aoi::term_task>(fmt::format("test_task_echo" ), std::initializer_list<std::string>{"echo", "hello world"});
  logger.get()->info("task:{} logging to {}", task->name(), task->log_path());
  task->params()["test"] = "test_param";
  auto task_ls = std::make_shared<aoi::term_task>(fmt::format("test_task_ls" ), std::initializer_list<std::string>{"ls", "-al"});
  task_ls->set_next(task);
  group.add_task(task_ls);
  group.add_task(task);
  group.execute();


  // test json injector
  auto injector = aoi::injector{context};
  using nlohmann::json;
  std::shared_ptr<aoi::work_group> new_work = nullptr;
  if(argc > 1) {
    // if filename specifiled, read from file
    std::string filename = argv[1];
    std::fstream reader(filename);
    if(reader.is_open()){
      json task_content;
      reader >> task_content;
      std::cout << "Content is : " << task_content << std::endl;
      new_work = injector.parse(task_content);
      new_work->execute();
    } else {
      std::cout << "file " << filename << "error" << std::endl;
    }
  } else {
    new_work = injector.parse(R"({"tasks":[{"type": "term", "name": "test-injector", "detail": {"command": ["echo", "injector"]}}]})"_json);
      new_work->execute();
  }
  
  context->run();
  return 0;
}
