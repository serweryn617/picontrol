class CommandResponseError(Exception):
    def __init__(self, message: str, return_code: int):
        self.return_code = return_code
        super().__init__(message)

class DeviceNotFoundError(Exception):
    pass

class RequestedCommandError(Exception):
   pass
