#pragma once

class Config {
  public:
    static bool Async;
    static void ReadConfig(int argc, char** argv);
  private:
    Config() {}
    virtual ~Config() {}
    Config(const Config&) {}
};