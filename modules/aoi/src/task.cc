#include <aoi/task.hh>
#include <aoi/log.hh>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <subprocess/subprocess.hpp>

#include <vector>

aoi::basic_task::basic_task(const std::string& name) : _name{name} {

}

const std::string& aoi::basic_task::name() { return _name; }

void aoi::basic_task::set_done(const bool done) { _done_flag = done; }
void aoi::basic_task::set_have_next(const bool have_next) { _have_next = have_next; }

void aoi::basic_task::set_next(std::shared_ptr<aoi::basic_task> next) {
  _next = next;
  _have_next = true;
}

void aoi::basic_task::set_executor(std::shared_ptr<boost::asio::io_context> executor){ _executor = executor; }

aoi::term_task::term_task(const std::string& name, const aoi::term_task::command_t& command, const aoi::term_task::handler_t& handler) : aoi::basic_task{ name }, _cmd{ command }, _handler{ handler } {}

aoi::term_task::term_task(const std::string& name, std::initializer_list<std::string> command, const aoi::term_task::handler_t& handler) : aoi::basic_task{ name }, _cmd{ command }, _handler{ handler } {}


void aoi::term_task::process_detail(const std::string name, const aoi::term_task::command_t& command, boost::asio::yield_context yield_context) {

  auto logger = aoi::logger();
  std::string command_str = boost::algorithm::join(this->_cmd, ", ");
  logger.get()->info("task: {} call command: {}", name, command_str);

  auto proc = subprocess::Popen(command, subprocess::output{subprocess::PIPE});
  auto res = proc.communicate().first;
  if(_handler) {
    _handler(name, proc.retcode(), res.buf.data());
  }
  else {
    this->default_handler(name, proc.retcode(), res.buf.data());
  }
  this->set_done();
  if(this->have_next())
    this->next()->process();
}

void aoi::term_task::process() {
  auto logger = aoi::logger();
  if(_cmd.empty()) {
    logger.get()->critical("task: {} is empty!", this->name());
    return ;
  }

  if(!this->executor()) {
    logger.get()->critical("task: {} have no executor!", this->name());
    return ;
  }

  boost::asio::spawn(*(this->executor()), boost::bind(&aoi::term_task::process_detail, this, this->name(), this->_cmd, boost::placeholders::_1));
}



void aoi::term_task::default_handler(const std::string& name, int ret, const std::string& buffer) {
  auto logger = aoi::logger();
  if(ret != 0) {
    logger.get()->critical("task: {}, exit: {}", name, ret);
  } else {
    logger.get()->info("task: {} success finished with: {}, output:\n{}", name, ret, buffer);
  }
}
