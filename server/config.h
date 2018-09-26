#pragma once

class Config {
  public:
    static bool Async;
  private:
    Config() {}
    virtual ~Config() {}
    Config(const Config&) {}
};