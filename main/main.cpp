#include <NimBLEDevice.h>
BLEServer* pServer = NULL;
BLECharacteristic* pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
    uint32_t onPassKeyRequest() {
        printf("Server PassKeyRequest\n");
        return 123456;
    }

    bool onConfirmPIN(uint32_t pass_key) {
        printf("The passkey YES/NO number: %d\n", pass_key);
        return true;
    }

    void onAuthenticationComplete(ble_gap_conn_desc desc) {
        printf("Starting BLE work!\n");
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0) {
            printf("*********\n");
            printf("Received Value: ");
            for (int i = 0; i < rxValue.length(); i++)
                printf("%c", (char) rxValue[i]);

            printf("\n*********\n");
        }
    }
};

void connectedTask(void* parameter) {
    for (;;) {
        if (deviceConnected) {
            pTxCharacteristic->setValue(&txValue, 1);
            pTxCharacteristic->notify();
            txValue++;
        }

        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            pServer->startAdvertising();  // restart advertising
            printf("start advertising\n");
            oldDeviceConnected = deviceConnected;
        }
        // connecting
        if (deviceConnected && !oldDeviceConnected) {
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay between loops to reset watchdog timer
    }

    vTaskDelete(NULL);
}

void setup() {
    printf("================ BLE SETUP ================");
    BLEDevice::init("UART Service");
    printf("getMTU = %d\n", BLEDevice::getMTU());
    printf("setMTU = %d\n", BLEDevice::setMTU(500));
    printf("getMTU = %d\n", BLEDevice::getMTU());

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        NIMBLE_PROPERTY::NOTIFY);

    BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        NIMBLE_PROPERTY::WRITE);

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);

    // Start advertising
    pServer->getAdvertising()->start();
    printf("Waiting a client connection to notify...\n");

    printf("================ BLE SETUP:DONE ================");
}

void loop() {
}