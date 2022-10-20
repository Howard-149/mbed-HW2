

#include "mbed.h"
#include "wifi_helper.h"

// Sensors drivers present in the BSP library
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#define IP_ADDRESS "192.168.158.139" //YOUR_IP_ADDRESS
#define PORT 6531 //YOUR_PORT


class SocketDemo {
    static constexpr size_t MAX_NUMBER_OF_ACCESS_POINTS = 10;
    static constexpr size_t MAX_MESSAGE_RECEIVED_LENGTH = 100;


public:
    SocketDemo() : _net(NetworkInterface::get_default_instance())
    {
    }

    ~SocketDemo()
    {
        if (_net) {
            _net->disconnect();
        }
    }

    void run()
    {
        if (!_net) {
            printf("Error! No network interface found.\r\n");
            return;
        }

        /* if we're using a wifi interface run a quick scan */
        if (_net->wifiInterface()) {
            /* the scan is not required to connect and only serves to show visible access points */
            wifi_scan();
        }

        /* connect will perform the action appropriate to the interface type to connect to the network */

        printf("Connecting to the network...\r\n");

        nsapi_size_or_error_t result = _net->connect();
        if (result != 0) {
            printf("Error! _net->connect() returned: %d\r\n", result);
            return;
        }

        print_network_info();

        /* opening the socket only allocates resources */
        result = _socket.open(_net);
        if (result != 0) {
            printf("Error! _socket.open() returned: %d\r\n", result);
            return;
        }

        /* now we have to find where to connect */

        SocketAddress address;
        address.set_ip_address(IP_ADDRESS);
        address.set_port(PORT);

        /* we are connected to the network but since we're using a connection oriented
         * protocol we still need to open a connection on the socket */


        result = _socket.connect(address);
        if (result != 0) {
            printf("Error! _socket.connect() returned: %d\r\n", result);
            return;
        }

        /* exchange an HTTP request and response */
        //sensor part//
        float sensor_value = 0;
        int16_t pDataXYZ[3] = {0};
        float pGyroDataXYZ[3] = {0};
        int sample_num=0;
       
        BSP_GYRO_Init();
        BSP_ACCELERO_Init();
        while (1){
            ++sample_num;
            BSP_ACCELERO_AccGetXYZ(pDataXYZ);
            float x = pDataXYZ[0], y = pDataXYZ[1], z = pDataXYZ[2];
            BSP_GYRO_GetXYZ(pGyroDataXYZ);
            float gx = pGyroDataXYZ[0], gy = pGyroDataXYZ[1], gz = pGyroDataXYZ[2];
            char acc_json[1024];
            int len = sprintf(acc_json,"{\"x\":%f,\"y\":%f,\"z\":%f,\"gx\":%f,\"gy\":%f,\"gz\":%f,\"s\":%d}",(float)((int)(x*10000))/10000,(float)((int)(y*10000))/10000, (float)((int)(z*10000))/10000,gx,gy,gz,sample_num);
            nsapi_size_or_error_t response = 0;
            response = _socket.send(acc_json,len);
            if (0 >= response){
                printf("Error seding: %d\n", response);
                _socket.close();
                break;
            }
            ThisThread::sleep_for(1s);
        } 


        printf("Demo concluded successfully \r\n");
    }

private:

    void wifi_scan()
    {
        WiFiInterface *wifi = _net->wifiInterface();

        WiFiAccessPoint ap[MAX_NUMBER_OF_ACCESS_POINTS];

        /* scan call returns number of access points found */
        int result = wifi->scan(ap, MAX_NUMBER_OF_ACCESS_POINTS);

        if (result <= 0) {
            printf("WiFiInterface::scan() failed with return value: %d\r\n", result);
            return;
        }

        printf("%d networks available:\r\n", result);

        for (int i = 0; i < result; i++) {
            printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n",
                   ap[i].get_ssid(), get_security_string(ap[i].get_security()),
                   ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
                   ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
                   ap[i].get_rssi(), ap[i].get_channel());
        }
        printf("\r\n");
    }

    void print_network_info()
    {
        /* print the network info */
        SocketAddress a;
        _net->get_ip_address(&a);
        printf("IP address: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_netmask(&a);
        printf("Netmask: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_gateway(&a);
        printf("Gateway: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    }

private:
    NetworkInterface *_net;

#if MBED_CONF_APP_USE_TLS_SOCKET
    TLSSocket _socket;
#else
    TCPSocket _socket;
#endif // MBED_CONF_APP_USE_TLS_SOCKET
};

int main() {
    printf("\r\nStarting HW2 Client\r\n\r\n");


    SocketDemo *example = new SocketDemo();
    MBED_ASSERT(example);
    example->run();
    printf("END");
    return 0;
}
