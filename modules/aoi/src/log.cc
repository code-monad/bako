#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <aoi/log.hh>

#include <iostream>

using namespace aoi;

logger::logger(spdlog::level::level_enum level, const std::string& base_dir) : _logger{spdlog::get("AOI_LOG")} {
  if(_logger == nullptr){
    try{
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(fmt::format("{}/aoi.log", base_dir), 23, 59);
      spdlog::sinks_init_list sink_list = { daily_sink, console_sink };
      spdlog::init_thread_pool(10000, 1);
      _logger = std::make_shared<spdlog::logger>("AOI_LOG", sink_list.begin(), sink_list.end());
      spdlog::register_logger(_logger);
    }
    catch (const spdlog::spdlog_ex& e) {
      std::cerr << "Failed to init AOI_LOG: " << e.what() << std::endl;
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



file_logger::file_logger(const std::string& name, spdlog::level::level_enum level, const std::string base_dir) : _logger{spdlog::get(name)} {
  if(_logger == nullptr) {
    try{
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("{}/{}.log", base_dir, name), true);
      _logger = std::make_shared<spdlog::logger>(name, file_sink);
    }
    catch (const spdlog::spdlog_ex& e) {
    std::cerr << "Failed to init spdlog: " << e.what() << std::endl;
  } catch(std::exception& e) {
    std::cerr << "Unexpected error happened while initing spdlog: " << e.what() << std::endl;
    throw;
    }
  }
}


std::shared_ptr<spdlog::logger> file_logger::get() {
  return _logger;
}

void file_logger::set_level(spdlog::level::level_enum level) {
  _logger->set_level(level);
}
