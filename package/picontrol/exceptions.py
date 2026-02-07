from picontrol.return_codes import RETURN_CODES


class CommandResponseError(Exception):
    def __init__(self, message: str, return_code: int):
        self.return_code = return_code
        status = f"Status: {RETURN_CODES[return_code]}" if return_code in RETURN_CODES else "Unknown status"
        super().__init__(message + "; " + status)


class DeviceNotFoundError(Exception):
    pass


class RequestedCommandError(Exception):
    pass
