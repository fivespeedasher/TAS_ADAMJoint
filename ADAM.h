#ifndef ADAM_H
#define ADAM_H
#include <modbus.h>
#include <vector>
#include <string>

using namespace std;

class ADAM
{
public:
    ADAM(const char *ip, int port);
    ~ADAM();
    const char *ip;
    int port;
    modbus_t *ctx;
    int set_non_blocking(int fd);
    int connect(bool debug);
    int disconnect();
};

class ADAM4051 : public ADAM
{
public:
    ADAM4051(const ADAM& adam, int slave_id, int total_coils = 8);
    ~ADAM4051();
    vector<uint8_t> state_coils;
    int read_coils();
private:
    int slave_id;
    int total_coils;
    modbus_t *ctx;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

class ADAM4168 : public ADAM
{
public:
    ADAM4168(const ADAM& adam, int slave_id, int total_channels, float duty_cycles);
    ~ADAM4168();
    int InitPulse(float duty_cycles);
    int SetMode(const vector<int>& PulseChannel);
    int StartPulse(const vector<int>& channels, uint16_t pulse_times);
    int SetDO(int channels, bool value);
private:
    int slave_id;
    int total_channels;
    modbus_t *ctx;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

class ADAM4068 : public ADAM
{
public:
    ADAM4068(const ADAM& adam, int slave_id, int total_coils);
    ~ADAM4068();
    vector<uint8_t> state_coils;
    int write_coil(int ch, bool flag);
private:
    int slave_id;
    int total_coils;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

#endif