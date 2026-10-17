#ifndef ML307_BOARD_H
#define ML307_BOARD_H

#include <memory>
#include <string>
#include <at_modem.h>
#include "board.h"


class Ml307Board : public Board {
protected:
    std::unique_ptr<AtModem> modem_;
    gpio_num_t tx_pin_;
    gpio_num_t rx_pin_;
    gpio_num_t dtr_pin_;
    NetworkEventCallback network_event_callback_;

    virtual std::string GetBoardJson() override;

    // Internal helper to trigger network event callback
    void OnNetworkEvent(NetworkEvent event, const std::string& data = "");
    
    // Network initialization task (runs in FreeRTOS task)
    static void NetworkTaskEntry(void* arg);
    void NetworkTask();

public:
    Ml307Board(gpio_num_t tx_pin, gpio_num_t rx_pin, gpio_num_t dtr_pin = GPIO_NUM_NC);
    virtual std::string GetBoardType() override;

    // 通过 4G 模组发送短信（走共享 AT UART，自动处理 '>' 提示符）
    // number: 纯数字号码；text: UTF-8 文本（ASCII 走 GSM，含中文自动走 UCS2）
    // 返回 true 表示模组回 +CMGS 发送成功
    bool SendSms(const std::string& number, const std::string& text);

    virtual void StartNetwork() override;
    virtual void SetNetworkEventCallback(NetworkEventCallback callback) override;
    virtual NetworkInterface* GetNetwork() override;
    virtual const char* GetNetworkStateIcon() override;
    virtual void SetPowerSaveLevel(PowerSaveLevel level) override;
    virtual AudioCodec* GetAudioCodec() override { return nullptr; }
    virtual std::string GetDeviceStatusJson() override;
};

#endif // ML307_BOARD_H
