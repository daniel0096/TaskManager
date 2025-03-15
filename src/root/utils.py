import app
from enum import Enum

class LogLevel(Enum):
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_LOG = 1,
    LOG_LEVEL_ERR = 2,
    LOG_LEVEL_MAX_NUM = 3

def TRACE_LOG(log_level: int, message: str) -> str:
    return app.PY_TRACE(log_level, message)