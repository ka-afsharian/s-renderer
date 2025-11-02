#include "engproj/logger/logger.hpp"

using namespace engproj::logger;

logger<loglevel::debug> engproj::logger::g_logger{"renderer"};
logger<loglevel::debug> engproj::logger::e_logger{"engine"};
logger<loglevel::debug> engproj::logger::t_logger{"engine"};
