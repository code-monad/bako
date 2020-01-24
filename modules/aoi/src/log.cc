#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include <aoi/log.hh>

#include <iostream>

using namespace aoi;

logger::logger(spdlog::level::level_enum level) : _logger{spdlog::get("AOI_LOG")} {
  if(_logger == nullptr){
    try{
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("aoi.log", 23, 59);
      spdlog::sinks_init_list sink_list = { daily_sink, console_sink };
      spdlog::init_thread_pool(10000, 1);
      _logger = std::make_shared<spdlog::logger>("AOI_LOG", sink_list.begin(), sink_list.end());
      spdlog::register_logger(_logger);
    }
    catch (const spdlog::spdlog_ex& e) {
      std::cerr << "Failed to init spdlog: " << e.what() << std::endl;
    } catch(std::exception& e) {
      std::cerr << "Unexpected error happened while initing spdlog: " << e.what() << std::endl;
      throw;
    }
  }

  this->set_level(level);

}


std::shared_ptr<spdlog::logger> logger::get() {
  return _logger;
}

void logger::set_level(spdlog::level::level_enum level) {
  _logger->set_level(level);
}
