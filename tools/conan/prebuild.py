from tools.conan import utils

def setup_monitoring_config():
    utils.cpy("data/configs/MangoHud.conf", "build/MangoHud.conf")