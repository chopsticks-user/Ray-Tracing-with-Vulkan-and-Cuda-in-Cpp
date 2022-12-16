from tools.conan import utils

def setup_monitoring_config(config_path: str):
    utils.cpy(config_path, "build/MangoHud.conf")