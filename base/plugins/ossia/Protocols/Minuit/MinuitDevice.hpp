#pragma once
#include <DeviceExplorer/Protocol/DeviceInterface.hpp>
#include "MinuitSpecificSettings.hpp"
//#include <API/Headers/Network/Protocols.h>
//#include <API/Headers/Network/Device.h>
//#include <API/Headers/Network/Address.h>
class MinuitDevice : public DeviceInterface
{
    public:
        MinuitDevice(const DeviceSettings& settings):
            DeviceInterface{settings}
        {
            //using namespace OSSIA;
            //OSC oscDeviceParameters{settings.host, settings.inputPort, settings.outputPort};

        }

        void addPath(const AddressSettings& address) override
        {

        }

        void updatePath(const AddressSettings& address) override
        {

        }

        void removePath(const QString& path) override
        {

        }

        void sendMessage(Message& mess) override
        {

        }

        bool check(const QString& str) override
        {
            return false;
        }
};